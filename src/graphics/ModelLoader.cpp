#include "graphics/ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <sstream>

namespace graphics
{
    Model ModelLoader::load(std::string_view filePath)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            filePath.data(),
            aiProcess_Triangulate |           // Convert polygons to triangles
            aiProcess_GenNormals |            // Generate normals if missing
            aiProcess_FlipWindingOrder        // Ensure correct winding order
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::ostringstream oss;
            oss << "Failed to load model '" << filePath << "': "
                << importer.GetErrorString();
            throw ModelLoadException(oss.str());
        }

        Model model;
        model.meshes.reserve(scene->mNumMeshes);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            const aiMesh* aiMesh = scene->mMeshes[i];
            Mesh mesh;

            // Extract vertex positions
            mesh.positions.reserve(aiMesh->mNumVertices);
            for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
            {
                const auto& pos = aiMesh->mVertices[j];
                mesh.positions.emplace_back(pos.x, pos.y, pos.z);
            }

            // Extract normals if available
            if (aiMesh->HasNormals())
            {
                mesh.normals.reserve(aiMesh->mNumVertices);
                for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
                {
                    const auto& normal = aiMesh->mNormals[j];
                    mesh.normals.emplace_back(normal.x, normal.y, normal.z);
                }
            }

            // Extract face indices
            for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j)
            {
                const aiFace& face = aiMesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; ++k)
                    mesh.indices.push_back(face.mIndices[k]);
            }

            model.meshes.push_back(mesh);
        }

        return model;
    }
}
