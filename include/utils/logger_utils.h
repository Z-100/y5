#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_INFO 0
#define LOG_DEBUG 1
#define LOG_WARN 2
#define LOG_ERROR 3

void log_internal(int log_type, const char* file, int line, const char* message);
void log_internal_f(int log_type, const char* file, int line, const char* message, ...);

#ifdef __cplusplus
}
#endif

#define log_info(msg) log_internal(LOG_INFO, __FILE__, __LINE__, msg)
#define log_debug(msg) log_internal(LOG_DEBUG, __FILE__, __LINE__, msg)
#define log_warn(msg) log_internal(LOG_WARN, __FILE__, __LINE__, msg)
#define log_error(msg) log_internal(LOG_ERROR, __FILE__, __LINE__, msg)

#define log_info_f(msg, ...) log_internal_f(LOG_INFO, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_debug_f(msg, ...) log_internal_f(LOG_DEBUG, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_warn_f(msg, ...) log_internal_f(LOG_WARN, __FILE__, __LINE__, msg, __VA_ARGS__)
#define log_error_f(msg, ...) log_internal_f(LOG_ERROR, __FILE__, __LINE__, msg, __VA_ARGS__)

#endif
