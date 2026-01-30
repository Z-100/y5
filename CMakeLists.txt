##########
# Config #
##########

cmake_minimum_required(VERSION 3.31)
project(y5 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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


###########
# LodePNG #
###########

add_library(lodepng STATIC lib/lodepng/src/lodepng.c)
target_include_directories(lodepng PUBLIC lib/lodepng/include)


#################
# TinyObjLoader #
#################

add_subdirectory(lib/tinyobjloader)


#############
# MiniAudio #
#############

add_subdirectory(lib/miniaudio)


################
# OpenGL + X11 #
################

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)


##########
# cimgui #
##########

set(CIMGUI_DIR lib/cimgui)
set(IMGUI_DIR ${CIMGUI_DIR}/imgui)
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

file(GLOB CIMGUI_CPP_FILES
        "${CIMGUI_DIR}/*.cpp"
        "${IMGUI_DIR}/*.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_glfw.cpp"
        "${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp"
)

add_library(cimgui STATIC ${CIMGUI_CPP_FILES})

set_target_properties(cimgui PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(cimgui PUBLIC
        ${CIMGUI_DIR}
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS}
)

target_compile_definitions(cimgui PUBLIC
        IMGUI_IMPL_API=
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(cimgui PUBLIC
        glfw
        glad
        OpenGL::GL
)


##########################
# Link sources/resources #
##########################

file(GLOB RESOURCES "res/*")

file(GLOB_RECURSE SRC_FILES
        CONFIGURE_DEPENDS
        "src/*.c"
        "src/bridges/*.cpp"
)

file(GLOB_RECURSE HDR_FILES
        CONFIGURE_DEPENDS
        "include/*.h"
)

include_directories(include)
file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)


###############
# Executables #
###############

add_executable(y5 ${SRC_FILES} ${HDR_FILES})

target_include_directories(y5 PRIVATE src)

set_target_properties(y5 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(y5 PRIVATE
        glfw
        glad
        cglm
        lodepng
        tinyobjloader
        miniaudio
        cimgui
        OpenGL::GL
        ${X11_LIBRARIES}
        stdc++
        m
        dl
)
