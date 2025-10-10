#include "core/callbacks.h"
#include "utils/headers_collection.h"

void error_callback(int id, const char* msg) {
	log_error_f("Error %d: %s", id, msg);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPosD, double yPosD) {

	float xPos = (float) xPosD;
	float yPos = (float) yPosD;

	Game* game = game_get_game();
	camera_process_mouse_movement(xPos, yPos, game->player_camera);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	Game* game = game_get_game();
	camera_process_mouse_scroll((float) yOffset, game->player_camera);
}
