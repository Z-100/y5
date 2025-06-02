cmake_minimum_required(VERSION 3.31)
project(y5 C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(lib/glfw)

add_executable(y5 src/main.c)

target_link_libraries(y5 PRIVATE glfw)
