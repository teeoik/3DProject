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
        ImGui::Begin("3D Project");
        ImGui::Text("Hello from ImGui.");
        ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
        ImGui::End();
    }
}
