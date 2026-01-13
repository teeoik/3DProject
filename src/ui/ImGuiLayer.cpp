#include "ui/ImGuiLayer.h"

#include <optional>
#include <string>
#include <string_view>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ui
{
#ifdef _WIN32
    constexpr size_t MAX_FILE_PATH = MAX_PATH;
#else
    constexpr size_t MAX_FILE_PATH = 260;
#endif

    ImGuiLayer::~ImGuiLayer()
    {
        shutdown();
    }

    void ImGuiLayer::init(GLFWwindow* window, std::string_view glslVersion)
    {
        if (initialized_)
            return;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glslVersion.data());

        initialized_ = true;
    }

    void ImGuiLayer::beginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::render()
    {
        ImGui::Render();
    }

    void ImGuiLayer::endFrame()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::shutdown()
    {
        if (!initialized_)
            return;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        initialized_ = false;
    }

    std::optional<std::string> ImGuiLayer::openFileDialog()
    {
#ifdef _WIN32
        OPENFILENAMEA ofn = {};
        char szFile[MAX_FILE_PATH] = {};

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = nullptr;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
            return std::string(szFile);

        return std::nullopt;
#else
        return std::nullopt;
#endif
    }
}
