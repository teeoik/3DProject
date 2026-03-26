#pragma once

#include "gfx/Mesh.h"
#include "gfx/RenderTarget.h"
#include "gfx/ShaderProgram.h"
#include "scene/CameraOrbit.h"
#include "scene/Scene.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace gfx
{
    /**
     * @brief Mesh rendering data (VAO/VBO).
     *
     * Stores GPU-side representation of mesh data.
     */
    struct MeshRenderData
    {
        std::uint32_t vao = 0;  // Vertex Array Object
        std::uint32_t vbo = 0;  // Vertex Buffer Object
        std::uint32_t ebo = 0;  // Element Buffer Object
        unsigned int indexCount = 0;
    };

    /**
     * @brief Renders scenes to off-screen render targets.
     *
     * Manages mesh GPU resources (VAO/VBO) and renders loaded models
     * with basic visualization. Caches mesh data to avoid re-uploads.
     *
     * Later: draw grid, axes, lighting, etc.
     */
    class Renderer final
    {
    public:
        /**
         * @brief Destroys the renderer and releases all cached GPU resources.
         *
         * Deallocates VAOs, VBOs, and EBOs for all cached meshes.
         */
        ~Renderer();

        /**
         * @brief Renders the scene to the given render target.
         *
         * Sets up viewport and OpenGL state, clears the target, and prepares for scene rendering.
         *
         * @param scene The scene to render.
         * @param camera The camera providing view/projection information.
         * @param target The render target to draw to.
         */
        void render(const scene::Scene& scene, const scene::CameraOrbit& camera, const RenderTarget& target);

    private:
        void uploadMeshData(const Mesh& mesh, MeshRenderData& renderData);
        void renderMesh(const MeshRenderData& renderData);
        void ensureShaderInitialized();

        std::unordered_map<std::size_t, MeshRenderData> meshCache_;
        std::unique_ptr<ShaderProgram> shader_;
    };
}

