#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <cimgui.h>

#include "bridges/imgui_backends_bridge.h"
#include "gui/gui.h"

#include "utils/headers_collection.h"

static ImGuiContext* imgui_context;
static ImGuiIO*		 imgui_io;

static void _draw_prod_info();
static void _draw_game_info(const game_t* game);
static void _draw_game_spawner(const game_t* game);

float gui_main_scale() {
	GLFWmonitor* glfw_monitor = glfwGetPrimaryMonitor();
	return cimGui_ImplGlfw_GetContentScaleForMonitor(glfw_monitor);
}

bool gui_init_imgui(const game_t* game) {

	log_info("Start initializing imgui");

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

	log_info("Finish initializing imgui");
	return true;
}

void gui_terminate_imgui() {

	log_info("Start terminating imgui");

	cimgui_ImplOpenGL3_Shutdown();
	cimgui_ImplGlfw_Shutdown();

	igDestroyContext(nullptr);

	log_info("Finish terminating imgui");
}

void gui_update_imgui(const game_t* game) {

	cimgui_ImplOpenGL3_NewFrame();
	cimgui_ImplGlfw_NewFrame();

	igNewFrame();

	_draw_prod_info();
	_draw_game_info(game);
	_draw_game_spawner(game);

	igEnd();
}

void gui_render_imgui() {

	igRender();

	ImDrawData* draw_data = igGetDrawData();
	cimgui_ImplOpenGL3_RenderDrawData(draw_data);
}

static void _draw_prod_info() {

	ImVec2 pos = { 1280.0f, 0.0f };
	igSetNextWindowPos(pos, ImGuiCond_Always, (ImVec2) { 0, 0 });

	// This'll stay for PROD
	igBegin("Fuck yeah", nullptr, 0);
	igText("Fuck you imgui :3");
}

static void _draw_game_info(const game_t* game) {

	ImVec2 pos = { 0.0f, 0.0f };
	igSetNextWindowPos(pos, ImGuiCond_Always, (ImVec2) { 0, 0 });

	igBegin("Perförmance", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	igText("FPS: %.1f", imgui_io->Framerate);
	igText("TPF: %.3f ms", 1000.0f / imgui_io->Framerate);

	float* position = game->player_camera->position;
	igText("XYZ: [ %.3f, %.3f, %.3f ]", position[0], position[1], position[2]);
	igText("Pitch/Yaw: [ %.3f, %.3f ]", game->player_camera->pitch, game->player_camera->yaw);
	igText("Mouse lock: %d", game->mouse_locked);

	igEnd();
}

static int selected_index = -1;

static void _draw_game_spawner(const game_t* game) {

	ImVec2 pos = { 0.0f, 500.0f };
	igSetNextWindowPos(pos, ImGuiCond_Always, (ImVec2) { 0, 0 });

	igBegin("Spawner", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	const char* elements[] = { "Monkey", "Cube" };

	ImVec2 size = { 0.0f, 0.0f };

	char title[50];
	sprintf(title, "Selected Object: %d", selected_index);

	igCombo_Str_arr(title, &selected_index, elements, 2, 5);

	if (igButton("Spawn object", size)) {
		log_info_f("Spawning: %s", selected_index != -1 ? elements[selected_index] : "Nothing");
	}

	igEnd();
}
