#pragma once

#include "gfx/Mesh.h"

#include <optional>

#include <glm/mat4x4.hpp>

namespace scene
{
    /**
     * @brief Minimal scene placeholder.
     *
     * Later: store loaded model, grid toggle, lighting, etc.
     */
    class Scene final
    {
    public:
        bool showGrid = true;
        bool showAxes = true;
        std::optional<gfx::Model> model;
        glm::mat4 modelTransform = glm::mat4(1.0f);
    };
}
