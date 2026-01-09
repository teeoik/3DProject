# CMakeList.txt : CMake project for 3DProject, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.20)
project(3DProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
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
)

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


