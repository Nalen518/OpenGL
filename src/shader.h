#pragma once
#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;

    void setBool(const std::string& name, bool             value) const;
    void setInt(const std::string& name, int              value) const;
    void setFloat(const std::string& name, float            value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat3(const std::string& name, const glm::mat3& mat)   const; // ← ADDED
    void setMat4(const std::string& name, const glm::mat4& mat)   const;

private:
    std::string readFile(const char* path);
    GLuint      compileShader(const std::string& src, GLenum type);
};