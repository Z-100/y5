#include "core/game.h"

#include "utils/headers_collection.h"

Game* game;
float last_frame = 0;

bool game_init() {

	game = malloc(sizeof(Game));

	if (!game) {
		log_error("Failed allocating memory");
		return false;
	}

	camera_create_player_camera(game);

	return true;
}

Game* game_get_game() {
	return game;
}

void game_update(Game* game) {
	float current_time = (float) glfwGetTime();
	game->delta_time   = current_time - last_frame;
	last_frame		   = current_time;
}

int game_is_running(const Game* game) {
	return !glfwWindowShouldClose(game->main_window);
}

float game_last_frame() {
	return last_frame;
}
