#include "gfx/ShaderLoader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace gfx
{
    std::string ShaderLoader::loadFromFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::filesystem::path absolutePath = std::filesystem::absolute(path);
            throw std::runtime_error("Failed to open shader file: " + absolutePath.string());
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
