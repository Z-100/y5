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

void set_uniform_bool(const unsigned int* shaderPtr, const char* name, bool value) {
	glUniform1i(glGetUniformLocation(*shaderPtr, name), (int) value);
}

void set_uniform_int(const unsigned int* shaderPtr, const char* name, int value) {
	glUniform1i(glGetUniformLocation(*shaderPtr, name), value);
}

void set_uniform_float(const unsigned int* shaderPtr, const char* name, float value) {
	glUniform1f(glGetUniformLocation(*shaderPtr, name), value);
}
