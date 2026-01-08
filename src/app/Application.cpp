#include "app/Application.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

namespace app
{
  Application::Application()
    : window_(1280, 720, "3DProject")
  {
    // Window must exist and have a current context before GLAD can load.
    window_.makeContextCurrent();
    window_.setVsync(true);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
      throw std::runtime_error("Failed to initialize GLAD");
    }

    // ImGui setup (GLFW + OpenGL3 backends)
    imgui_.init(window_.nativeHandle(), "#version 330");
  }

  int Application::run()
  {
    while (!window_.shouldClose())
    {
      window_.pollEvents();

      imgui_.beginFrame();

      drawUi();

      // Render
      imgui_.render(); // ImGui::Render() - generates draw data

      const auto [fbw, fbh] = window_.framebufferSize();
      glViewport(0, 0, fbw, fbh);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      imgui_.endFrame(); // renders ImGui draw data via OpenGL3 backend

      window_.swapBuffers();
    }

    return 0;
  }

  void Application::drawUi()
  {
    ImGui::Begin("3D Project");
    ImGui::Text("Hello from ImGui.");
    ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
    ImGui::End();
  }
}
