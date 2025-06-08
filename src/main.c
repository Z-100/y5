#include "file_reader.h"
#include "mesh_generator.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

const char* TITLE  = "y5";
const int	WIDTH  = 800;
const int	HEIGHT = 600;

float screenColors[] = { 0, 1, 0 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);

unsigned int compile_glsl_shader(const char* shaderName, unsigned int shaderType);
unsigned int build_shader_program();

void bind_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize
);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

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

	unsigned int shaderProgram = build_shader_program();

	unsigned int vertexBuffer, // VBO
		vertexArray,		   // VAO
		elementBuffer,		   // EBO
		trianglesSize;

	bind_vbo_vao_ebo(&vertexBuffer, &vertexArray, &elementBuffer, &trianglesSize);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(mainWindow)) {

		process_inputs(mainWindow);

		glClearColor(screenColors[0], screenColors[1], screenColors[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Triangle magic
		glUseProgram(shaderProgram);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, trianglesSize, GL_UNSIGNED_INT, nullptr);
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

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
	printf("Framebuffer size: %dx%d\n", width, height);
}

void process_inputs(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		screenColors[0] += 0.05f;
		screenColors[1] -= 0.05f;
		screenColors[2] += 0.05f;
	}
}

unsigned int build_shader_program() {

	unsigned int vertexShader =
		compile_glsl_shader("res/shaders/VertexShader.glsl", GL_VERTEX_SHADER);

	unsigned int fragmentShader =
		compile_glsl_shader("res/shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

	if (vertexShader == 0 || fragmentShader == 0)
		printf("Error compiling shaders, exiting...\n");

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int	 success;
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

void bind_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize
) {

	struct FloatArray* verticesData = generate_vertices(-1.0f, 1.0f, -1.0f, 1.0f);
	struct IntArray*   indicesData	= generate_indices(0, 0);

	unsigned long long verticesDataSize = verticesData->length * sizeof(float);
	unsigned long long indicesDataSize	= indicesData->length * sizeof(unsigned int);

	glGenVertexArrays(1, vertexArray);
	glGenBuffers(1, vertexBuffer);
	glGenBuffers(1, elementBuffer);

	glBindVertexArray(*vertexArray);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesDataSize, verticesData->data, GL_STATIC_DRAW);

	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize, indicesData->data, GL_STATIC_DRAW);

	// Define vertices structure
	int triangle_definition_size = 3;
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, triangle_definition_size * sizeof(float), nullptr
	);
	glEnableVertexAttribArray(0);
	*trianglesSize = indicesData->length;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int compile_glsl_shader(const char* shaderName, const unsigned int shaderType) {

	char* shaderPtr = readLines(shaderName);

	const unsigned int vertexShader = glCreateShader(shaderType);
	glShaderSource(vertexShader, 1, &shaderPtr, nullptr);
	glCompileShader(vertexShader);

	free(shaderPtr);

	int	 success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		printf("Error during shader compilation: %s", infoLog);
		return 0;
	}

	return vertexShader;
}
