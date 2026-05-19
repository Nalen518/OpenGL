#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

// Suppress C4251: Assimp DLL-interface warnings (safe to ignore when using prebuilt DLL)
#pragma warning(disable : 4251)

Model::Model(const std::string& path) { loadModel(path); }

void Model::Draw(Shader& shader) {
    for (auto& m : meshes) m.Draw(shader);
}

void Model::loadModel(const std::string& path) {
    // Resolve absolute path
    std::filesystem::path fsPath = std::filesystem::absolute(path);
    std::string absPath = fsPath.string();
    std::cout << "[Model] Loading: " << absPath << "\n";

    if (!std::filesystem::exists(fsPath)) {
        std::cerr << "[ERROR] Not found: " << absPath << "\n";
        return;
    }

    // Read OBJ file ourselves (std::ifstream handles spaces in paths correctly)
    // then strip mtllib/usemtl lines so Assimp never touches the MTL file
    // (we set our own Phong materials via shader uniforms anyway)
    std::ifstream file(fsPath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Cannot open: " << absPath << "\n";
        return;
    }

    std::string filtered, line;
    while (std::getline(file, line)) {
        // Skip material library and usemtl directives
        if (line.rfind("mtllib", 0) == 0) continue;
        if (line.rfind("usemtl", 0) == 0) continue;
        filtered += line + "\n";
    }
    file.close();
    std::cout << "[Model] File read (" << filtered.size() << " bytes after stripping mtl)\n";

    // Feed filtered content to Assimp via memory buffer
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(
        filtered.c_str(),
        filtered.size(),
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices,
        ".obj"   // hint Assimp this is an OBJ file
    );

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        std::cerr << "[ASSIMP ERROR]: " << importer.GetErrorString() << "\n";
        return;
    }
    std::cout << "[Model] Loaded " << scene->mNumMeshes << " mesh(es) OK\n";
    directory = absPath.substr(0, absPath.find_last_of("/\\"));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.Position  = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        v.Normal    = mesh->HasNormals()
                    ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                    : glm::vec3(0.f, 1.f, 0.f);
        v.TexCoords = mesh->mTextureCoords[0]
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0.f, 0.f);
        vertices.push_back(v);
    }

    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}
