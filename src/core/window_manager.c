#include "core/window_manager.h"
#include "utils/headers_collection.h"

constexpr int DEFAULT_WINDOW_WIDTH	= 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;

bool window_manager_create_main(Game* game) {

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		log_error("Failed initializing GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	game->main_scale = gui_main_scale();
	float main_scale = game->main_scale;

	int width_scaled  = (int) (DEFAULT_WINDOW_WIDTH * main_scale);
	int height_scaled = (int) (DEFAULT_WINDOW_HEIGHT * main_scale);

	GLFWwindow* main_window = glfwCreateWindow(width_scaled, height_scaled, "y5", nullptr, nullptr);

	if (main_window == nullptr) {
		log_error_f("Failed creating GLFW window (w:%s, h:%s)", width_scaled, height_scaled);
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(main_window);

	glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(main_window, mouse_callback);
	glfwSetScrollCallback(main_window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		log_error("Error loading GLAD");
		return false;
	}

	game->main_window = main_window;

	return true;
}

bool window_manager_destroy_main(Game* game) {
	glfwTerminate();
	return true;
}
