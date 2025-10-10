#include "utils/headers_collection.h"

const char* APP_NAME = "y5";
void		process_inputs(Game* game);

int main() {

	log_info_f("Starting %s application", APP_NAME);

	bool game_initialized = game_init();
	Game* game = game_get_game();

	if (!game_initialized || !game)
		return -1;

	bool window_created = window_manager_create_main(game);

	if (!window_created)
		return -1;

	bool gui_created = gui_init_imgui(game);

	if (!gui_created)
		return -1;

	renderer_init_default(game);

	renderer_initialize_cubes();

	while (game_is_running(game)) {

		game_update(game);
		process_inputs(game);

		gui_update_imgui();

		renderer_game_loop(game);

		gui_render_imgui();

		renderer_update(game);
	}

	gui_terminate_imgui();
	renderer_destroy();
	window_manager_destroy_main(game);

	return 0;
}

void process_inputs(Game* game) {

	GLFWwindow* main_window = game->main_window;

	// ---------------
	// Close da window
	// ---------------

	if (glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(main_window, true);

	// ----------------
	// Process movemint
	// ----------------

	if (glfwGetKey(main_window, GLFW_KEY_W) == GLFW_PRESS)
		camera_process_keyboard(game, FORWARD);

	if (glfwGetKey(main_window, GLFW_KEY_A) == GLFW_PRESS)
		camera_process_keyboard(game, LEFT);

	if (glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS)
		camera_process_keyboard(game, BACKWARD);

	if (glfwGetKey(main_window, GLFW_KEY_D) == GLFW_PRESS)
		camera_process_keyboard(game, RIGHT);

	if (glfwGetKey(main_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera_process_keyboard(game, UP);

	if (glfwGetKey(main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera_process_keyboard(game, DOWN);
}
