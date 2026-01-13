#include "graphics/Mesh.h"

namespace graphics
{
    size_t Model::getTriangleCount() const
    {
        size_t count = 0;
        for (const auto& mesh : meshes)
            count += mesh.indices.size() / 3;
        return count;
    }

    size_t Model::getVertexCount() const
    {
        size_t count = 0;
        for (const auto& mesh : meshes)
            count += mesh.positions.size();
        return count;
    }
}
