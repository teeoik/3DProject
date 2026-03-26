#include "gfx/Renderer.h"
#include "gfx/Mesh.h"
#include "gfx/RenderTarget.h"
#include "gfx/ShaderLoader.h"
#include "gfx/ShaderProgram.h"
#include "scene/CameraOrbit.h"
#include "scene/Scene.h"

#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/fwd.hpp>

namespace gfx
{
    Renderer::~Renderer()
    {
        for (auto& [key, data] : meshCache_)
        {
            glDeleteBuffers(1, &data.vbo);
            glDeleteBuffers(1, &data.ebo);
            glDeleteVertexArrays(1, &data.vao);
        }
    }

    void Renderer::uploadMeshData(const Mesh& mesh, MeshRenderData& renderData)
    {
        glGenVertexArrays(1, &renderData.vao);
        glGenBuffers(1, &renderData.vbo);
        glGenBuffers(1, &renderData.ebo);

        glBindVertexArray(renderData.vao);

        glBindBuffer(GL_ARRAY_BUFFER, renderData.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.positions.size() * sizeof(Vec3), mesh.positions.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

        renderData.indexCount = static_cast<unsigned int>(mesh.indices.size());

        glBindVertexArray(0);
    }

    void Renderer::renderMesh(const MeshRenderData& renderData)
    {
        glBindVertexArray(renderData.vao);
        glDrawElements(GL_TRIANGLES, renderData.indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer::ensureShaderInitialized()
    {
        if (!shader_)
        {
            std::string vertexSource = ShaderLoader::loadFromFile("shaders/basic.vert");
            std::string fragmentSource = ShaderLoader::loadFromFile("shaders/basic.frag");
            shader_ = std::make_unique<ShaderProgram>(vertexSource, fragmentSource);
        }
    }

    void Renderer::render(const scene::Scene& scene, const scene::CameraOrbit& camera, const RenderTarget& target)
    {
        target.bind();

        glViewport(0, 0, target.width(), target.height());

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ensureShaderInitialized();
        shader_->use();

        glm::mat4 view = camera.viewMatrix();
        glm::mat4 projection = camera.projectionMatrix();

        shader_->setMat4("uView", view);
        shader_->setMat4("uProjection", projection);

        if (scene.model.has_value())
        {
            const Model& model = scene.model.value();
            for (std::size_t i = 0; i < model.meshes.size(); ++i)
            {
                const Mesh& mesh = model.meshes[i];
                MeshRenderData& renderData = meshCache_[i];

                if (renderData.vao == 0)
                    uploadMeshData(mesh, renderData);

                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                renderMesh(renderData);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        RenderTarget::unbind();
    }
}

