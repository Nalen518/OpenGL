#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Shader; // forward decl

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    GLuint VAO = 0;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(Shader& shader);

private:
    GLuint VBO = 0, EBO = 0;
    void setupMesh();
};
