#pragma once

#include <utility>
#include <string_view>

struct GLFWwindow;

namespace platform
{
    /**
     * @brief GLFW window wrapper providing OpenGL context management.
     */
    class GlfwWindow final
    {
    public:
        /**
         * @brief Constructs and creates a GLFW window.
         *
         * Initializes GLFW, configures defined OpenGL version and core profile context,
         * and creates a window with the specified dimensions and title.
         * In debug builds, enables OpenGL debug context.
         *
         * @param width Window width in pixels.
         * @param height Window height in pixels.
         * @param title Window title string
         *
         * @throw std::runtime_error if glfwInit() fails.
         * @throw std::runtime_error if glfwCreateWindow() fails.
         */
        GlfwWindow(int width, int height, std::string_view title);

        /**
         * @brief Destroys the window and terminates GLFW.
         *
         * Cleans up the native GLFW window handle and terminates the GLFW library.
         */
        ~GlfwWindow();

        GlfwWindow(const GlfwWindow&) = delete;
        GlfwWindow& operator=(const GlfwWindow&) = delete;

        GlfwWindow(GlfwWindow&&) = delete;
        GlfwWindow& operator=(GlfwWindow&&) = delete;

        /**
         * @brief Makes this window's OpenGL context current on the calling thread.
         *
         * @pre Must be called on the thread where the window was created.
         */
        void makeContextCurrent();

        /**
         * @brief Enables or disables vertical sync (vsync).
         *
         * @param enabled true to enable vsync (swap interval = 1), false to disable (swap interval = 0).
         */
        void setVsync(bool enabled);

        /**
         * @brief Processes pending window events.
         *
         * Polls the event queue for user input, window events, and other system events.
         */
        void pollEvents();

        /**
         * @brief Swaps the front and back buffers.
         *
         * Presents the rendered frame to the window. Should be called after rendering
         * and before the next frame.
         */
        void swapBuffers();

        /**
         * @brief Checks if the window should close.
         *
         * @return true if the user has requested the window to close or close flag is set, false otherwise.
         */
        bool shouldClose() const;

        /**
         * @brief Retrieves the framebuffer dimensions.
         *
         * @return A pair containing the width and height of the framebuffer in pixels.
         */
        std::pair<int, int> framebufferSize() const;

        /**
         * @brief Returns the native GLFW window handle.
         *
         * @return Pointer to the underlying GLFWwindow structure.
         */
        GLFWwindow* nativeHandle() const { return window_; }

    private:
        static void errorCallback(int error, const char* description);

        GLFWwindow* window_ = nullptr;
    };
}
