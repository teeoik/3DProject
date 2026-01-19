#include "app/Application.h"
#include "gfx/ModelLoader.h"

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

            // Render ImGui draw lists
            imgui_.render();

            // Clear and set viewport for scene rendering
            const auto [fbw, fbh] = window_.framebufferSize();
            glViewport(0, 0, fbw, fbh);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Submit ImGui draw data to OpenGL backend and swap buffers
            imgui_.endFrame();

            window_.swapBuffers();
        }

        return 0;
    }

    void Application::drawUi()
    {
        static bool firstFrame = true;

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

        if (firstFrame)
        {
            firstFrame = false;
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

        ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Up, 0.10f, &dockTop, &dockMain);
        ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.25f, &dockLeft, &dockMain);

        ImGui::DockBuilderDockWindow("Toolbar", dockTop);
        ImGui::DockBuilderDockWindow("Model Tree", dockLeft);
        ImGui::DockBuilderDockWindow("Model View", dockMain);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    void Application::drawToolbarPanel()
    {
        ImGui::Begin("Toolbar");

        if (ImGui::Button("Open OBJ", ImVec2(100, 0)))
        {
            auto filePath = imgui_.openFileDialog();
            if (filePath.has_value())
            {
                selectedObjFile_ = filePath.value();
                try
                {
                    currentModel_ = gfx::ModelLoader::load(selectedObjFile_.value());
                }
                catch (const gfx::ModelLoadException& e)
                {
                    std::cerr << "Error loading model: " << e.what() << std::endl;
                    currentModel_ = std::nullopt;
                }
            }
        }

        ImGui::SameLine();
        if (selectedObjFile_.has_value())
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "File: %s", selectedObjFile_.value().c_str());

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
        ImGui::Begin("Model View");
        if (currentModel_.has_value())
            ImGui::Text("Model is ready for rendering");
        else
            ImGui::Text("Model View");
        ImGui::End();
    }

}
