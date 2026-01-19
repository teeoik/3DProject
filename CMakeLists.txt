# CMakeList.txt : CMake project for 3DProject, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.20)
project(3DProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(3DProject
    src/main.cpp
    src/app/Application.cpp
    src/platform/GlfwWindow.cpp
    src/ui/ImGuiLayer.cpp
    src/gfx/Mesh.cpp
    src/gfx/ModelLoader.cpp
    src/gfx/RenderTarget.cpp
    src/scene/CameraOrbit.cpp
    src/scene/Scene.cpp
    src/gfx/Renderer.cpp
    src/ui/ViewportPanel.cpp
)

# Hide console in release builds on Windows
if (WIN32)
    set_target_properties(3DProject PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    # Use WIN32 property but only apply SUBSYSTEM:WINDOWS linker flag for Release
    set_property(TARGET 3DProject APPEND PROPERTY LINK_OPTIONS
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
    )
    # Set entry point for Windows subsystem
    set_property(TARGET 3DProject APPEND PROPERTY LINK_OPTIONS
        $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
    )
endif()

target_include_directories(3DProject PRIVATE src)

# Link libs
target_link_libraries(3DProject PRIVATE
    glfw
    assimp::assimp
    glad::glad
    imgui::imgui
)

# Windows: OpenGL is in opengl32
if (WIN32)
    target_link_libraries(3DProject PRIVATE opengl32)
endif()

target_compile_definitions(3DProject PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)


