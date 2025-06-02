# Config
cmake_minimum_required(VERSION 3.31)
project(y5 C)
set(CMAKE_C_STANDARD 23)

# GLFW
add_subdirectory(lib/glfw)

# GLAD 1
add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

# OpenGL
find_package(OpenGL REQUIRED)

# Executables
add_executable(y5 src/main.c)

target_link_libraries(y5 PRIVATE glfw glad OpenGL::GL)
