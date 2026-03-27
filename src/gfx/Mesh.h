#pragma once

#include <vector>

#include <glm/ext/vector_float3.hpp>

namespace gfx
{
    /**
     * @brief Represents a single mesh with vertices and indices.
     */
    struct Mesh
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
    };

    /**
     * @brief Represents a loaded 3D model containing one or more meshes.
     */
    struct Model
    {
        std::vector<Mesh> meshes;

        /**
         * @brief Returns the total number of triangles in all meshes.
         */
        size_t getTriangleCount() const;

        /**
         * @brief Returns the total number of vertices in all meshes.
         */
        size_t getVertexCount() const;
    };
}
