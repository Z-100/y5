#include "shader_loader.h"
#include "file_reader.h"
#include "types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const char* SHADERS_DIRECTORY = "res/shaders";

unsigned int compile_shader(const struct Shader* shader);
unsigned int build_shader_program(struct Shader* shaders, unsigned int shadersLength);

unsigned int compile_shaders_to_shader_program(struct Array* shadersArray) {

	if (shadersArray == NULL) {
		fprintf(stderr, "Cannot compile no shaders\n");
		return 0;
	}

	struct Shader* shaders = shadersArray->data;

	for (int i = 0; i < shadersArray->length; i++) {

		struct Shader* shader = &shaders[i];

		if (shader->name == NULL || shader->type == 0) {
			fprintf(
				stderr, "Cannot compile incomplete shader config!Provided: name:%s, type:%d\n",
				shader->name, shader->type
			);
			continue;
		}

		unsigned int shaderId = compile_shader(shader);

		if (shaderId != 0)
			shader->id = shaderId;
	}

	unsigned int shaderProgram = build_shader_program(shaders, shadersArray->length);

	free_array(shadersArray);

	return shaderProgram;
}

// ================
// Helper functions
// ================

unsigned int compile_shader(const struct Shader* shader) {

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
		fprintf(stderr, "Error compiling shader: %s\nDetail: %s\n", shader->name, logMsg);
		return 0;
	}

	return shaderID;
}

unsigned int build_shader_program(struct Shader* shaders, unsigned int shadersLength) {

	int	 success;
	char logMsg[512];

	unsigned int shaderProgram = glCreateProgram();

	for (int i = 0; i < shadersLength; i++) {
		struct Shader* shader = &shaders[i];
		if (shader->id != 0)
			glAttachShader(shaderProgram, shader->id);
	}

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, nullptr, logMsg);
		fprintf(stderr, "Error linking shader: %s\n", logMsg);
	}

	for (int i = 0; i < 2; i++) {
		struct Shader* shader = &shaders[i];
		if (shader->id != 0)
			glDeleteShader(shader->id);
	}

	return shaderProgram;
}

// ====================
// Direct shader access
// ====================

void use_shader(const unsigned int* shaderPtr) {
	glUseProgram(*shaderPtr);
}

// ======================================================
// Macros for cheap method overloading? idk but they cool
// ======================================================

#define SET_UNIFORM_BASE(C_TYPE, GL_UNIFORM_FUN, GL_TYPE) \
	int set_uniform_##C_TYPE(const unsigned int* shaderPtr, const char* name, GL_TYPE value) { \
		int locationInShader = glGetUniformLocation(*shaderPtr, name); \
		if (locationInShader == -1) { \
			fprintf(stderr, "Uniform '%s' not found in shader:%d\n", name, *shaderPtr); \
			return -1; \
		} \
		GL_UNIFORM_FUN(locationInShader, value); \
		return 0; \
	}

#define SET_UNIFORM_VEC(C_TYPE, GL_UNIFORM_FUN, VALUE_TYPE, VEC_LEN) \
	int set_uniform_##C_TYPE(const unsigned int* shaderPtr, const char* name, VALUE_TYPE* value) { \
		int locationInShader = glGetUniformLocation(*shaderPtr, name); \
		if (locationInShader == -1) { \
			fprintf(stderr, "Uniform '%s' not found in shader:%d\n", name, *shaderPtr); \
			return -1; \
		} \
		/* clang-format off */ \
		GL_UNIFORM_FUN( \
			locationInShader, \
			/* OpenGL doesn't care abt source vec4 if target is vec2. */ \
			/* Maybe I'll run into problems in the future tho, who knows */ \
			VEC_LEN >= 1 ? value[0] : 0.0f, \
			VEC_LEN >= 2 ? value[1] : 0.0f, \
			VEC_LEN >= 3 ? value[2] : 0.0f, \
			VEC_LEN >= 4 ? value[2] : 0.0f \
		); \
		/* clang-format on */ \
		return 0; \
	}

SET_UNIFORM_BASE(bool, glUniform1i, int)
SET_UNIFORM_BASE(int, glUniform1i, int)
SET_UNIFORM_BASE(float, glUniform1f, float)
SET_UNIFORM_BASE(double, glUniform1d, double)

SET_UNIFORM_VEC(vec4, glUniform4f, float, 4)
SET_UNIFORM_VEC(vec3, glUniform4f, float, 3)
