#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../commonFns.h"

#include "Mesh.h"

// Utility function for texture loading
unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model {
public:
    // Constructor: Load model from path
    Model(const std::string& path);

    // Draw the model
    void Draw(unsigned int shaderID);

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures; // Store loaded textures to prevent duplicates
    std::string directory;

    // Load model with Assimp
    void loadModel(const std::string& path);

    // Process Assimp nodes recursively
    void processNode(aiNode* node, const aiScene* scene);

    // Process a single Assimp mesh
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // Load textures from a material
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

#endif
