// ============================================================
//  OpenGL Assignment – Phong Shading + OBJ Loader + MVP Configs
//  Controls:  1/2/3 – switch MVP config
//             F     – cycle shader mode
//             W     – toggle wireframe
//             ESC   – quit
// ============================================================
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "shader.h"
#include "model.h"

// ── Window ───────────────────────────────────────────────────
const unsigned int SCR_W = 1280, SCR_H = 720;

// ── State ────────────────────────────────────────────────────
int  gConfig = 1;
int  gShaderMode = 0;
bool gWireframe = false;

// ── Callbacks ────────────────────────────────────────────────
void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow* win, int key, int, int action, int) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(win, true);

        if (key == GLFW_KEY_1) {
            gConfig = 1;
            std::cout << "Config 1: Perspective front view\n";
        }
        if (key == GLFW_KEY_2) {
            gConfig = 2;
            std::cout << "Config 2: Low angle dramatic view\n";
        }
        if (key == GLFW_KEY_3) {
            gConfig = 3;
            std::cout << "Config 3: Orthographic side view\n";
        }
        if (key == GLFW_KEY_F) {
            gShaderMode = (gShaderMode + 1) % 4;
            std::string modeNames[] = {
                "Standard Phong",
                "Phong + Attenuation",
                "Phong + Attenuation + Rim Light",
                "Phong + Attenuation + Rim + Emission"
            };
            std::cout << "Shader Mode: " << modeNames[gShaderMode] << "\n";
        }
        if (key == GLFW_KEY_W) {
            gWireframe = !gWireframe;
            glPolygonMode(GL_FRONT_AND_BACK, gWireframe ? GL_LINE : GL_FILL);
            std::cout << "Wireframe: " << (gWireframe ? "ON" : "OFF") << "\n";
        }
    }
}

// ── Material struct ──────────────────────────────────────────
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float     shininess;
    glm::vec3 emission;
};

void applyMaterial(Shader& s, const Material& m) {
    s.setVec3("material.ambient", m.ambient);
    s.setVec3("material.diffuse", m.diffuse);
    s.setVec3("material.specular", m.specular);
    s.setFloat("material.shininess", m.shininess);
    s.setVec3("material.emission", m.emission);
}

