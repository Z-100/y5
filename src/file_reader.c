#include "file_reader.h"

#include <glad/glad.h>
#include <stdio.h>

char* readLines(const char* filePath) {

	fprintf(stdout, "Start reading %s\n", filePath);

	FILE* filePtr = fopen(filePath, "r");

	if (!filePtr) {
		fprintf(stderr, "File not found: %s\n", filePath);
		return nullptr;
	}

	if (fseek(filePtr, 0, SEEK_END) != 0) {
		fprintf(stderr, "Failed to seek to end of file\n");
		fclose(filePtr);
		return nullptr;
	}

	const int fileLength = ftell(filePtr);

	if (fileLength < 0) {
		fprintf(stderr, "File is empty\n");
		fclose(filePtr);
		return nullptr;
	}

	rewind(filePtr);

	char* fileContentBuffer = malloc(fileLength + 1);

	if (!fileContentBuffer) {
		fclose(filePtr);
		fprintf(stderr, "Calloc failed\n");
		return nullptr;
	}

	const size_t bytesRead = fread(fileContentBuffer, 1, fileLength, filePtr);

	if (bytesRead < (size_t) fileLength && ferror(filePtr)) {
		fprintf(stderr, "Failed to read from file\n");
		fclose(filePtr);
		free(fileContentBuffer);
		return nullptr;
	}

	fileContentBuffer[bytesRead] = '\0';

	fclose(filePtr);
	fprintf(stdout, "Finish reading %s\n", filePath);

	return fileContentBuffer;
}
