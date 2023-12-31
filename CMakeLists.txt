cmake_minimum_required(VERSION 3.26)
project(opengl_potd)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm/glm)

find_package(OpenGL REQUIRED)

include_directories(libs/glfw/include/GLFW)
include_directories(libs/glad)
include_directories(libs/glm/glm)
include_directories(include)
include_directories(libs/nlohmann)
include_directories(libs/stb)
include_directories(libs/khronos)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(opengl_potd main.cpp libs/glad/glad.c)
target_link_libraries(opengl_potd glfw)