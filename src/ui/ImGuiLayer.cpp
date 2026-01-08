#include "ui/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace ui
{
  ImGuiLayer::~ImGuiLayer()
  {
    shutdown();
  }

  void ImGuiLayer::init(GLFWwindow* window, const char* glslVersion)
  {
    if (initialized_)
      return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

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
}
