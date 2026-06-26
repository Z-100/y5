#include "graphics/renderer.h"
#include "utils/collection_hdr.h"

static void _initialize_texture(char* texture_path, unsigned int* texture);
static void _initialize_object(gl_object_t* gl_object);

constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

static GLuint* texture_ids;
static size_t  texture_ids_count = 0;

static gl_object_t** gl_objects;
static size_t		 gl_objects_count = 0;

static shader_program_t** shader_programs;
static size_t			  shader_programs_count = 0;

// =================
// General rendering
// =================

void renderer_init(const game_t* game) {

	texture_ids		= malloc(sizeof(GLuint) * 100);
	gl_objects		= malloc(sizeof(model_object_t*) * 100);
	shader_programs = malloc(sizeof(shader_program_t) * NUM_SHADER_PROGRAMS);

	if (!texture_ids || !gl_objects || !shader_programs) {
		log_error("Failed allocating renderer arrays!");
		return;
	}

	glfwSwapInterval(1); // le vsync
	glEnable(GL_DEPTH_TEST);
}

void renderer_update(const game_t* game) {
	glfwSwapBuffers(game->main_window);
	glfwPollEvents();
}

void renderer_destroy() {

	free(texture_ids);

	for (int i = 0; i < gl_objects_count; i++) {
		gl_object_t* object = gl_objects[i];
		glDeleteVertexArrays(1, &object->vao);
		glDeleteBuffers(1, &object->vbo);
		free(object);
	}

	for (int i = 0; i < shader_programs_count; i++) {
		glDeleteProgram(shader_programs[i]->id);
	}

	free(shader_programs);
}

void renderer_draw(uint32_t model_id) {

	gl_object_t* gl_object = gl_objects[model_id];

	glBindVertexArray(gl_object->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_object->ebo);
	glDrawElements(GL_TRIANGLES, gl_object->model_object->index_count, GL_UNSIGNED_INT, nullptr);
}

// =================
// Loading/unloading
// =================

void renderer_load_shader(shader_program_t* shader_program) {

	bool success_compile = shader_loader_compile(shader_program);
	if (!success_compile) {
		log_error("Failed compiling shader program!");
		return;
	}

	GLuint			  shader_program_id = shader_program->id;
	shader_texture_t* shader_textures	= shader_program->textures;

	shader_programs[shader_programs_count++] = shader_program;

	for (int i = 0; i < shader_program->textures_count; i++) {

		shader_texture_t* texture = &shader_textures[i];

		GLuint texture_id = texture_ids[texture_ids_count++];
		texture_id		  = -1;

		_initialize_texture(texture->texture_name, &texture_id);

		texture->id = texture_id;

		use_shader(&shader_program_id);
		set_uniform_int(&shader_program_id, texture->uniform_name, texture_id);

		log_info_f(
			"Bound texture '%s' to uniform '%s' in shader_program:%d", texture->texture_name,
			texture->uniform_name, texture_id
		);
	}
}

uint8_t renderer_load_model(model_object_t* model_object) {

	gl_object_t* gl_object = malloc(sizeof(gl_object_t));
	if (!gl_object) {
		log_error("Failed allocating gl_object for model!");
		return -1;
	}

	uint8_t gl_object_id		   = gl_objects_count;
	gl_objects[gl_objects_count++] = gl_object;

	gl_object->model_object = model_object;
	_initialize_object(gl_object);

	return gl_object_id;
}

// ==================
// Internal functions
// ==================

static void _initialize_object(gl_object_t* gl_object) {

	model_object_t* model = gl_object->model_object;
	GLuint*			vbo	  = &gl_object->vbo;
	GLuint*			vao	  = &gl_object->vao;
	GLuint*			ebo	  = &gl_object->ebo;

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
