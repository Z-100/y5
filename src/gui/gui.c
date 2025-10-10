#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <cimgui.h>

#include "bridges/imgui_backends_bridge.h"
#include "gui/gui.h"

#include "utils/headers_collection.h"

ImGuiContext* imgui_context;
ImGuiIO*	  imgui_io;

void draw_fps_counter();

float gui_main_scale() {
	return cimGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
}

bool gui_init_imgui(const Game* game) {

	igCreateContext(nullptr);

	imgui_io = igGetIO_Nil();
	imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	bool initialized_for_glfw = cimgui_ImplGlfw_InitForOpenGL(game->main_window, true);

	if (!initialized_for_glfw) {
		log_error("Failed initializing imgui for GLFW");
		return false;
	}

	bool initialized_for_opengl = cimgui_ImplOpenGL3_Init("#version 130");

	if (!initialized_for_opengl) {
		log_error("Failed initializing imgui for OpenGL");
		return false;
	}

	igStyleColorsDark(nullptr);

	float main_scale = game->main_scale;

	ImGuiStyle* style = igGetStyle();
	ImGuiStyle_ScaleAllSizes(style, main_scale);
	style->FontScaleDpi = main_scale;

	return true;
}

void gui_terminate_imgui() {
	cimgui_ImplOpenGL3_Shutdown();
	cimgui_ImplGlfw_Shutdown();
	igDestroyContext(nullptr);
}

void gui_update_imgui() {
	cimgui_ImplOpenGL3_NewFrame();
	cimgui_ImplGlfw_NewFrame();
	igNewFrame();

	// This'll stay for PROD
	igBegin("Fuck yeah", nullptr, 0);
	igText("Fuck you imgui :3");

	draw_fps_counter();

	igEnd();
}

void gui_render_imgui() {
	igRender();
	cimgui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void draw_fps_counter() {

	ImVec2 pos = { 0.0f, 0.0f };
	igSetNextWindowPos(pos, ImGuiCond_Always, (ImVec2) { 0, 0 });

	igBegin("Perförmance", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	igText("FPS: %.1f", imgui_io->Framerate);
	igText("FPS Time: %.3f ms", 1000.0f / imgui_io->Framerate);
	igEnd();
}
