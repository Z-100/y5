#ifndef GAME_H
#define GAME_H

#include "utils/collection_sys.h"

typedef struct game_t {

	GLFWwindow* main_window;
	camera_t*		player_camera;

	ecs_engine_t* ecs_engine;

	float main_scale;
	float delta_time;
	bool  mouse_locked;

	bool running;
} game_t;

bool  game_init();
game_t* game_get_game();

void game_update();

int	  game_is_running();
float game_last_frame();

#endif
