#pragma once

#include <vector>

namespace gfx
{
    /**
     * @brief Simple 3D vector representation.
     */
    struct Vec3
    {
        float x, y, z;

        Vec3() = default;
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    };

    /**
     * @brief Represents a single mesh with vertices and indices.
     */
    struct Mesh
    {
        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
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
