#include "gfx/ShaderProgram.h"

#include <stdexcept>
#include <string>
#include <string_view>

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gfx
{
    ShaderProgram::ShaderProgram(std::string_view vertexSource, std::string_view fragmentSource)
    {
        std::uint32_t vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
        std::uint32_t fragment = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

        program_ = glCreateProgram();
        glAttachShader(program_, vertex);
        glAttachShader(program_, fragment);
        glLinkProgram(program_);

        int success;
        glGetProgramiv(program_, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program_, 512, nullptr, infoLog);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(program_);
            program_ = 0;
            throw std::runtime_error(std::string("Shader program linking failed: ") + infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ShaderProgram::~ShaderProgram()
    {
        if (program_ != 0)
            glDeleteProgram(program_);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(program_);
    }

    void ShaderProgram::setMat4(std::string_view name, const glm::mat4& matrix) const
    {
        int location = glGetUniformLocation(program_, name.data());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::setVec3(std::string_view name, const glm::vec3& value) const
    {
        int location = glGetUniformLocation(program_, name.data());
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void ShaderProgram::setVec2(std::string_view name, const glm::vec2& value) const
    {
        int location = glGetUniformLocation(program_, name.data());
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    std::uint32_t ShaderProgram::compileShader(std::string_view source, std::uint32_t type)
    {
        std::uint32_t shader = glCreateShader(type);
        const char* src = source.data();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            glDeleteShader(shader);
            throw std::runtime_error(std::string("Shader compilation failed: ") + infoLog);
        }

        return shader;
    }
}
