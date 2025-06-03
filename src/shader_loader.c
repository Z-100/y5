#include "shader_loader.h"

#include <stdio.h>
#include <glad/glad.h>

char* loadShader(const char* shaderName) {

    FILE *shaderFile = fopen(shaderName, "r");

    if (!shaderFile) {
        perror(strcat("File not found: ", shaderName));
        return nullptr;
    }

    fseek(shaderFile, 0, SEEK_END);
    const int fileLength = ftell(shaderFile);
    rewind(shaderFile);

    char* fileContentBuffer = calloc(fileLength + 1, sizeof(char));

    if (!fileContentBuffer) {
        perror(strcat("Couldn't calloc memory for: ", shaderName));
        return nullptr;
    }

    const size_t bytesRead = fread(fileContentBuffer, 1, fileLength, shaderFile);
    fclose(shaderFile);

    if (fileLength > bytesRead) fileContentBuffer[bytesRead] = '\0';
    else fileContentBuffer[fileLength] = '\0';

    return fileContentBuffer;
}
