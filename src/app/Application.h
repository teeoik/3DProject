#pragma once

#include "platform/GlfwWindow.h"
#include "ui/ImGuiLayer.h"

namespace app
{
  class Application final
  {
  public:
    Application();
    int run();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

  private:
    void drawUi();

    platform::GlfwWindow window_;
    ui::ImGuiLayer imgui_;
  };
}
