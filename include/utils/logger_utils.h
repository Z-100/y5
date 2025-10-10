#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

void log_info(char* info_message);
void log_info_f(char* info_message, ...);

void log_error(char* error_message);
void log_error_f(const char* error_message, ...);

#endif
