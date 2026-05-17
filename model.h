#pragma once
#include <string>
#include <vector>
#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    explicit Model(const std::string& path);
    void Draw(Shader& shader);
private:
    std::vector<Mesh> meshes;
    std::string       directory;
    void loadModel   (const std::string& path);
    void processNode (aiNode* node, const aiScene* scene);
    Mesh processMesh (aiMesh* mesh, const aiScene* scene);
};
