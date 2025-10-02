##########
# Config #
##########

cmake_minimum_required(VERSION 3.31)
project(y5 C CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 17)


########
# GLFW #
########

add_subdirectory(lib/glfw)


########
# GLAD #
########

add_library(glad STATIC lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)


########
# CGLM #
########

add_subdirectory(lib/cglm)


##########
# OpenGL #
##########

find_package(OpenGL REQUIRED)


##########
# cimgui #
##########

set(CIMGUI_DIR lib/cimgui)
set(IMGUI_DIR ${CIMGUI_DIR}/imgui)
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

file(GLOB CIMGUI_C_FILES
    "${CIMGUI_DIR}/*.c"
    "${CIMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_BACKENDS}/imgui_impl_glfw.cpp"
    "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

add_library(cimgui STATIC ${CIMGUI_C_FILES})

target_include_directories(cimgui PUBLIC
    lib/cimgui
    lib/cimgui/imgui
    lib/cimgui/imgui/backends
)

target_compile_definitions(cimgui PUBLIC
    IMGUI_IMPL_API=
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(cimgui PUBLIC glfw glad OpenGL::GL)


##########################
# Link sources/resources #
##########################

file(GLOB RESOURCES "res/*")
file(GLOB SRC_FILES "src/*.c" "src/*.cpp")

include_directories(include)
file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)


###############
# Executables #
###############

add_executable(y5 ${SRC_FILES})
target_link_libraries(y5 PRIVATE glfw glad cglm cimgui OpenGL::GL)
