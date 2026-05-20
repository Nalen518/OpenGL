# 🎮 OpenGL 3D Rendering Pipeline

> A real-time 3D rendering application built with **C++ and OpenGL 4.1 Core Profile**, featuring a custom Phong shading pipeline, Assimp-based model loading, and multiple interactive MVP matrix configurations.

---

## ✨ Features

| Feature | Details |
|---|---|
| **Phong Shading** | Ambient · Diffuse · Specular per-pixel lighting |
| **4 Shader Modes** | Standard Phong → Attenuation → Rim Light → Emission |
| **3D Model Loading** | Wavefront `.obj` via Assimp with custom MTL stripping |
| **3 MVP Configurations** | Perspective · Dramatic Low Angle · Orthographic |
| **3 Material Types** | Gold · Cyan Plastic · Chrome |
| **Animated Scene** | Orbiting light source, auto-rotating models |
| **Wireframe Toggle** | Switch between solid and wireframe rendering |

---

---

## 🗂️ Project Structure

```
OpenGL/
├── Main.cpp                  # Entry point, render loop, MVP configs
├── shader.h / shader.cpp     # GLSL shader loader & uniform setter
├── model.h  / model.cpp      # Assimp model loader, scene graph traversal
├── mesh.h   / mesh.cpp       # VAO/VBO/EBO pipeline, draw call
├── glad.c                    # GLAD OpenGL loader
├── shaders/
│   ├── phong.vert            # Vertex shader (world transform, normal matrix)
│   └── phong.frag            # Fragment shader (Phong + attenuation + rim + emission)
├── models/
│   └── Water.obj             # 3D geometry model
├ 
└── OpenGL.sln                # Visual Studio 2022 solution
```

---

## 🚀 How to Run

### Option A — Pre-built Executable (Windows)

1. Download [`OpenGL.zip`](../../releases/latest) from the Releases page
2. Extract the ZIP — keep all folders in place
3. Double-click `OpenGL.exe`

> **Requirement:** [Visual C++ Redistributable 2022 (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe) must be installed.

### Option B — Build from Source (Visual Studio 2022)

**Prerequisites:**

| Library | Version | Notes |
|---|---|---|
| GLFW | 3.x | Window & input |
| GLAD | GL 4.1 | OpenGL loader |
| GLM | 0.9.9+ | Math library |
| Assimp | 5.x | Model loading |

```
1. Clone this repository
2. Open  OpenGL.sln  in Visual Studio 2022
3. Set build to  x64 | Debug
4. Press F5 to build and run
```

---

## ⌨️ Controls

| Key | Action |
|---|---|
| `1` | Config 1 — Standard Perspective (60° FOV) |
| `2` | Config 2 — Dramatic Low Angle (90° FOV) |
| `3` | Config 3 — Orthographic side view |
| `F` | Cycle shader mode (0 → 1 → 2 → 3) |
| `W` | Toggle wireframe mode |
| `ESC` | Quit |

---

## 💡 Shader Modes

| Mode | Name | Description |
|---|---|---|
| `0` | Standard Phong | Ambient + Diffuse + Specular |
| `1` | + Attenuation | Light intensity fades with distance |
| `2` | + Rim Light | Blue silhouette glow at grazing angles |
| `3` | + Emission | Material self-illumination |

---

## 🧱 MVP Configurations

### Config 1 — Standard Perspective
- Camera at `(0, 0, 10)`, 60° FOV
- Natural human-eye view of the scene

### Config 2 — Dramatic Low Angle
- Camera at `(0, -4, 8)` looking up, 90° FOV
- Heroic, cinematic perspective with strong depth convergence

### Config 3 — Orthographic
- Camera at `(12, 0, 0)`, side profile
- No perspective distortion — technical/CAD-style rendering

---

## 🎨 Materials

| Object | Material | Shininess | Key Property |
|---|---|---|---|
| Left | Gold | 128 | Warm yellow, strong specular |
| Center | Cyan Plastic | 32 | Cool tone, subtle emission |
| Right | Chrome | 256 | Mirror-like, extreme specular |

---



---

## 🛠️ Tech Stack

- **Language:** C++17
- **Graphics API:** OpenGL 4.1 Core Profile
- **Windowing:** GLFW 3
- **OpenGL Loader:** GLAD
- **Math:** GLM (OpenGL Mathematics)
- **Model Loading:** Assimp 5
- **IDE:** Visual Studio 2022

---

## 📝 License

This project was created as a Computer Graphics course assignment.
