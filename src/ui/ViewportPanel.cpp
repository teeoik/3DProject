#include "ui/ViewportPanel.h"
#include "gfx/Renderer.h"
#include "scene/CameraOrbit.h"
#include "scene/Scene.h"

#include <algorithm>
#include <cstdarg>

#include <imgui.h>

namespace ui
{
    void ViewportPanel::draw(gfx::Renderer& renderer, scene::Scene& scene, scene::CameraOrbit& camera, const gfx::RenderSettings& settings)
    {
        ImGui::Begin("Model View");

        const ImVec2 avail = ImGui::GetContentRegionAvail();
        const int w = std::max(1, static_cast<int>(avail.x));
        const int h = std::max(1, static_cast<int>(avail.y));

        if (target_.fbo() == 0)
            target_.create(w, h);
        else
            target_.resize(w, h);

        camera.setAspect(static_cast<float>(w) / static_cast<float>(h));

        // Only manipulate camera when viewport is hovered.
        const bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

        if (hovered)
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                const ImVec2 delta = ImGui::GetIO().MouseDelta;
                camera.onMouseDrag(delta.x, delta.y);
            }

            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
            {
                const ImVec2 delta = ImGui::GetIO().MouseDelta;
                camera.onPan(delta.x, -delta.y);
            }

            const float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.0f)
                camera.onMouseWheel(wheel);
        }

        renderer.render(scene, camera, target_, settings);

        // OpenGL textures have origin at bottom-left; ImGui assumes top-left. Flip UVs vertically.
        ImTextureID texId = (ImTextureID)(uintptr_t)target_.colorTexture();
        ImGui::Image(texId, avail, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
}
