#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string_view>

namespace gfx
{
    /**
     * @brief Manages GLSL shader program compilation and uniform setting.
     *
     * Compiles vertex and fragment shaders and provides methods to set uniforms.
     */
    class ShaderProgram final
    {
    public:
        /**
         * @brief Compiles and links vertex and fragment shaders.
         *
         * @param vertexSource Vertex shader source code.
         * @param fragmentSource Fragment shader source code.
         * @throw std::runtime_error if compilation or linking fails.
         */
        ShaderProgram(std::string_view vertexSource, std::string_view fragmentSource);

        /**
         * @brief Destroys the shader program and releases GPU resources.
         */
        ~ShaderProgram();

        /**
         * @brief Activates this shader program for rendering.
         */
        void use() const;

        /**
         * @brief Sets a 4x4 matrix uniform.
         *
         * @param name Uniform variable name.
         * @param matrix The matrix value.
         */
        void setMat4(std::string_view name, const glm::mat4& matrix) const;

    private:
        std::uint32_t program_ = 0;

        std::uint32_t compileShader(std::string_view source, std::uint32_t type);
    };
}
