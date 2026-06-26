#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "utils/collection_sys.h"

typedef struct window_manger_info_t {
	float main_scale;
	char* title;
} window_manger_info_t;

GLFWwindow* window_manager_create_main(window_manger_info_t wm_info);
bool		window_manager_destroy_main(game_t* game);

#endif
