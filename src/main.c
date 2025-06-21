#include "mesh_generator.h"
#include "shader_loader.h"
#include "stolen_img_loader.h"
#include "types.h"

#include <math.h>
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

struct Array* provide_shaders(bool useFirst);

void elmo_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize,
	unsigned int* texture1,
	unsigned int* texture2
);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

	if (mainWindow == nullptr) {
		fprintf(stderr, "Error creating window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mainWindow);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	if (!gladLoadGLLoader(glfwGetProcAddress)) {
		fprintf(stderr, "Error loading GLAD");
		return -1;
	}

	struct Array* shadersArray2	  = provide_shaders(false);
	unsigned int  shader_program2 = compile_shaders_to_shader_program(shadersArray2);

	unsigned int vertexBuffer2, // VBO
		vertexArray2,			// VAO
		elementBuffer2,			// EBO
		trianglesSize2, texture1, texture2;

	elmo_vbo_vao_ebo(
		&vertexBuffer2, &vertexArray2, &elementBuffer2, &trianglesSize2, &texture1, &texture2
	);
	use_shader(&shader_program2);
	set_uniform_int(&shader_program2, "u_elmoTexture", 0);
	set_uniform_int(&shader_program2, "u_obamaTexture", 1);

	while (!glfwWindowShouldClose(mainWindow)) {

		process_inputs(mainWindow);

		glClearColor(screenColors[0], screenColors[1], screenColors[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		use_shader(&shader_program2);

		glBindVertexArray(vertexArray2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		float timeValue	 = glfwGetTime();
		float greenValue = (cosf(timeValue) + 1) / 2;
		set_uniform_float(&shader_program2, "u_positionMultiplier", -greenValue);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vertexArray2);
	glDeleteBuffers(1, &vertexBuffer2);

	glDeleteProgram(shader_program2);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
	fprintf(stdout, "Framebuffer size: (%d*%d)\n", width, height);
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

struct Array* provide_shaders(bool useFirst) {

	struct Shader* shadersPtr = malloc(sizeof(struct Shader) * 2);
	if (!shadersPtr) {
		fprintf(stderr, "Failed to allocate shader program memory\n");
		return new_array(nullptr, 0);
	}

	shadersPtr[0].name = VERTEX_SHADER;
	shadersPtr[0].type = GL_VERTEX_SHADER;

	shadersPtr[1].name = useFirst ? FRAGMENT_SHADER_NOISE_1 : FRAGMENT_SHADER_NOISE_2;
	shadersPtr[1].type = GL_FRAGMENT_SHADER;

	return new_array(shadersPtr, 2);
}

void elmo_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize,
	unsigned int* texture1,
	unsigned int* texture2
) {

	// clang-format off
	float vertices[] = {
		// pos               // col              // tex coord (flipped tho)
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f
	};

	unsigned int indices[] = {
		// 0, 1, 3,
		// 1, 2, 3 ,
		1, 2, 4
	};
	// clang-format on

	glGenVertexArrays(1, vertexArray);
	glGenBuffers(1, vertexBuffer);
	glGenBuffers(1, elementBuffer);

	glBindVertexArray(*vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*trianglesSize = sizeof(indices) / sizeof(unsigned int);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, texture1);
	glBindTexture(GL_TEXTURE_2D, *texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* imageData1 = nullptr;
	unsigned	   width1, height1;

	unsigned error1 =
		lodepng_decode32_file(&imageData1, &width1, &height1, "res/textures/obama.png");
	if (error1)
		fprintf(stderr, "error1 %u: %s\n", error1, lodepng_error_text(error1));

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData1
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, texture2);
	glBindTexture(GL_TEXTURE_2D, *texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* imageData2 = nullptr;
	unsigned	   width2, height2;
	unsigned	   error2 =
		lodepng_decode32_file(&imageData2, &width2, &height2, "res/textures/elmo.png");
	if (error1)
		fprintf(stderr, "error2 %u: %s\n", error2, lodepng_error_text(error2));

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2
	);
	glGenerateMipmap(GL_TEXTURE_2D);
}
