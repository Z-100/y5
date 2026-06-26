#include "core/game.h"
#include "utils/collection_hdr.h"

game_t* game_init() {

	game_t* game = malloc(sizeof(game_t));

	if (!game) {
		log_error("Failed allocating for game");
		return nullptr;
	}

	// ----------------------------
	// Create required game objects
	// ----------------------------

	game->player_camera = camera_create_player_camera();

	window_manger_info_t wm_info = { .main_scale = game->main_scale, .title = "y5 Engine" };
	game->main_window			 = window_manager_create_main(wm_info);
	glfwSetWindowUserPointer(game->main_window, game); // Now available from window too :)

	gui_info_t gui_info = { .main_window = game->main_window, .main_scale = game->main_scale };
	game->gui			= gui_init_imgui(gui_info);

	game->audio_manager = init_audio_manager();

	game->ecs_engine = ecs_engine_init();

	// ---------------
	// Default options
	// ---------------

	game->main_scale   = gui_provide_main_scale();
	game->mouse_locked = true;
	game->running	   = true;

	return game;
}

void game_update(game_t* game) {

	float current_time = (float) glfwGetTime();
	game->delta_time   = current_time - game->last_frame;
	game->last_frame   = current_time;
}

int game_is_running(const game_t* game) {
	return !glfwWindowShouldClose(game->main_window);
}
