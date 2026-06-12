#include "graphics/shader_loader.h"
#include "utils/headers_collection.h"

const char* SHADERS_DIRECTORY = "res/shaders";

static GLuint _compile_shader(const shader_t* shader);
static void	  _link_shaders(shader_program_t* shader_program);

bool shader_loader_compile(shader_program_t* shader_program) {

	if (shader_program == nullptr || shader_program->shaders == nullptr) {
		log_error("No shaders provided");
		return false;
	}

	if (shader_program->id != -1) {
		log_error_f("Shader program:%d already compiled", shader_program->id);
		return false;
	}

	shader_t* shaders = shader_program->shaders;

	for (int i = 0; i < shader_program->shaders_count; i++) {

		shader_t* shader = &shaders[i];

		if (shader->name == nullptr || shader->type == 0) {
			log_error_f("Incomplete shader config: '%s' (type: %d)", shader->name, shader->type);
			continue;
		}

		GLuint shader_id = _compile_shader(shader);

		if (shader_id != 0) {
			shader->id = shader_id;
		}
	}

	_link_shaders(shader_program);
	return true;
}

// ================
// Helper functions
// ================

static GLuint _compile_shader(const shader_t* shader) {

	int	 success;
	char logMsg[512];

	char* shaderContentBufferPtr = read_lines_dir_name(SHADERS_DIRECTORY, shader->name);

	const unsigned int shaderID = glCreateShader(shader->type);
	glShaderSource(shaderID, 1, (const GLchar**) &shaderContentBufferPtr, nullptr);
	glCompileShader(shaderID);

	free(shaderContentBufferPtr);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderID, 512, nullptr, logMsg);
		log_error_f("Error compiling shader '%s': %s", shader->name, logMsg);
		return 0;
	}

	return shaderID;
}

static void _link_shaders(shader_program_t* shader_program) {

	int	 success;
	char logMsg[512];

	GLuint shader_prog_id = glCreateProgram();

	shader_program->id		 = shader_prog_id;
	GLuint shader_program_id = shader_program->id;

	shader_t* shaders = shader_program->shaders;

	for (int i = 0; i < shader_program->shaders_count; i++) {

		shader_t* shader = &shaders[i];

		if (shader->id != 0) {
			glAttachShader(shader_program_id, shader->id);
		}
	}

	glLinkProgram(shader_program_id);

	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader_program_id, 512, nullptr, logMsg);
		log_error_f("Error linking shaders: %s", logMsg);
	}

	for (int i = 0; i < 2; i++) {

		shader_t* shader = &shaders[i];

		if (shader->id != 0) {
			glDeleteShader(shader->id);
		}
	}
}

// ====================
// Direct shader access
// ====================

void use_shader(const unsigned int* shaderPtr) {
	glUseProgram(*shaderPtr);
}

int set_uniform_vec3(const unsigned int* shaderPtr, const char* name, const vec3* value) {

	GLint locationInShader = glGetUniformLocation(*shaderPtr, name);

	if (locationInShader == -1) {
		log_error_f("Uniform (vec3) '%s' not found in shader:%d", name, *shaderPtr);
		return -1;
	}

	glUniform3fv(locationInShader, 1, *value);
	return 0;
}

int set_uniform_vec4(const unsigned int* shaderPtr, const char* name, const vec4* value) {

	GLint locationInShader = glGetUniformLocation(*shaderPtr, name);

	if (locationInShader == -1) {
		log_error_f("Uniform (vec4) '%s' not found in shader:%d", name, *shaderPtr);
		return -1;
	}

	glUniform4fv(locationInShader, 1, *value);
	return 0;
}

int set_uniform_mat4(const unsigned int* shaderPtr, const char* name, const mat4* matrix) {

	GLint locationInShader = glGetUniformLocation(*shaderPtr, name);

	if (locationInShader == -1) {
		log_error_f("Uniform (mat4) '%s' not found in shader:%d", name, *shaderPtr);
		return -1;
	}

	glUniformMatrix4fv(locationInShader, 1, GL_FALSE, (const GLfloat*) matrix[0][0]);
	return 0;
}

int set_uniform_material(
	const unsigned int* shaderPtr,
	const char*			name,
	const material_t*	material
) {

	// Should be enough
	char uniform_name[64];

	sprintf(uniform_name, "%s.%s", name, "ambient");
	int ambient_set = set_uniform_vec3(shaderPtr, uniform_name, &material->ambient);

	sprintf(uniform_name, "%s.%s", name, "diffuse");
	int diffuse_set = set_uniform_vec3(shaderPtr, uniform_name, &material->diffuse);

	sprintf(uniform_name, "%s.%s", name, "specular");
	int specular_set = set_uniform_vec3(shaderPtr, uniform_name, &material->specular);

	sprintf(uniform_name, "%s.%s", name, "shininess");
	int shininess_set = set_uniform_float(shaderPtr, uniform_name, material->shininess);

	return ambient_set + diffuse_set + specular_set + shininess_set;
}

// ======================================================
// Macros for cheap method overloading? idk but they cool
// ======================================================

#define SET_UNIFORM_PRIMITIVE(C_TYPE, GL_UNIFORM_FUN, GL_TYPE) \
	int set_uniform_##C_TYPE(const unsigned int* shaderPtr, const char* name, GL_TYPE value) { \
		int locationInShader = glGetUniformLocation(*shaderPtr, name); \
		if (locationInShader == -1) { \
			log_error_f("Uniform (primitive) '%s' not found in shader:%d", name, *shaderPtr); \
			return -1; \
		} \
		GL_UNIFORM_FUN(locationInShader, value); \
		return 0; \
	}

SET_UNIFORM_PRIMITIVE(bool, glUniform1i, int)
SET_UNIFORM_PRIMITIVE(int, glUniform1i, int)
SET_UNIFORM_PRIMITIVE(float, glUniform1f, float)
SET_UNIFORM_PRIMITIVE(double, glUniform1d, double)
