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
find_package(glm CONFIG REQUIRED)

add_subdirectory(src)


