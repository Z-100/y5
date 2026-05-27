#include "graphics/renderer.h"
#include "utils/headers_collection.h"

static void _initialize_object(ModelObject* model, component_renderer_t* renderer);

static void _initialize_texture(char* texture_path, unsigned int* texture);

// clang-format off
Light light = {
	.position = { 0.0f, 5.0f, 0.0f },
	.ambient =  { 0.8f, 0.8f, 0.8f },
	.diffuse =  { 0.5f, 0.5f, 0.5f },
	.specular = { 1.0f, 1.0f, 1.0f }
};
// clang-format on

static unsigned int shader_texture, shader_light_source;

constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

static GLuint* textures;
static uint8_t textures_counter = 0;

static component_renderer_t** render_components;
static uint8_t				  num_render_components = 0;

// TODO: Make better
static ModelObject** models;
static int			 models_counter = 0;

// ============================
// = Renderer Base Actions    =
// ============================

void renderer_init(const Game* game) {

	textures = malloc(sizeof(GLuint) * 100);
	models	 = malloc(sizeof(ModelObject*) * 100);

	render_components = malloc(sizeof(component_renderer_t*) * 100);

	if (!textures || !models || !render_components) {
		log_error("Failed allocating renderer arrays!");
		return;
	}

	glfwSwapInterval(1); // le vsync
	glEnable(GL_DEPTH_TEST);
}

void renderer_update(const Game* game) {
	glfwSwapBuffers(game->main_window);
	glfwPollEvents();
}

void renderer_destroy() {

	for (int i = 0; i < num_render_components; i++) {
		component_renderer_t* component = render_components[i];
		glDeleteVertexArrays(1, &component->vao);
		glDeleteBuffers(1, &component->vbo);
		free(component);
	}

	glDeleteProgram(shader_texture);

	free(render_components);
	free(textures);
	free(models);
}

// ============================
// = Renderer Loading         =
// ============================

void renderer_init_model(ModelObject* model) {

	models[models_counter++] = model;

	component_renderer_t* renderer = malloc(sizeof(component_renderer_t));
	renderer->material			   = materials_black_rubber();

	_initialize_object(model, renderer);

	render_components[num_render_components++] = renderer;
}

void renderer_init_shaders() {

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

	GLuint texture = textures[textures_counter++];
	texture		   = 0;
	_initialize_texture("res/textures/elmo.png", &texture);

	texture = textures[textures_counter++];
	texture = 0;
	_initialize_texture("res/textures/obama.png", &texture);

	use_shader(&shader_texture);
	set_uniform_int(&shader_texture, "u_elmoTexture", 0);
	set_uniform_int(&shader_texture, "u_obamaTexture", 1);
}

// ============================
// = Renderer Game Loop       =
// ============================

void renderer_game_loop(const Game* game) {

	Camera* player_camera = game->player_camera;

	light.position[0] = 7.5f * sinf(game_last_frame());
	light.position[2] = 7.5f * cosf(game_last_frame());

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	use_shader(&shader_texture);
	set_uniform_vec3(&shader_texture, "u_viewPos", &player_camera->position);

	set_uniform_vec3(&shader_texture, "u_light.position", &light.position);
	set_uniform_vec3(&shader_texture, "u_light.ambient", &light.ambient);
	set_uniform_vec3(&shader_texture, "u_light.diffuse", &light.diffuse);
	set_uniform_vec3(&shader_texture, "u_light.specular", &light.specular);

	mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(
		glm_rad(player_camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f,
		projectionTransform
	);
	set_uniform_mat4(&shader_texture, "u_projectionTransform", &projectionTransform);

	mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
	camera_get_view_matrix(player_camera, &viewTransform);
	set_uniform_mat4(&shader_texture, "u_viewTransform", &viewTransform);

	for (int i = 0; i < num_render_components; i++) {

		Material* material = render_components[i]->material;
		set_uniform_material(&shader_texture, "u_material", material);

		mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;

		float angle = 20.0f * (float) i;
		glm_rotate(modelMatrix, glm_rad(game_last_frame() * angle), (vec3) { 1.0f, 0.3f, 0.5f });

		set_uniform_mat4(&shader_texture, "u_modelTransform", &modelMatrix);

		ModelObject* model = models[i];

		glBindVertexArray(render_components[i]->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_components[i]->ebo);
		glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, nullptr);
	}

	use_shader(&shader_light_source);
	set_uniform_vec3(&shader_light_source, "u_lightSourceColor", &light.ambient);

	set_uniform_mat4(&shader_light_source, "u_projectionTransform", &projectionTransform);
	set_uniform_mat4(&shader_light_source, "u_viewTransform", &viewTransform);

	mat4 modelTransform = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelTransform, light.position);

	set_uniform_mat4(&shader_light_source, "u_modelTransform", &modelTransform);

	glBindVertexArray(render_components[1]->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_components[1]->ebo);
	glDrawElements(GL_TRIANGLES, models[1]->index_count, GL_UNSIGNED_INT, nullptr);
}

// ============================
// = Renderer Internal Func   =
// ============================

static void _initialize_object(ModelObject* model, component_renderer_t* renderer) {

	GLuint* vbo = &renderer->vbo;
	GLuint* vao = &renderer->vao;
	GLuint* ebo = &renderer->ebo;

	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	glGenBuffers(1, ebo);

	glBindVertexArray(*vao);

	// model->vertex_count is the number of floats now
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(real_t) * model->vertex_count, model->vertices, GL_STATIC_DRAW
	);

	// Upload index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model->index_count, model->indices,
		GL_STATIC_DRAW
	);

	// Updated stride: 8 floats per-vertex: pos(3), tex(2), normal(3)
	constexpr GLsizei stride = 8 * sizeof(real_t);

	// pos (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
	glEnableVertexAttribArray(0);

	// tex (x, y)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(real_t)));
	glEnableVertexAttribArray(1);

	// norm (x, y, z)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(real_t)));
	glEnableVertexAttribArray(2);
}

static void _initialize_texture(char* texture_path, unsigned int* texture) {

	unsigned char* image_data = nullptr;
	unsigned	   width, height;

	unsigned error = lodepng_decode32_file(&image_data, &width, &height, texture_path);
	if (error) {
		log_error_f(
			"Error loading image (%s): %d: %s", texture_path, error, lodepng_error_text(error)
		);
		return;
	}

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data
	);

	glGenerateMipmap(GL_TEXTURE_2D);
}
