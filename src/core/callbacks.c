#include "core/callbacks.h"
#include "utils/headers_collection.h"

void error_callback(int id, const char* msg) {
	log_error_f("Error %d: %s", id, msg);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

static bool first_mouse = true;
void mouse_callback(GLFWwindow* window, double xPosD, double yPosD) {

	Game* game = game_get_game();
	Camera* camera = game->player_camera;

	// No camera movement if not locked
	if (!game->mouse_locked) {
		first_mouse = true;
		return;
	}

	float x_pos = (float) xPosD;
	float y_pos = (float) yPosD;

	if (first_mouse) {
		camera->last_x = x_pos;
		camera->last_y = y_pos;
		first_mouse = false;
	}

	camera_process_mouse_movement(x_pos, y_pos, camera);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	Game* game = game_get_game();
	camera_process_mouse_scroll((float) yOffset, game->player_camera);
}
