#include "utils/file_reader.h"
#include "utils/headers_collection.h"


char* read_lines_dir_name(const char* directory, const char* fileName) {

	size_t fullPathLength = strlen(directory) + strlen(fileName) + 2;
	char*  fullPath		  = malloc(fullPathLength);

	if (!fullPath) {
		log_error_f("Failed allocating full file name memory for: '%s/&s'", directory, fileName);
		return nullptr;
	}

	snprintf(fullPath, fullPathLength, "%s/%s", directory, fileName);

	return read_lines_path(fullPath);
}

char* read_lines_path(const char* filePath) {

	log_info_f("Start reading: '%s'", filePath);

	FILE* filePtr = fopen(filePath, "r");

	if (!filePtr) {
		log_error_f("File not found: '%s'", filePath);
		return nullptr;
	}

	if (fseek(filePtr, 0, SEEK_END) != 0) {
		log_error_f("Failed finding EOF for: '%s'", filePath);
		fclose(filePtr);
		return nullptr;
	}

	const int fileLength = ftell(filePtr);

	if (fileLength < 0) {
		log_warn_f("Failed allocating file buffer for: '%s'", filePath);
		fclose(filePtr);
		return nullptr;
	}

	rewind(filePtr);

	char* fileContentBuffer = malloc(fileLength + 1);

	if (!fileContentBuffer) {
		log_error_f("Failed allocating file buffer for: '%s'", filePath);
		fclose(filePtr);
		return nullptr;
	}

	const size_t bytesRead = fread(fileContentBuffer, 1, fileLength, filePtr);

	if (bytesRead < (size_t) fileLength && ferror(filePtr)) {
		log_error_f("Failed reading file: '%s'", filePath);
		fclose(filePtr);
		free(fileContentBuffer);
		return nullptr;
	}

	fileContentBuffer[bytesRead] = '\0';

	fclose(filePtr);
	log_info_f("Finish reading: '%s'", filePath);

	return fileContentBuffer;
}
