#include "gfx/ShaderLoader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace gfx
{
    static std::filesystem::path getExecutableDirectory()
    {
#ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::filesystem::path exePath(buffer);
        return exePath.parent_path();
#else
        return std::filesystem::current_path();
#endif
    }

    std::string ShaderLoader::loadFromFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::filesystem::path exeDir = getExecutableDirectory();
            std::filesystem::path absolutePath = exeDir / path;

            file.open(absolutePath);
            if (!file.is_open())
            {
                throw std::runtime_error("Failed to open shader file: " + absolutePath.string() + 
                                       " (also tried: " + std::filesystem::absolute(path).string() + ")");
            }
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
