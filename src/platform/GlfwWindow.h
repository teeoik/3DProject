#pragma once

#include <utility>

struct GLFWwindow;

namespace platform
{
  class GlfwWindow final
  {
  public:
    GlfwWindow(int width, int height, const char* title);
    ~GlfwWindow();

    GlfwWindow(const GlfwWindow&) = delete;
    GlfwWindow& operator=(const GlfwWindow&) = delete;

    GlfwWindow(GlfwWindow&&) = delete;
    GlfwWindow& operator=(GlfwWindow&&) = delete;

    void makeContextCurrent();
    void setVsync(bool enabled);

    void pollEvents();
    void swapBuffers();

    bool shouldClose() const;
    std::pair<int, int> framebufferSize() const;

    GLFWwindow* nativeHandle() const { return window_; }

  private:
    static void errorCallback(int error, const char* description);

    GLFWwindow* window_ = nullptr;
  };
}
