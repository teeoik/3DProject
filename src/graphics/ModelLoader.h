#pragma once

#include "graphics/Mesh.h"

#include <string_view>
#include <stdexcept>

namespace graphics
{
    /**
     * @brief Exception thrown when model loading fails.
     */
    class ModelLoadException : public std::runtime_error
    {
    public:
        explicit ModelLoadException(const std::string& message)
            : std::runtime_error(message) {}
    };

    /**
     * @brief Loads 3D models from files using assimp.
     */
    class ModelLoader final
    {
    public:
        /**
         * @brief Loads a model from the specified file path.
         *
         * Uses assimp to load the model, extracting vertex positions, normals,
         * and face indices from the scene. Supports OBJ and other formats
         * supported by assimp.
         *
         * @param filePath Path to the model file to load.
         * @return Loaded Model with mesh data.
         * @throw ModelLoadException if loading or parsing fails.
         */
        static Model load(std::string_view filePath);
    };
}
