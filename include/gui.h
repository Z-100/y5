#ifndef GUI_H
#define GUI_H

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "GLFW/glfw3.h"

void gui_init(GLFWwindow* mainWindow);
void gui_terminate();
void gui_render();
void gui_update();

#endif
