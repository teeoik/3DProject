#pragma once

#include "platform/GlfwWindow.h"
#include "ui/ImGuiLayer.h"
#include "graphics/Mesh.h"

#include <imgui.h>
#include <optional>
#include <string>

namespace app
{
    /**
     * @brief Main application class managing window, rendering, and UI.
     *
     * Initializes the GLFW window, loads OpenGL via GLAD, and sets up ImGui
     * for UI rendering. Provides the main application loop that processes
     * events, renders the UI, and manages buffer swaps.
     *
     * @post On construction: window is created, OpenGL context is current, GLAD is loaded,
     *       and ImGui is initialized with OpenGL3 backend.
     * @throw std::runtime_error if GLAD initialization fails.
     */
    class Application final
    {
    public:
        /**
         * @brief Constructs the application and initializes all subsystems.
         *
         * Creates a 1280x720 window titled "3DProject", makes its OpenGL context current,
         * enables vsync, initializes GLAD for OpenGL function loading, and sets up ImGui
         * with GLFW and OpenGL3 backends.
         *
         * @post Window exists with active OpenGL context; GLAD and ImGui are initialized.
         * @throw std::runtime_error if GLAD initialization fails.
         */
        Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        /**
         * @brief Runs the main application loop.
         *
         * Continuously processes events, renders the UI, clears the framebuffer,
         * and presents frames until the window close flag is set. Returns after
         * the window is closed.
         *
         * @return 0 on successful exit.
         *
         * @pre Constructor must have completed successfully.
         */
        int run();

    private:
        /**
         * @brief Renders the application UI using ImGui.
         *
         * Sets up a docking-enabled UI with three panels: Toolbar (top 10%),
         * Model Tree (left 25%), and Model View (remaining space).
         * Initializes the docking layout on the first frame.
         *
         * @pre ImGui frame must be active (beginFrame() called in current frame).
         */
        void drawUi();

        /**
         * @brief Initializes the default docking layout on first frame.
         *
         * Creates a dockspace with three regions: top toolbar, left model tree,
         * and main model view area.
         *
         * @param dockspaceId ImGui dockspace identifier.
         * @param viewportSize Size of the main viewport.
         */
        void initializeDockingLayout(ImGuiID dockspaceId, ImVec2 viewportSize);

        /**
         * @brief Renders the toolbar panel.
         */
        void drawToolbarPanel();

        /**
         * @brief Renders the model tree panel.
         */
        void drawModelTreePanel();

        /**
         * @brief Renders the model view panel.
         */
        void drawModelViewPanel();

        platform::GlfwWindow window_;
        ui::ImGuiLayer imgui_;
        std::optional<std::string> selectedObjFile_;
        std::optional<graphics::Model> currentModel_;
    };
}
