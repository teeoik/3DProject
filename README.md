# 3D Project

Practice project for managing 3D models and graphics.

The goal is to practice efficient multithreaded code and rendering while wokring with 3D models.
Current implementation includes:
- 3D model viewer
    - Load and display 3D models in formats: 
    - Controls to manipulate the view: 

Project uses OpenGL for rendering. GLFW is used for window and input management, GLAD for OpenGL 
function loading, and Dear ImGui for simple GUI.

## Tech Stack
- C++23
- OpenGL 3.3 Core
- GLFW
- GLAD
- Dear ImGui
- Assimp
- CMake + vcpkg
- Visual Studio 2026 (Windows)

## Build
```bash
# Requires vcpkg installed
# Open folder in Visual Studio (CMake project)
