#include "core/window_manager.h"
#include "utils/collection_hdr.h"

constexpr int DEFAULT_WINDOW_WIDTH	= 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;

GLFWwindow* window_manager_create_main(window_manger_info_t wm_info) {

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		log_error("Failed initializing GLFW");
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float main_scale = gui_provide_main_scale();

	int width_scaled  = (int) (DEFAULT_WINDOW_WIDTH * main_scale);
	int height_scaled = (int) (DEFAULT_WINDOW_HEIGHT * main_scale);

	GLFWwindow* main_window =
		glfwCreateWindow(width_scaled, height_scaled, wm_info.title, nullptr, nullptr);

	if (main_window == nullptr) {
		log_error_f("Failed creating GLFW window (w:%s, h:%s)", width_scaled, height_scaled);
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(main_window);

	glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(main_window, mouse_callback);
	glfwSetScrollCallback(main_window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		log_error("Error loading GLAD");
		return nullptr;
	}

	return main_window;
}

bool window_manager_destroy_main(game_t* game) {
	glfwTerminate();
	return true;
}
