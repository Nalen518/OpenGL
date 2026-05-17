#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;     // NEW: forward to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; // NEW: precomputed on CPU, not GPU

void main() {
    // ================================================
    // 1. WORLD SPACE POSITION
    //    Multiply by model matrix to get world position
    //    Used in fragment shader for light direction calc
    // ================================================
    FragPos = vec3(model * vec4(aPos, 1.0));

    // ================================================
    // 2. NORMAL TRANSFORMATION
    //    Use precomputed normalMatrix from CPU
    //    This correctly handles non-uniform scaling
    //    without computing inverse() on every vertex
    // ================================================
    Normal = normalMatrix * aNormal;

    // ================================================
    // 3. TEXTURE COORDINATES
    //    Pass through to fragment shader
    //    Even if unused now, ready for future use
    // ================================================
    TexCoords = aTexCoords;

    // ================================================
    // 4. CLIP SPACE POSITION
    //    Final position = Projection * View * World
    //    This is what OpenGL uses to draw on screen
    // ================================================
    gl_Position = projection * view * vec4(FragPos, 1.0);
}