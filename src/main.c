#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "file_reader.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);
unsigned int compileShaderIntoGL(const char *shaderName, unsigned int shaderType);
unsigned int buildShaderProgram();

void bind_vao_vbo(unsigned int* vertexBuffer, unsigned int* vertexArray);

float screenColors[] = {0, 1, 0};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *mainWindow = glfwCreateWindow(800, 600, "y5", nullptr, nullptr);

    if (mainWindow == nullptr) {
        printf("Error creating window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mainWindow);
    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

    if (!gladLoadGLLoader(glfwGetProcAddress)) {
        printf("Error loading GLAD");
        return -1;
    }

    unsigned int shaderProgram = buildShaderProgram();
    unsigned int vertexBuffer, vertexArray;
    bind_vao_vbo(&vertexBuffer, &vertexArray);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(mainWindow)) {
        processInput(mainWindow);

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    printf("Framebuffer size: %dx%d\n", width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        screenColors[0] += 0.05f;
        screenColors[1] -= 0.05f;
        screenColors[2] += 0.05f;
    }
}


unsigned int buildShaderProgram() {

    unsigned int vertexShader = compileShaderIntoGL("res/shaders/VertexShader.glsl", GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShaderIntoGL("res/shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        printf("Error compiling shaders, exiting...\n");
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
        printf("Error during shader linking: %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void bind_vao_vbo(unsigned int* vertexBuffer, unsigned int* vertexArray) {

    float vertices[] = {
        -0.5f, -0.5f, 0,
         0.5f, -0.5f, 0,
            0,  0.5f, 0,
    };

    glGenVertexArrays(1, vertexArray);
    glGenBuffers(1, vertexBuffer);

    glBindVertexArray(*vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int compileShaderIntoGL(const char *shaderName, const unsigned int shaderType) {

    char *shaderPtr = readLines(shaderName);

    const unsigned int vertexShader = glCreateShader(shaderType);
    glShaderSource(vertexShader, 1, &shaderPtr, nullptr);
    glCompileShader(vertexShader);

    free(shaderPtr);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        printf("Error during shader compilation: %s", infoLog);
        return 0;
    }

    return vertexShader;
}
