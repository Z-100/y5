#include "utils/collection_hdr.h"
#include "ecs/systems/sys_inputs.h"

// TODO: idk
void sys_inputs_process(
	ecs_engine_t* engine,
	camera_t*	  camera,
	bool		  mouse_locked,
	GLFWwindow*	  main_window
) {

	if (!mouse_locked) {
		return;
	}

	float delta_time = engine->delta_time;

	if (glfwGetKey(main_window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, FORWARD);
	}

	if (glfwGetKey(main_window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, LEFT);
	}

	if (glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, BACKWARD);
	}

	if (glfwGetKey(main_window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, RIGHT);
	}

	if (glfwGetKey(main_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, UP);
	}

	if (glfwGetKey(main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera_process_keyboard(camera, delta_time, DOWN);
	}
}
