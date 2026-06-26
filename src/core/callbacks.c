#include "core/callbacks.h"
#include "utils/collection_hdr.h"

void error_callback(int id, const char* msg) {
	log_error_f("Error %d: %s", id, msg);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

static bool first_mouse = true;

void mouse_callback(GLFWwindow* window, double xPosD, double yPosD) {

	game_t*	  game	 = glfwGetWindowUserPointer(window);
	camera_t* camera = game->player_camera;

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
		first_mouse	   = false;
	}

	camera_process_mouse_movement(x_pos, y_pos, camera);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	game_t* game = glfwGetWindowUserPointer(window);
	camera_process_mouse_scroll((float) yOffset, game->player_camera);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		game_t* game = glfwGetWindowUserPointer(window);

		// Flip the state
		game->mouse_locked = !game->mouse_locked;

		// Apply the cursor mode change
		int cursor_mode = game->mouse_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		game_t* game = glfwGetWindowUserPointer(window);
		play_sound(game->audio_manager, "wololo");
	}
}
