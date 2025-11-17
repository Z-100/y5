#include "utils/logger_utils.h"
#include "utils/headers_collection.h"

#include <time.h>

const char* time_type_location_message = "%s - %s - %s: <%s>";

char* get_date_time() {

	time_t	   current_time = time(nullptr);
	struct tm* time_info	= localtime(&current_time);

	char* date_time_str = malloc(sizeof(char) * 100);

	if (!date_time_str || !time_info) {
		fprintf(stderr, "Error getting logger date_time");
		return nullptr;
	}

	strftime(date_time_str, 100, "%Y-%m-%d %H:%M:%S", time_info);
	return date_time_str;
}

void _log(int log_type, char* file, int line, char* message) {
	_log_f(log_type, file, line, message, nullptr);
}

void _log_f(int log_type, char* file, int line, char* message, ...) {

	char* date_time = get_date_time();
	if (!date_time)
		date_time = "0000-00-00 00:00:00";

	char* type;
	switch (log_type) {
		case LOG_INFO:
			type = "INFO";
			break;
		case LOG_DEBUG:
			type = "DEBUG";
			break;
		case LOG_WARN:
			type = "WARN";
			break;
		case LOG_ERROR:
			type = "ERROR";
			break;
		default:
			type = "UNKNOWN";
			break;
	}

	char location[256];
	snprintf(location, sizeof(location), "%s:%d - ", file ? file : "unknown", line);

	char formatted_log_msg[1024];
	snprintf(
		formatted_log_msg, sizeof(formatted_log_msg), time_type_location_message, date_time, type,
		location, message ? message : ""
	);

	va_list args;
	va_start(args, message);
	vfprintf(stdout, formatted_log_msg, args);
	va_end(args);

	fprintf(stdout, "\n");
	fflush(stdout);

	free(date_time);
}
