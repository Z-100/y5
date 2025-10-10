#ifndef GAME_H
#define GAME_H

typedef struct GLFWwindow GLFWwindow;
typedef struct Camera	  Camera;

typedef struct Game {

	GLFWwindow* main_window;
	Camera*		player_camera;

	float main_scale;
	float delta_time;

	bool running;
} Game;

bool  game_init();
Game* game_get_game();

void game_update(Game* game);

int	  game_is_running(const Game* game);
float game_last_frame();

#endif
