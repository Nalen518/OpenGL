#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// ---- Material properties ----
struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
    vec3  emission;      // NEW: self-glow color
};

// ---- Light source ----
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // NEW: attenuation factors
    float constant;      // usually 1.0
    float linear;        // weakens light over distance
    float quadratic;     // weakens light faster at range
};

uniform Material material;
uniform Light     light;
uniform vec3      viewPos;

// NEW: shader mode toggle (set from main.cpp)
// 0 = standard Phong
// 1 = Phong + attenuation
// 2 = Phong + attenuation + rim light
// 3 = Phong + attenuation + rim + emission
uniform int shaderMode;

void main() {
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);

    // ================================================
    // 1. AMBIENT — constant base illumination
    // ================================================
    vec3 ambient = light.ambient * material.ambient;

    // ================================================
    // 2. DIFFUSE — Lambert: brighter when facing light
    // ================================================
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // ================================================
    // 3. SPECULAR — Phong reflection toward viewer
    // ================================================
    vec3  reflDir = reflect(-lightDir, norm);
    float spec    = pow(max(dot(viewDir, reflDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // ================================================
    // 4. ATTENUATION — light weakens over distance
    //    Formula: 1 / (c + l*d + q*d^2)
    //    Modes 1, 2, 3 only
    // ================================================
    float attenuation = 1.0; // default: no attenuation
    if (shaderMode >= 1) {
        float distance    = length(light.position - FragPos);
        attenuation       = 1.0 / (
            light.constant +
            light.linear    * distance +
            light.quadratic * distance * distance
        );
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }

    // ================================================
    // 5. RIM LIGHT — glows at edges facing away from camera
    //    Creates a dramatic backlit outline effect
    //    Modes 2, 3 only
    // ================================================
    vec3 rimLight = vec3(0.0);
    if (shaderMode >= 2) {
        float rim    = 1.0 - max(dot(viewDir, norm), 0.0);
        rim          = pow(rim, 3.0); // sharpness of rim
        rimLight     = rim * vec3(0.2, 0.5, 1.0); // blue rim glow
    }

    // ================================================
    // 6. EMISSION — object glows on its own
    //    Independent of any light source
    //    Mode 3 only
    // ================================================
    vec3 emission = vec3(0.0);
    if (shaderMode >= 3) {
        emission = material.emission;
    }

    // ================================================
    // FINAL OUTPUT — combine all components
    // ================================================
    vec3 result = ambient + diffuse + specular + rimLight + emission;
    FragColor   = vec4(result, 1.0);
}