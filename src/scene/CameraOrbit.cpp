#include "scene/CameraOrbit.h"

#include <algorithm>
#include <cmath>

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{
    void CameraOrbit::onMouseDrag(float dx, float dy)
    {
        yaw_ += dx * 0.01f;
        pitch_ += dy * 0.01f;

        pitch_ = std::clamp(pitch_, -1.55f, 1.55f); // Clamp pitch to prevent gimbal lock
    }

    void CameraOrbit::onMouseWheel(float delta)
    {
        distance_ *= (1.0f - delta * 0.1f);
        distance_ = std::clamp(distance_, 0.2f, 100.0f);
    }

    void CameraOrbit::onPan(float dx, float dy)
    {
        panX_ += dx * 0.01f;
        panY_ += dy * 0.01f;
    }

    glm::mat4 CameraOrbit::viewMatrix() const
    {
        // Calculate camera position in spherical coordinates
        float x = std::cos(pitch_) * std::sin(yaw_) * distance_ + panX_;
        float y = std::sin(pitch_) * distance_ + panY_;
        float z = std::cos(pitch_) * std::cos(yaw_) * distance_;

        // Look at origin
        return glm::lookAt(glm::vec3(x, y, z), glm::vec3(panX_, panY_, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 CameraOrbit::projectionMatrix() const
    {
        // 60 degree FOV, near plane 0.1, far plane 1000
        return glm::perspective(glm::radians(60.0f), aspect_, 0.1f, 1000.0f);
    }
}
