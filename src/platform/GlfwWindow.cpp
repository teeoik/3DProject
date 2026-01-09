#include "platform/GlfwWindow.h"

#include <cstdio>
#include <utility>
#include <stdexcept>
#include <string_view>

#include <GLFW/glfw3.h>

namespace platform
{
    void GlfwWindow::errorCallback(int error, const char* description)
    {
        std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    GlfwWindow::GlfwWindow(int width, int height, std::string_view title)
    {
        glfwSetErrorCallback(GlfwWindow::errorCallback);

        if (!glfwInit())
            throw std::runtime_error("glfwInit() failed");

        // Uses OpenGL 3.3 veersion
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        window_ = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (!window_)
        {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow() failed");
        }
    }

    GlfwWindow::~GlfwWindow()
    {
        if (window_)
        {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }
        glfwTerminate();
    }

    void GlfwWindow::makeContextCurrent()
    {
        glfwMakeContextCurrent(window_);
    }

    void GlfwWindow::setVsync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void GlfwWindow::pollEvents()
    {
        glfwPollEvents();
    }

    void GlfwWindow::swapBuffers()
    {
        glfwSwapBuffers(window_);
    }

    bool GlfwWindow::shouldClose() const
    {
        return glfwWindowShouldClose(window_) != 0;
    }

    std::pair<int, int> GlfwWindow::framebufferSize() const
    {
        int w = 0, h = 0;
        glfwGetFramebufferSize(window_, &w, &h);
        return { w, h };
    }
}
