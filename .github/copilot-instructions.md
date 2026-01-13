# Copilot instructions for 3DProject

## Language & build
- Use C++23.
- Prefer standard library types (std::string, std::vector, std::span) over custom equivalents.
- Code must compile on Windows with MSVC and CMake.

## Code style
- Braces: Allman style.
- Indentation: 4 spaces, no tabs.
- Naming:
  - Types: PascalCase (Renderer, GlfwWindow)
  - Functions/methods: camelCase (makeContextCurrent)
  - Variables: camelCase (framebufferWidth)
  - Constants: UPPER_SNAKE_CASE
- Avoid macros unless required by third-party libraries.

## Comments & documentation
- Public APIs in headers: Doxygen-style comments '/** ... */' describing contract:
  - responsibilities, preconditions, postconditions, failure modes
  - document parameters only when semantics are not obvious (units, ownership, lifetime).
- Do not add comments that restate the code.
- If source files require comments, use '//'

## Error handling
- Use exceptions for fatal initialization failures (e.g., GLAD/GLFW init).
- Avoid exceptions in per-frame hot paths unless explicitly requested.
- Prefer early returns for error conditions in non-fatal routines.

## OpenGL specifics
- Use OpenGL 3.3 core profile.
- Use GLAD loader and define IMGUI_IMPL_OPENGL_LOADER_GLAD when relevant.
