#include "shader_loader.h"
#include "stolen_img_loader.h"
#include "types.h"
#include "camera.h"

#include <cglm/cglm.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// =============
// = constants =
// =============

const char*	  TITLE	 = "y5";
constexpr int WIDTH	 = 800;
constexpr int HEIGHT = 600;

float screenColors[] = { 0, 1, 0 };

bool  firstMouse = true;
float lastX		 = 0.0f;
float lastY		 = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Camera* camera;

// =======================
// = method declarations =
// =======================

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosD, double yPosD);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
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

// ==========================
// = method implementations =
// ==========================

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

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);

	if (!gladLoadGLLoader(glfwGetProcAddress)) {
		fprintf(stderr, "Error loading GLAD");
		return -1;
	}

	struct Array* shadersArray	 = provide_shaders(false);
	unsigned int  shader_program = compile_shaders_to_shader_program(shadersArray);

	unsigned int VBO, VAO, EBO, indicesSize, texture1, texture2;

	elmo_vbo_vao_ebo(&VBO, &VAO, &EBO, &indicesSize, &texture1, &texture2);
	use_shader(&shader_program);

	set_uniform_int(&shader_program, "u_elmoTexture", 0);
	set_uniform_int(&shader_program, "u_obamaTexture", 1);

	// clang-format off
	vec3 cubePositions[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f },
		{ -3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{ -1.7f, 3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f, 2.0f, -2.5f },
		{ 1.5f, 0.2f, -1.5f },
		{ -1.3f, 1.0f, -1.5f },
	};
	// clang-format on

	camera = create_default_camera();

	if (camera == nullptr)
		return -1;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(mainWindow)) {

		float now = (float) glfwGetTime();
		deltaTime = now - lastFrame;
		lastFrame = now;

		process_inputs(mainWindow);

		glClearColor(screenColors[0], screenColors[1], screenColors[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		use_shader(&shader_program);

		mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
		glm_perspective(
			glm_rad(camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f, projectionTransform
		);
		set_uniform_mat4(&shader_program, "u_projectionTransform", &projectionTransform);

		mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
		camera_get_view_matrix(camera, &viewTransform);
		set_uniform_mat4(&shader_program, "u_viewTransform", &viewTransform);

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++) {

			mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
			glm_translate(modelMatrix, cubePositions[i]);

			float angle = 20.0f * (float) i;
			glm_rotate(modelMatrix, glm_rad(now * angle), (vec3) { 1.0f, 0.3f, 0.5f });

			set_uniform_mat4(&shader_program, "u_modelTransform", &modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(shader_program);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
	fprintf(stdout, "Framebuffer size: (%d*%d)\n", width, height);
}

void mouse_callback(GLFWwindow* window, double xPosD, double yPosD) {

	float xPos = (float) xPosD;
	float yPos = (float) yPosD;

	if (firstMouse) {
		lastX	   = xPos;
		lastY	   = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX		  = xPos;
	lastY		  = yPos;

	camera_process_mouse_movement(xOffset, yOffset, camera);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera_process_mouse_scroll((float) yOffset, camera);
}

void process_inputs(GLFWwindow* window) {

	// ---------------
	// Close da window
	// ---------------

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// ----------------
	// Process movemint
	// ----------------

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_process_keyboard(FORWARD, deltaTime, camera);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_process_keyboard(LEFT, deltaTime, camera);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_process_keyboard(BACKWARD, deltaTime, camera);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_process_keyboard(RIGHT, deltaTime, camera);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera_process_keyboard(UP, deltaTime, camera);

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		camera_process_keyboard(DOWN, deltaTime, camera);
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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// clang-format on

	glGenVertexArrays(1, vertexArray);
	glGenBuffers(1, vertexBuffer);

	glBindVertexArray(*vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, texture1);
	glBindTexture(GL_TEXTURE_2D, *texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* imageData1 = nullptr;
	unsigned	   width1, height1;

	unsigned error1 =
		lodepng_decode32_file(&imageData1, &width1, &height1, "res/textures/elmo.png");
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

	unsigned error2 =
		lodepng_decode32_file(&imageData2, &width2, &height2, "res/textures/obama.png");
	if (error1)
		fprintf(stderr, "error2 %u: %s\n", error2, lodepng_error_text(error2));

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2
	);
	glGenerateMipmap(GL_TEXTURE_2D);
}
