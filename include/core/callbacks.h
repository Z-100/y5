#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "utils/collection_sys.h"

void error_callback(int id, const char* msg);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosD, double yPosD);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

#endif
