#ifndef GAME_H
#define GAME_H

#include "utils/collection_sys.h"

typedef struct game_t {

	// Engine globals
	GLFWwindow*		 main_window;
	camera_t*		 player_camera;
	ecs_engine_t*	 ecs_engine;
	audio_manager_t* audio_manager;
	gui_t*			 gui;

	// idk maybe?
	float delta_time;
	float last_frame;

	// General globals
	float main_scale;
	bool  mouse_locked;
	bool  running;

} game_t;

game_t* game_init();
void	game_update(game_t* game);
int		game_is_running(const game_t* game);

#endif
