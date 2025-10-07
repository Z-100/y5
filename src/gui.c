#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>

#include "gui.h"

#define igGetIO igGetIO_Nil

ImGuiContext* ctx;
ImGuiIO*	  io;

void gui_init(GLFWwindow* mainWindow) {

	igCreateContext(nullptr);

	io = igGetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGuiStyle* style = igGetStyle();

	igStyleColorsDark(nullptr);

	igNewFrame();
}

void gui_terminate() {
	igDestroyContext(ctx);
}

void gui_update() {
}

void gui_render() {
	igRender();
}

