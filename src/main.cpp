#include <cstdio>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char* description)
{
  std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit())
    return 1;

  // Request an OpenGL 3.3 Core context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(1280, 720, "3DProject", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // vsync

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  // Setup Dear ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  // Initialize ImGui backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Your UI
    ImGui::Begin("3D Project");
    ImGui::Text("Hello from ImGui.");
    ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
    ImGui::End();

    // Rendering
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
