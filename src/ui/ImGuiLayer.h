#pragma once

#include <string_view>

struct GLFWwindow;

namespace ui
{
    /**
     * @brief ImGui integration layer for rendering UI with Dear ImGui.
     *
     * @note ImGui context is created on first init() call and destroyed on shutdown().
     */
    class ImGuiLayer final
    {
    public:
        /**
         * @brief Constructs an uninitialized ImGui layer.
         *
         * The layer must be initialized via init() before use.
         */
        ImGuiLayer() = default;

        /**
         * @brief Destroys the ImGui layer and cleans up resources.
         *
         * Calls shutdown() to ensure ImGui context and backends are properly cleaned up.
         */
        ~ImGuiLayer();

        ImGuiLayer(const ImGuiLayer&) = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;

        /**
         * @brief Initializes the ImGui context and platform/renderer backends.
         *
         * Sets up ImGui with GLFW platform backend and OpenGL3 renderer backend.
         * Applies dark color scheme by default. Multiple calls are safe (subsequent
         * calls are ignored if already initialized).
         *
         * @param window Valid GLFW window handle.
         * @param glslVersion GLSL version string (e.g., "#version 330"), determines shader compatibility.
         *
         * @pre window must be a valid, non-null GLFWwindow pointer.
         * @post ImGui context is created; GLFW and OpenGL3 backends are initialized.
         *
         * @throw May throw if ImGui backend initialization fails (implementation dependent).
         */
        void init(GLFWwindow* window, std::string_view glslVersion);

        /**
         * @brief Starts a new ImGui frame.
         *
         * Must be called once per frame before any ImGui UI code.
         * Advances input handling and prepares the ImGui context for drawing.
         *
         * @pre init() must have been called successfully.
         */
        void beginFrame();

        /**
         * @brief Renders ImGui draw data into vertex buffers.
         *
         * Calls ImGui::Render() to finalize the draw list for the current frame.
         * Should be called after all ImGui UI code but before endFrame().
         *
         * @pre beginFrame() must have been called in the current frame.
         */
        void render();

        /**
         * @brief Renders ImGui draw data to the OpenGL framebuffer.
         *
         * Executes the OpenGL3 backend rendering, submitting ImGui draw data to the GPU.
         * Should be called after render() to present the UI.
         *
         * @pre render() must have been called in the current frame.
         */
        void endFrame();

        /**
         * @brief Shuts down the ImGui context and backends.
         *
         * Cleans up ImGui context and detaches platform/renderer backends.
         * Safe to call multiple times (subsequent calls are ignored if not initialized).
         *
         * @post ImGui context and backends are destroyed; layer returns to uninitialized state.
         */
        void shutdown();

    private:
        bool initialized_ = false;
    };
}
