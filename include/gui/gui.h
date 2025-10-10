#ifndef GUI_H
#define GUI_H

typedef struct Game Game;

float gui_main_scale();

bool gui_init_imgui(const Game* game);

void gui_terminate_imgui();
void gui_render_imgui();
void gui_update_imgui();

#endif
