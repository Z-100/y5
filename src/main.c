#include "mesh_generator.h"
#include "shader_loader.h"
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

void bind_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize,
	bool		  useFirstIndices
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

	struct Array* shadersArray1	 = provide_shaders(true);
	unsigned int  shader_program = compile_shaders_to_shader_program(shadersArray1);

	unsigned int vertexBuffer, // VBO
		vertexArray,		   // VAO
		elementBuffer,		   // EBO
		trianglesSize;

	bind_vbo_vao_ebo(&vertexBuffer, &vertexArray, &elementBuffer, &trianglesSize, true);

	struct Array* shadersArray2	  = provide_shaders(false);
	unsigned int  shader_program2 = compile_shaders_to_shader_program(shadersArray2);

	unsigned int vertexBuffer2, // VBO
		vertexArray2,			// VAO
		elementBuffer2,			// EBO
		trianglesSize2;

	bind_vbo_vao_ebo(&vertexBuffer2, &vertexArray2, &elementBuffer2, &trianglesSize2, false);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(mainWindow)) {

		process_inputs(mainWindow);

		glClearColor(screenColors[0], screenColors[1], screenColors[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Triangle magic
		use_shader(&shader_program);



		float timeValue = glfwGetTime();
		float greenValue = (cosf(timeValue) + 1) / 2;

		set_uniform_float(&shader_program, "u_colorMultiplier", greenValue);

		float vec4[] = { 1.0f, 0.5f, 0.25f, 1.0f };
		float vec3[] = { 1.0f, 0.5f, 0.25f };
		// set_uniform_vec4(&shader_program, "u_vec4Stuff", vec4);
		set_uniform_vec3(&shader_program, "u_vec3Stuff", vec3);

		set_uniform_float(&shader_program, "u_positionMultiplier", greenValue);


		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, trianglesSize, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		// Triangle magic 2
		use_shader(&shader_program2);

		glBindVertexArray(vertexArray2);
		glDrawElements(GL_TRIANGLES, trianglesSize2, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);




		set_uniform_float(&shader_program2, "u_positionMultiplier", -greenValue);



		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);

	glDeleteProgram(shader_program);
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

void bind_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize,
	bool		  useFirstIndices
) {

	struct FloatArray* verticesData = generate_vertices(10, 10);
	struct IntArray*   indicesData =
		  useFirstIndices ? generate_indices(10, 10) : generate_indices2(10, 10);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	*trianglesSize = indicesData->length;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
