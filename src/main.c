#include <cglm/cglm.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <lodepng.h>

#include "graphics/shader_loader.h"
#include "core/types.h"
#include "graphics/camera.h"
#include "gui/gui.h"
#include "utils/math_utils.h"

// =============
// = constants =
// =============

constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

bool  firstMouse = true;
float lastX		 = 0.0f;
float lastY		 = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Camera* camera;

// =======================
// = method declarations =
// =======================

static void error_callback(int id, const char* msg) {
	printf("Error %d: %s\n", id, msg);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosD, double yPosD);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void process_inputs(GLFWwindow* window);

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

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float main_scale = gui_scale();

	int width_scaled  = (int) (WIDTH * main_scale);
	int height_scaled = (int) (HEIGHT * main_scale);

	GLFWwindow* mainWindow = glfwCreateWindow(width_scaled, height_scaled, "y5", nullptr, nullptr);

	if (mainWindow == nullptr) {
		fprintf(stderr, "Error creating window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mainWindow);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	// le vsync
	glfwSwapInterval(1);

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		fprintf(stderr, "Error loading GLAD");
		return -1;
	}

	gui_init(mainWindow);

	struct Shader shaders_texture[] = {
		{ .name = VERTEX_SHADER_TEXTURES, .type = GL_VERTEX_SHADER },
		{ .name = FRAGMENT_SHADER_TEXTURES, .type = GL_FRAGMENT_SHADER }
	};

	struct Shader shaders_light_source[] = {
		{ .name = VERTEX_SHADER_LIGHT_SOURCE, .type = GL_VERTEX_SHADER },
		{ .name = FRAGMENT_SHADER_LIGHT_SOURCE, .type = GL_FRAGMENT_SHADER }
	};

	unsigned int shader_texture = compile_shaders_to_shader_program(new_array(shaders_texture, 2));
	unsigned int shader_light_source =
		compile_shaders_to_shader_program(new_array(shaders_light_source, 2));

	unsigned int VBO, VAO, EBO, indicesSize, texture1, texture2;

	elmo_vbo_vao_ebo(&VBO, &VAO, &EBO, &indicesSize, &texture1, &texture2);

	use_shader(&shader_texture);
	set_uniform_int(&shader_texture, "u_elmoTexture", 0);
	set_uniform_int(&shader_texture, "u_obamaTexture", 1);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	vec3 lightPosition = { 0.0f, 5.0f, 0.0f };
	vec3 lightColor	   = { 1.0f, 1.0f, 1.0f };

	camera = create_default_camera();

	if (camera == nullptr)
		return -1;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(mainWindow)) {

		gui_update();

		float now = (float) glfwGetTime();
		deltaTime = now - lastFrame;
		lastFrame = now;

		process_inputs(mainWindow);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		use_shader(&shader_texture);
		set_uniform_vec3(&shader_texture, "u_lightColor", &lightColor);

		mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
		glm_perspective(
			glm_rad(camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f, projectionTransform
		);
		set_uniform_mat4(&shader_texture, "u_projectionTransform", &projectionTransform);

		mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
		camera_get_view_matrix(camera, &viewTransform);
		set_uniform_mat4(&shader_texture, "u_viewTransform", &viewTransform);

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++) {

			mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
			glm_translate(modelMatrix, cubePositions[i]);

			float angle = 20.0f * (float) i;
			glm_rotate(modelMatrix, glm_rad(now * angle), (vec3) { 1.0f, 0.3f, 0.5f });

			set_uniform_mat4(&shader_texture, "u_modelTransform", &modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		use_shader(&shader_light_source);
		set_uniform_vec3(&shader_light_source, "u_lightSourceColor", &lightColor);

		set_uniform_mat4(&shader_light_source, "u_projectionTransform", &projectionTransform);
		set_uniform_mat4(&shader_light_source, "u_viewTransform", &viewTransform);

		mat4 modelTransform = GLM_MAT4_IDENTITY_INIT;
		glm_translate(modelTransform, lightPosition);

		vec3 lightSize = {
			math_max2_float(lightColor[0] * 0.2f, 0.1f),
			math_max2_float(lightColor[1] * 0.1f, 0.1f),
			math_max2_float(lightColor[2] * 1.2f, 0.1f),
		};
		glm_scale(modelTransform, lightSize);

		set_uniform_mat4(&shader_light_source, "u_modelTransform", &modelTransform);

		lightColor[0] = sinf(lastFrame);
		lightColor[1] = cosf(lastFrame);
		lightColor[2] = -sinf(lastFrame);

		glm_vec3_scale(lightColor, lightColor[0] * 1.2f, lightColor);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		gui_render();

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(shader_texture);

	gui_terminate();

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

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera_process_keyboard(DOWN, deltaTime, camera);
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
