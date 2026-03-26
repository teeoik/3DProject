#pragma once

#include <glm/ext/vector_float3.hpp>

namespace gfx
{
    /**
     * @brief Rendering configuration that controls visual output.
     * 
     * Stores background color, draw modes, and other rendering options.
     * Designed for easy serialization in future versions.
     */
    struct RenderSettings
    {
        enum class BackgroundType
        {
            SolidColor,
            Gradient,
            Checkered
        };

        enum class DrawMode
        {
            Mesh
        };

        BackgroundType backgroundType = BackgroundType::SolidColor;
        bool lightBackground = true;
        bool greenMesh = false;
        DrawMode drawMode = DrawMode::Mesh;

        glm::vec3 getBackgroundColor() const
        {
            return lightBackground ? glm::vec3(0.8f, 0.8f, 0.8f) : glm::vec3(0.2f, 0.2f, 0.2f);
        }

        glm::vec3 getMeshColor() const
        {
            return greenMesh ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f);
        }

        /**
         * @brief Returns default settings.
         * 
         * Centralized location for configuring application defaults.
         */
        static RenderSettings defaultSettings()
        {
            RenderSettings settings;
            settings.backgroundType = BackgroundType::SolidColor;
            settings.lightBackground = true;
            settings.greenMesh = false;
            settings.drawMode = DrawMode::Mesh;
            return settings;
        }
    };
}
