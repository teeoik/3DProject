#pragma once

#include "gfx/RenderTarget.h"
#include "gfx/Renderer.h"
#include "scene/CameraOrbit.h"
#include "scene/Scene.h"

namespace ui
{
    /**
     * @brief ImGui panel that hosts the 3D viewport.
     *
     * Renders into an off-screen RenderTarget and displays it in ImGui::Image.
     */
    class ViewportPanel final
    {
    public:
        /**
         * @brief Renders the viewport panel with scene content and handles camera input.
         *
         * Manages render target resizing, camera aspect ratio, input routing (orbit, pan, zoom),
         * and displays the rendered scene as an ImGui texture.
         *
         * @param renderer The graphics renderer to render the scene.
         * @param scene The scene to render.
         * @param camera The camera to use for rendering and receive input.
         */
        void draw(gfx::Renderer& renderer, scene::Scene& scene, scene::CameraOrbit& camera);

    private:
        gfx::RenderTarget target_;
    };
}
