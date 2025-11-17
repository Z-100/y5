#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#define LOG_INFO 0
#define LOG_DEBUG 1
#define LOG_WARN 2
#define LOG_ERROR 3

void _log(int log_type, char* file, int line, char* message);
void _log_f(int log_type, char* file, int line, char* message, ...);

#define log_info(msg) _log(LOG_INFO, __FILE__, __LINE__, msg)
#define log_debug(msg) _log(LOG_INFO, __FILE__, __LINE__, msg)
#define log_warn(msg) _log(LOG_INFO, __FILE__, __LINE__, msg)
#define log_error(msg) _log(LOG_INFO, __FILE__, __LINE__, msg)

#define log_info_f(msg, ...) _log_f(LOG_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_debug_f(msg, ...) _log_f(LOG_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_warn_f(msg, ...) _log_f(LOG_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_error_f(msg, ...) _log_f(LOG_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)

#endif
