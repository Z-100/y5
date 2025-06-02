#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "y5", nullptr, nullptr);

    if (mainWindow == NULL) {
        printf("Error creating window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mainWindow);

    if (!gladLoadGLLoader(glfwGetProcAddress)) {
        printf("Error loading GLAD");
        return -1;
    }

    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
    printf("Framebuffer size: %dx%d\n", width, height);
}
