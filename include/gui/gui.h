#ifndef GUI_H
#define GUI_H

#include "utils/collection_sys.h"

typedef struct gui_info_t {
	GLFWwindow* main_window;
	float		main_scale;
} gui_info_t;

typedef struct gui_t {
	ImGuiContext* imgui_context;
	ImGuiIO*	  imgui_io;
} gui_t;

// ============================
// Simple one time getters
// > Less include order hell :P
// ============================

float gui_provide_main_scale();

// ========================
// GUI init/rendering cycle
// ========================

gui_t* gui_init_imgui(gui_info_t gui_info);
void   gui_terminate_imgui(gui_t* gui);

void gui_render_imgui();
void gui_update_imgui(const game_t* game);

#endif
