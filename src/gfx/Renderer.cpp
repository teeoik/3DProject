#include "gfx/Renderer.h"
#include "gfx/Mesh.h"
#include "gfx/RenderSettings.h"
#include "gfx/RenderTarget.h"
#include "gfx/ShaderLoader.h"
#include "gfx/ShaderProgram.h"
#include "scene/CameraOrbit.h"
#include "scene/Scene.h"

#include <exception>
#include <iostream>
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

        if (backgroundVao_ != 0)
        {
            glDeleteVertexArrays(1, &backgroundVao_);
            glDeleteBuffers(1, &backgroundVbo_);
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

    void Renderer::ensureBackgroundInitialized()
    {
        if (backgroundVao_ != 0)
            return;

        float vertices[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f,  1.0f
        };

        glGenVertexArrays(1, &backgroundVao_);
        glGenBuffers(1, &backgroundVbo_);

        glBindVertexArray(backgroundVao_);
        glBindBuffer(GL_ARRAY_BUFFER, backgroundVbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        try
        {
            std::string vertexSource = ShaderLoader::loadFromFile("shaders/background.vert");
            std::string gradientFragmentSource = ShaderLoader::loadFromFile("shaders/background_gradient.frag");
            std::string checkeredFragmentSource = ShaderLoader::loadFromFile("shaders/background_checkered.frag");

            backgroundShader_ = std::make_unique<ShaderProgram>(vertexSource, gradientFragmentSource);
            checkeredShader_ = std::make_unique<ShaderProgram>(vertexSource, checkeredFragmentSource);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to load background shaders: " << e.what() << std::endl;
            std::cerr << "Current working directory might be incorrect. Ensure shader files are in 'shaders/' directory." << std::endl;
        }
    }

    void Renderer::applyBackground(const RenderSettings& settings, int width, int height)
    {
        const auto color = settings.getBackgroundColor();

        switch (settings.backgroundType)
        {
        case RenderSettings::BackgroundType::SolidColor:
            glClearColor(color.r, color.g, color.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;

        case RenderSettings::BackgroundType::Gradient:
        {
            glClearColor(color.r, color.g, color.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ensureBackgroundInitialized();

            if (!backgroundShader_)
            {
                break;
            }

            glDisable(GL_DEPTH_TEST);
            backgroundShader_->use();
            backgroundShader_->setVec3("uColor", color);

            glBindVertexArray(backgroundVao_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            glEnable(GL_DEPTH_TEST);
            break;
        }

        case RenderSettings::BackgroundType::Checkered:
        {
            glClearColor(color.r, color.g, color.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ensureBackgroundInitialized();

            if (!checkeredShader_)
            {
                break;
            }

            glDisable(GL_DEPTH_TEST);
            checkeredShader_->use();
            checkeredShader_->setVec3("uColor", color);
            checkeredShader_->setVec2("uResolution", glm::vec2(static_cast<float>(width), static_cast<float>(height)));

            glBindVertexArray(backgroundVao_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            glEnable(GL_DEPTH_TEST);
            break;
        }
        }
    }

    void Renderer::render(const scene::Scene& scene, const scene::CameraOrbit& camera, const RenderTarget& target, const RenderSettings& settings)
    {
        target.bind();

        glViewport(0, 0, target.width(), target.height());

        glEnable(GL_DEPTH_TEST);

        applyBackground(settings, target.width(), target.height());

        ensureShaderInitialized();
        shader_->use();

        glm::mat4 view = camera.viewMatrix();
        glm::mat4 projection = camera.projectionMatrix();

        shader_->setMat4("uView", view);
        shader_->setMat4("uProjection", projection);
        shader_->setVec3("uMeshColor", settings.getMeshColor());

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

