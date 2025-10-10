#include "utils/logger_utils.h"
#include "utils/headers_collection.h"

void log_info(char* info_message) {
	fprintf(stdout, "%s\n", info_message);
	fflush(stdout);
}

void log_info_f(char* info_message, ...) {
	va_list args;
	va_start(args, info_message);
	vfprintf(stdout, info_message, args);
	fprintf(stdout, "\n");
	va_end(args);
	fflush(stdout);
}

void log_error(char* error_message) {
	fprintf(stderr, "%s\n", error_message);
	fflush(stderr);
}

void log_error_f(const char* error_message, ...) {
	va_list args;
	va_start(args, error_message);
	vfprintf(stderr, error_message, args);
	fprintf(stderr, "\n");
	va_end(args);
	fflush(stderr);
}
