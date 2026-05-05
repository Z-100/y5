#include "utils/file_reader.h"
#include "utils/headers_collection.h"

char* read_lines_dir_name(char* directory, char* fileName) {
	char* file_path = concat_dir_file(directory, fileName);
	return read_lines_path(file_path);
}

char* read_lines_path(char* filePath) {

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
