#include "utils/string_utils.h"
#include "utils/collection_hdr.h"

char* concat_dir_file(char* dir, char* file) {

	size_t full_len = strlen(dir) + strlen(file) + 2;
	char*  path		= malloc(full_len);

	if (!path) {
		log_error_f("Failed allocating full file name memory for: '%s/&s'", dir, file);
		return nullptr;
	}

	snprintf(path, full_len, "%s/%s", dir, file);

	return path;
}
