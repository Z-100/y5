#include "file_reader.h"

#include <stdio.h>
#include <glad/glad.h>

char* readLines(const char* filePath) {

    FILE *filePtr = fopen(filePath, "r");

    if (!filePtr) {
        perror(strcat("File not found: ", filePath));
        return nullptr;
    }

    fseek(filePtr, 0, SEEK_END);
    const int fileLength = ftell(filePtr);
    rewind(filePtr);

    char* fileContentBuffer = calloc(fileLength + 1, sizeof(char));

    if (!fileContentBuffer) {
        perror(strcat("Couldn't calloc memory for: ", filePath));
        return nullptr;
    }

    const size_t bytesRead = fread(fileContentBuffer, 1, fileLength, filePtr);
    fclose(filePtr);

    if (fileLength > bytesRead) fileContentBuffer[bytesRead] = '\0';
    else fileContentBuffer[fileLength] = '\0';

    return fileContentBuffer;
}
