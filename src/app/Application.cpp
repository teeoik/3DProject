#include "app/Application.h"
#include "gfx/ModelLoader.h"
#include "ui/ViewportPanel.h"

#include <iostream>
#include <optional>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>



namespace app
{
    Application::Application()
        : window_(1280, 720, "3DProject")
    {
        window_.makeContextCurrent();
        window_.setVsync(true);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("Failed to initialize GLAD");

        imgui_.init(window_.nativeHandle(), "#version 330"); // GLFW + OpenGL3 backends
    }

    int Application::run()
    {
        while (!window_.shouldClose())
        {
            window_.pollEvents();

            imgui_.beginFrame();
            drawUi();
            imgui_.render();

            // Clear and set viewport for scene rendering
            const auto [fbw, fbh] = window_.framebufferSize();
            glViewport(0, 0, fbw, fbh);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            imgui_.endFrame();

            window_.swapBuffers();
        }

        return 0;
    }

    void Application::drawUi()
    {
        constexpr ImGuiWindowFlags HOST_FLAGS =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_MenuBar;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("DockHost", nullptr, HOST_FLAGS);
        ImGui::PopStyleVar(2);

        ImGuiID dockspaceId = ImGui::GetID("MainDockspace");
        ImGui::DockSpace(dockspaceId, ImVec2(0, 0), ImGuiDockNodeFlags_None);

        if (firstFrame_)
        {
            firstFrame_ = false;
            initializeDockingLayout(dockspaceId, viewport->Size);
        }

        ImGui::End();

        drawToolbarPanel();
        drawModelTreePanel();
        drawModelViewPanel();
    }

    void Application::initializeDockingLayout(ImGuiID dockspaceId, ImVec2 viewportSize)
    {
        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceId, viewportSize);

        ImGuiID dockMain = dockspaceId;
        ImGuiID dockLeft;
        ImGuiID dockTop;

        ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Up, 0.20f, &dockTop, &dockMain);
        ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.25f, &dockLeft, &dockMain);

        ImGui::DockBuilderDockWindow("Toolbar", dockTop);
        ImGui::DockBuilderDockWindow("Model Tree", dockLeft);
        ImGui::DockBuilderDockWindow("Model View", dockMain);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    void Application::drawToolbarPanel()
    {
        ImGui::Begin("Toolbar");

        if (ImGui::BeginTabBar("ToolbarTabs"))
        {
            if (ImGui::BeginTabItem("File"))
            {
                if (ImGui::Button("Open OBJ", ImVec2(100, 0)))
                {
                    auto filePath = imgui_.openFileDialog();
                    if (filePath.has_value())
                    {
                        selectedObjFile_ = filePath.value();
                        try
                        {
                            currentModel_ = gfx::ModelLoader::load(selectedObjFile_.value());
                            scene_.model = currentModel_;
                        }
                        catch (const gfx::ModelLoadException& e)
                        {
                            std::cerr << "Error loading model: " << e.what() << std::endl;
                            currentModel_ = std::nullopt;
                            scene_.model = std::nullopt;
                        }
                    }
                }

                ImGui::SameLine();
                if (selectedObjFile_.has_value())
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "File: %s", selectedObjFile_.value().c_str());

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Graphics"))
            {
                ImGui::BeginGroup();
                ImGui::Text("Background");

                bool isLight = renderSettings_.lightBackground;
                if (ImGui::RadioButton("Light", isLight))
                    renderSettings_.lightBackground = true;
                ImGui::SameLine();
                if (ImGui::RadioButton("Dark", !isLight))
                    renderSettings_.lightBackground = false;

                ImGui::SetNextItemWidth(150);
                const char* bgTypes[] = { "Solid Color", "Gradient", "Checkered" };
                int currentBg = static_cast<int>(renderSettings_.backgroundType);
                if (ImGui::Combo("##Background", &currentBg, bgTypes, IM_ARRAYSIZE(bgTypes)))
                    renderSettings_.backgroundType = static_cast<gfx::RenderSettings::BackgroundType>(currentBg);
                ImGui::EndGroup();

                ImGui::SameLine();
                ImGui::Dummy(ImVec2(30, 0));
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::Text("Draw Mode");
                if (ImGui::RadioButton("Mesh", renderSettings_.drawMode == gfx::RenderSettings::DrawMode::Mesh))
                    renderSettings_.drawMode = gfx::RenderSettings::DrawMode::Mesh;
                ImGui::EndGroup();

                if (renderSettings_.drawMode == gfx::RenderSettings::DrawMode::Mesh)
                {
                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2(30, 0));
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::Text("Mesh Color");
                    bool isGreen = renderSettings_.greenMesh;
                    if (ImGui::RadioButton("Green", isGreen))
                        renderSettings_.greenMesh = true;
                    ImGui::SameLine();
                    if (ImGui::RadioButton("Black", !isGreen))
                        renderSettings_.greenMesh = false;
                    ImGui::EndGroup();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void Application::drawModelTreePanel()
    {
        ImGui::Begin("Model Tree");
        if (currentModel_.has_value())
        {
            ImGui::Text("Model loaded");
            ImGui::Text("Meshes: %zu", currentModel_.value().meshes.size());
            ImGui::Text("Vertices: %zu", currentModel_.value().getVertexCount());
            ImGui::Text("Triangles: %zu", currentModel_.value().getTriangleCount());
        }
        else
        {
            ImGui::Text("No model loaded");
        }
        ImGui::End();
    }

    void Application::drawModelViewPanel()
    {
        viewportPanel_.draw(renderer_, scene_, camera_, renderSettings_);
    }

}
