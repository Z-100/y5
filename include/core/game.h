#ifndef GAME_H
#define GAME_H

typedef struct GLFWwindow GLFWwindow;
typedef struct Camera	  Camera;
typedef float			  vec3[3];

typedef struct GuiStats {
	vec3* speed;
	float* fps;
} GuiStats;

typedef struct Game {

	GLFWwindow* main_window;
	Camera*		player_camera;
	GuiStats*	gui_stats;

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
