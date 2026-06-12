#ifndef GUI_H
#define GUI_H

#include "utils/collection_sys.h"

float gui_main_scale();

bool gui_init_imgui(const game_t* game);

void gui_terminate_imgui();
void gui_render_imgui();
void gui_update_imgui(const game_t* game);

#endif
