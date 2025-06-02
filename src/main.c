#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float screenColors[] = {0, 1, 0};

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "y5", nullptr, nullptr);

    if (mainWindow == nullptr) {
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

        glClearColor(
            screenColors[0],
            screenColors[1],
            screenColors[2],
            1.0f
        );
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(mainWindow);

        glfwPollEvents();
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
    printf("Framebuffer size: %dx%d\n", width, height);
}

void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        screenColors[0] += 0.05f;
        screenColors[1] -= 0.05f;
        screenColors[2] += 0.05f;
    }
}
