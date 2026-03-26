#pragma once

#include <glm/glm.hpp>

namespace scene
{
    /**
     * @brief Orbit camera state with input handling.
     *
     * Manages yaw, pitch, distance, and pan parameters.
     * Provides view and projection matrix generation.
     */
    class CameraOrbit final
    {
    public:
        /**
         * @brief Sets the viewport aspect ratio.
         *
         * @param aspect Viewport aspect ratio (width / height).
         */
        void setAspect(float aspect) { aspect_ = aspect; }

        /**
         * @brief Handles mouse drag for camera orbit.
         *
         * @param dx Horizontal mouse delta (pixels).
         * @param dy Vertical mouse delta (pixels).
         */
        void onMouseDrag(float dx, float dy);

        /**
         * @brief Handles mouse wheel for zoom.
         *
         * @param delta Wheel delta (typically ±1).
         */
        void onMouseWheel(float delta);

        /**
         * @brief Handles middle mouse drag for camera pan.
         *
         * @param dx Horizontal pan delta (pixels).
         * @param dy Vertical pan delta (pixels).
         */
        void onPan(float dx, float dy);

        /**
         * @brief Generates the view matrix from current camera state.
         *
         * @return View matrix for rendering.
         */
        glm::mat4 viewMatrix() const;

        /**
         * @brief Generates the projection matrix.
         *
         * @return Perspective projection matrix.
         */
        glm::mat4 projectionMatrix() const;

        float aspect() const { return aspect_; }

    private:
        float aspect_ = 16.0f / 9.0f;

        float yaw_ = 0.0f;
        float pitch_ = 0.0f;
        float distance_ = 3.0f;

        float panX_ = 0.0f;
        float panY_ = 0.0f;
    };
}
