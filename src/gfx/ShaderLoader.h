#pragma once

#include <filesystem>
#include <string>

namespace gfx
{
    /**
     * @brief Utility for loading shader source code from files.
     */
    class ShaderLoader final
    {
    public:
        /**
         * @brief Loads shader source code from a file.
         *
         * @param path Path to the shader file.
         * @return Shader source code as a string.
         * @throw std::runtime_error if file cannot be opened or read.
         */
        static std::string loadFromFile(const std::filesystem::path& path);
    };
}
