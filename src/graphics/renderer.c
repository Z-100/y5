#include "graphics/renderer.h"
#include "utils/headers_collection.h"

// ============================
// = TODO: Manage differently =
// ============================

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

unsigned int VBO, VAO, EBO, indicesSize, texture1, texture2, lightCubeVAO;
unsigned int shader_texture, shader_light_source;

constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

// ============================
// = TODO: Manage differently =
// ============================

void renderer_init_default(const Game* game) {
	glfwSwapInterval(1); // le vsync
	glEnable(GL_DEPTH_TEST);
}

void renderer_update(const Game* game) {
	glfwSwapBuffers(game->main_window);
	glfwPollEvents();
}

void renderer_destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_texture);
}

// ============================
// = TODO: Manage differently =
// ============================

void renderer_initialize_cubes() {

	struct Shader shaders_texture[] = {
		{ .name = VERTEX_SHADER_TEXTURES, .type = GL_VERTEX_SHADER },
		{ .name = FRAGMENT_SHADER_TEXTURES, .type = GL_FRAGMENT_SHADER }
	};

	struct Shader shaders_light_source[] = {
		{ .name = VERTEX_SHADER_LIGHT_SOURCE, .type = GL_VERTEX_SHADER },
		{ .name = FRAGMENT_SHADER_LIGHT_SOURCE, .type = GL_FRAGMENT_SHADER }
	};

	shader_texture		= compile_shaders_to_shader_program(new_array(shaders_texture, 2));
	shader_light_source = compile_shaders_to_shader_program(new_array(shaders_light_source, 2));

	elmo_vbo_vao_ebo(&VBO, &VAO, &EBO, &indicesSize, &texture1, &texture2);

	use_shader(&shader_texture);
	set_uniform_int(&shader_texture, "u_elmoTexture", 0);
	set_uniform_int(&shader_texture, "u_obamaTexture", 1);

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (5 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void renderer_game_loop(const Game* game) {

	Camera* player_camera = game->player_camera;

	lightPosition[0] = 7.5f * sinf(game_last_frame());
	lightPosition[2] = 7.5f * cosf(game_last_frame());

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	use_shader(&shader_texture);
	set_uniform_vec3(&shader_texture, "u_lightPos", &lightPosition);
	set_uniform_vec3(&shader_texture, "u_lightColor", &lightColor);
	set_uniform_vec3(&shader_texture, "u_viewPos", &player_camera->position);
	set_uniform_vec3(&shader_texture, "u_objectColor", &lightColor);

	mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(
		glm_rad(player_camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f,
		projectionTransform
	);
	set_uniform_mat4(&shader_texture, "u_projectionTransform", &projectionTransform);

	mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
	camera_get_view_matrix(player_camera, &viewTransform);
	set_uniform_mat4(&shader_texture, "u_viewTransform", &viewTransform);

	glBindVertexArray(VAO);
	for (int i = 0; i < 10; i++) {

		mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
		glm_translate(modelMatrix, cubePositions[i]);

		float angle = 20.0f * (float) i;
		glm_rotate(modelMatrix, glm_rad(game_last_frame() * angle), (vec3) { 1.0f, 0.3f, 0.5f });

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

	glBindVertexArray(lightCubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int elmo_vbo_vao_ebo(
	unsigned int* vertexBuffer,
	unsigned int* vertexArray,
	unsigned int* elementBuffer,
	unsigned int* trianglesSize,
	unsigned int* texture1,
	unsigned int* texture2
) {

	// clang-format off
	float vertices[] = {
		    /* Vertices */       /* Textures */     /* Normals */
		-0.5f, -0.5f, -0.5f,	  0.0f, 0.0f,	  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	  1.0f, 0.0f,	  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f,	  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 0.0f,	  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,	  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,	  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 1.0f,	  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 1.0f,	  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	  0.0f, 1.0f,	  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,	  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,	  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	  1.0f, 1.0f,	  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	  1.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	  0.0f, 0.0f,	  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	  0.0f, 1.0f,	  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f,	  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	  1.0f, 1.0f,	  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	  1.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	  0.0f, 0.0f,	  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	  0.0f, 1.0f,	  0.0f,  1.0f,  0.0f
	};
	// clang-format on

	glGenVertexArrays(1, vertexArray);
	glGenBuffers(1, vertexBuffer);

	glBindVertexArray(*vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (5 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

	if (error1) {
		log_error_f("Error loading image (elmo): %d: %s", error1, lodepng_error_text(error1));
		return -1;
	}

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

	if (error2) {
		log_error_f("Error loading image (obama): %d: %s", error1, lodepng_error_text(error1));
		return -1;
	}

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2
	);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// ============================
// = TODO: Manage differently =
// ============================
