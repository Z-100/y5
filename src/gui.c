#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include "cimgui.h"
#include "imgui_backends_bridge.h"
#include <GLFW/glfw3.h>

#include "gui.h"

#define igGetIO igGetIO_Nil

ImGuiContext* imgui_context;
ImGuiIO*	  imgui_io;

void draw_fps_counter();

float gui_scale() {
	return cimGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
}

void gui_init(GLFWwindow* mainWindow) {

	igCreateContext(nullptr);

	imgui_io = igGetIO();
	imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	bool cimgui_impl_glfw_init_for_opengl = cimgui_ImplGlfw_InitForOpenGL(mainWindow, true);

	if (!cimgui_impl_glfw_init_for_opengl) {
		fprintf(stderr, "Failed initializing imgui GLFW backend\n");
		return;
	}

	bool cimgui_impl_opengl3_init = cimgui_ImplOpenGL3_Init("#version 130");

	if (!cimgui_impl_opengl3_init) {
		fprintf(stderr, "Failed initializing imgui OpenGL backend\n");
		return;
	}

	igStyleColorsDark(nullptr);

	float		main_scale = gui_scale();
	ImGuiStyle* style	   = igGetStyle();
	ImGuiStyle_ScaleAllSizes(style, main_scale);
	style->FontScaleDpi = main_scale;
}

void gui_terminate() {
	cimgui_ImplOpenGL3_Shutdown();
	cimgui_ImplGlfw_Shutdown();
	igDestroyContext(nullptr);
}

void gui_update() {
	cimgui_ImplOpenGL3_NewFrame();
	cimgui_ImplGlfw_NewFrame();
	igNewFrame();

	// This'll stay for PROD
	igBegin("Fuck yeah", nullptr, 0);
	igText("Fuck you imgui :3");

	draw_fps_counter();

	igEnd();
}

void gui_render() {
	igRender();
	cimgui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void draw_fps_counter() {

	ImVec2 pos = { 0.0f, 0.0f };
	igSetNextWindowPos(pos, ImGuiCond_Always, (ImVec2){0, 0});

	igBegin("Perförmance", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	igText("FPS: %.1f", imgui_io->Framerate);
	igText("FPS Time: %.3f ms", 1000.0f / imgui_io->Framerate);
	igEnd();
}