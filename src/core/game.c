#include "core/game.h"
#include "utils/headers_collection.h"

float last_frame = 0;

static game_t* game = nullptr;

bool game_init() {

	game = malloc(sizeof(game_t));

	if (!game) {
		log_error("Failed allocating for game");
		return false;
	}

	camera_create_player_camera(game);

	return true;
}

game_t* game_get_game() {
	return game;
}

void game_update() {
	float current_time = (float) glfwGetTime();
	game->delta_time   = current_time - last_frame;
	last_frame		   = current_time;
}

int game_is_running() {
	return !glfwWindowShouldClose(game->main_window);
}

float game_last_frame() {
	return last_frame;
}
