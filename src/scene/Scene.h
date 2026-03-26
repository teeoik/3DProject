#pragma once

#include "gfx/Mesh.h"

#include <optional>

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
    };
}
