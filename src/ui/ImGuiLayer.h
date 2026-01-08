#pragma once

struct GLFWwindow;

namespace ui
{
  class ImGuiLayer final
  {
  public:
    ImGuiLayer() = default;
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    void init(GLFWwindow* window, const char* glslVersion);
    void beginFrame();
    void render();   // calls ImGui::Render()
    void endFrame(); // renders ImGui draw data via OpenGL backend
    void shutdown();

  private:
    bool initialized_ = false;
  };
}