// ── Entry point ───────────────────────────────────────────────
int main() {
    // ---- GLFW init ----------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H,
        "OpenGL Assignment - Phong Shading  [1/2/3] Config  [F] Shader  [W] Wire  [ESC] Quit",
        nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // ---- GLAD init ----------------------------------------
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << "\n";
    std::cout << "Controls:\n";
    std::cout << "  [1][2][3] = MVP config\n";
    std::cout << "  [F]       = cycle shader mode\n";
    std::cout << "  [W]       = wireframe toggle\n";
    std::cout << "  [ESC]     = quit\n";

    // ---- Shaders ------------------------------------------
    Shader phong("shaders/phong.vert", "shaders/phong.frag");

    // ---- Model --------------------------------------------
    const std::string MODEL_PATH = "models/Water.obj";
    std::cout << "[Model] Loading: " << MODEL_PATH << "\n";
    Model ourModel(MODEL_PATH);
    std::cout << "[Model] Loaded successfully\n";

    // ---- Materials (Item 2) --------------------------------
    // Gold — warm, very shiny, high specular
    Material matGold = {
        {0.25f, 0.20f, 0.07f},
        {0.75f, 0.61f, 0.23f},
        {0.63f, 0.56f, 0.37f},
        128.f,
        {0.00f, 0.00f, 0.00f}
    };
    // Cyan Plastic — cool tone, medium specular, slight emission
    Material matPlastic = {
        {0.00f, 0.10f, 0.06f},
        {0.00f, 0.51f, 0.51f},
        {0.50f, 0.50f, 0.50f},
        32.f,
        {0.00f, 0.05f, 0.10f}
    };
    // Chrome — mirror-like, extreme specular, very shiny
    Material matChrome = {
        {0.25f, 0.25f, 0.25f},
        {0.40f, 0.40f, 0.40f},
        {0.97f, 0.97f, 0.97f},
        256.f,
        {0.00f, 0.00f, 0.00f}
    };

    // ---- Render loop --------------------------------------
    while (!glfwWindowShouldClose(window)) {
        float time = (float)glfwGetTime();

        glClearColor(0.12f, 0.12f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phong.use();

        // ---- Shader mode ----------------------------------
        phong.setInt("shaderMode", gShaderMode);

        // ---- Animated orbiting light ----------------------
        glm::vec3 lightPos(
            8.f * sinf(time * 0.7f),
            5.f,
            8.f * cosf(time * 0.7f)
        );
        phong.setVec3("light.position", lightPos);
        phong.setVec3("light.ambient", glm::vec3(0.3f));
        phong.setVec3("light.diffuse", glm::vec3(1.0f));
        phong.setVec3("light.specular", glm::vec3(1.0f));
        phong.setFloat("light.constant", 1.0f);
        phong.setFloat("light.linear", 0.09f);
        phong.setFloat("light.quadratic", 0.032f);

        // ================================================
        //  Item 4 — 3 distinct MVP configurations
        // ================================================
        glm::mat4 projection(1.f), view(1.f);
        glm::vec3 camPos;
        float aspect = (float)SCR_W / SCR_H;

        if (gConfig == 1) {
            // Config 1: Standard perspective front view
            // Camera at eye level, 60 FOV — natural view
            camPos = glm::vec3(0.f, 0.f, 10.f);
            view = glm::lookAt(camPos,
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.f, 1.f, 0.f));
            projection = glm::perspective(
                glm::radians(60.f), aspect, 0.1f, 1000.f);
        }
        else if (gConfig == 2) {
            // Config 2: Dramatic low angle
            // Camera below looking up — heroic perspective
            camPos = glm::vec3(0.f, -4.f, 8.f);
            view = glm::lookAt(camPos,
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.f, 1.f, 0.f));
            projection = glm::perspective(
                glm::radians(90.f), aspect, 0.1f, 1000.f);
        }
        else {
            // Config 3: Orthographic side view
            // No perspective distortion — technical drawing style
            camPos = glm::vec3(12.f, 0.f, 0.f);
            view = glm::lookAt(camPos,
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.f, 1.f, 0.f));
            float h = 4.f;
            projection = glm::ortho(
                -h * aspect, h * aspect, -h, h, 0.1f, 1000.f);
        }

        phong.setMat4("view", view);
        phong.setMat4("projection", projection);
        phong.setVec3("viewPos", camPos);

        // ================================================
        //  3 Objects — same model, different materials
        //  Arranged in clean horizontal line
        // ================================================

        // Object 1 — Gold (left), slowly rotating
        {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, glm::vec3(-3.5f, 0.f, 0.f));
            model = glm::rotate(model, time * 0.4f, glm::vec3(0.f, 1.f, 0.f));
            model = glm::scale(model, glm::vec3(0.05f));
            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            phong.setMat4("model", model);
            phong.setMat3("normalMatrix", normalMatrix);
            applyMaterial(phong, matGold);
            ourModel.Draw(phong);
        }

        // Object 2 — Cyan Plastic (center), stationary
        {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
            model = glm::rotate(model, glm::radians(20.f), glm::vec3(0.f, 1.f, 0.f));
            model = glm::scale(model, glm::vec3(0.05f));
            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            phong.setMat4("model", model);
            phong.setMat3("normalMatrix", normalMatrix);
            applyMaterial(phong, matPlastic);
            ourModel.Draw(phong);
        }

        // Object 3 — Chrome (right), counter-rotating
        {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, glm::vec3(3.5f, 0.f, 0.f));
            model = glm::rotate(model, -time * 0.4f, glm::vec3(0.f, 1.f, 0.f));
            model = glm::scale(model, glm::vec3(0.05f));
            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            phong.setMat4("model", model);
            phong.setMat3("normalMatrix", normalMatrix);
            applyMaterial(phong, matChrome);
            ourModel.Draw(phong);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}