#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    GLuint vert = compileShader(readFile(vertexPath), GL_VERTEX_SHADER);
    GLuint frag = compileShader(readFile(fragmentPath), GL_FRAGMENT_SHADER);
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);
    int ok; char log[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std::cerr << "LINK ERROR:\n" << log << "\n";
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
}

std::string Shader::readFile(const char* path) {
    std::ifstream f(path);
    if (!f) {
        std::cerr << "Cannot open shader: " << path << "\n";
        return "";
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(const std::string& src, GLenum type) {
    GLuint s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    int ok; char log[512];
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(s, 512, nullptr, log);
        std::cerr << "COMPILE ERROR:\n" << log << "\n";
    }
    return s;
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string& n, bool  v) const {
    glUniform1i(glGetUniformLocation(ID, n.c_str()), (int)v);
}
void Shader::setInt(const std::string& n, int   v) const {
    glUniform1i(glGetUniformLocation(ID, n.c_str()), v);
}
void Shader::setFloat(const std::string& n, float v) const {
    glUniform1f(glGetUniformLocation(ID, n.c_str()), v);
}
void Shader::setVec3(const std::string& n, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(ID, n.c_str()), 1, glm::value_ptr(v));
}
void Shader::setMat3(const std::string& n, const glm::mat3& m) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}
void Shader::setMat4(const std::string& n, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}