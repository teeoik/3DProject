# 3D Project

Practice project for managing 3D models and graphics.

The goal is to practice efficient multithreaded code and rendering while working with 3D models.

## Current Features
- 3D model viewer with multi-panel docking UI
    - Load and display 3D mesh in OBJ format (via Assimp)
    - Model information panel showing mesh count, vertex count, and triangle count
    - Real-time viewport rendering with OpenGL 3.3
- Interactive camera controls:
    - Right mouse drag: Orbit camera around model
    - Middle mouse drag: Pan camera
    - Mouse wheel: Zoom in/out
- Toolbar with file browser for loading models

Project uses OpenGL for rendering. GLFW is used for window and input management, GLAD for OpenGL 
function loading, and Dear ImGui for the docking UI interface.

## Tech Stack
- C++23
- OpenGL 3.3 Core
- GLFW (window and input management)
- GLAD (OpenGL function loader)
- Dear ImGui with docking (UI framework)
- Assimp (3D model loading)
- GLM (mathematics library)
- CMake + vcpkg (build system)
- Visual Studio 2022 (Windows)

## Build
```bash
# Prerequisites:
# - Visual Studio 2022 with C++ development tools
# - vcpkg installed and integrated

# Clone the repository
git clone https://github.com/teeoik/3DProject
cd 3DProject

# Open folder in Visual Studio as a CMake project
# Visual Studio will automatically configure CMake and install dependencies via vcpkg
# Build and run from Visual Studio

# Or use CMake directly:
cmake --preset=default
cmake --build out/build/x64-Debug
```

## Project Structure
```
3DProject/
├── src/
│   ├── app/         # Application layer (main loop, UI coordination)
│   ├── gfx/         # Graphics (renderer, shaders, model/mesh loading)
│   ├── scene/       # Scene management (camera, scene data)
│   ├── ui/          # UI components (ImGui panels, viewport)
│   ├── platform/    # Platform abstraction (GLFW window)
│   └── shaders/     # GLSL shader files
├── CMakeLists.txt
└── vcpkg.json       # Dependency manifest
```

