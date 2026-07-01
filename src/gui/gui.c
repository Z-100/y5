#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <cimgui.h>

#include "bridges/imgui_backends_bridge.h"
#include "gui/gui.h"

#include "utils/collection_hdr.h"

static void _draw_prod_info();
static void _draw_game_info(const game_t* game);
static void _draw_game_spawner(const game_t* game);

float gui_provide_main_scale() {
	GLFWmonitor* glfw_monitor = glfwGetPrimaryMonitor();
	return cimGui_ImplGlfw_GetContentScaleForMonitor(glfw_monitor);
}

gui_t* gui_init_imgui(gui_info_t gui_info) {

	log_info("Start initializing imgui");

	gui_t* gui = malloc(sizeof(gui_t));
	if (!gui) {
		log_error("Failed malloc for gui_t");
		return nullptr;
	}

	igCreateContext(nullptr);

	gui->imgui_io = igGetIO_Nil();
	gui->imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	bool initialized_for_glfw = cimgui_ImplGlfw_InitForOpenGL(gui_info.main_window, true);

	if (!initialized_for_glfw) {
		log_error("Failed initializing imgui for GLFW");
		return nullptr;
	}

	bool initialized_for_opengl = cimgui_ImplOpenGL3_Init("#version 130");

	if (!initialized_for_opengl) {
		log_error("Failed initializing imgui for OpenGL");
		return nullptr;
	}

	igStyleColorsDark(nullptr);

	float main_scale = gui_provide_main_scale();

	ImGuiStyle* style = igGetStyle();
	ImGuiStyle_ScaleAllSizes(style, main_scale);
	style->FontScaleDpi = main_scale;

	log_info("Finish initializing imgui");
	return gui;
}

void gui_terminate_imgui(gui_t* gui) {

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

	ImGuiIO* imgui_io = game->gui->imgui_io;

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

	// TODO: Refactor and idk ??
	if (igButton("Spawn object", size)) {

		// component_group_t grp1 = { .bit_mask = { .render = 1, .transform = 1, .rotation = 1 } };
		// spawn_info_t* spawn1 = spawn_info;
		// spawn1->initial_pos[0] = game->player_camera->position[0];
		// spawn1->initial_pos[1] = game->player_camera->position[1];
		// spawn1->initial_pos[2] = game->player_camera->position[2];
		//
		// spawner_summon(game->ecs_engine, grp1, spawn1);

		log_info_f("Spawning: %s", selected_index != -1 ? elements[selected_index] : "Nothing");
	}

	igEnd();
}
