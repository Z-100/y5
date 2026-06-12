#include "utils/headers_collection.h"

const char* APP_NAME = "y5";
void		process_inputs(game_t* game);

int main() {

	log_info_f("Starting %s application", APP_NAME);

	bool	game_initialized = game_init();
	game_t* game			 = game_get_game();

	if (!game_initialized || !game) {
		return -1;
	}

	bool window_created = window_manager_create_main(game);

	if (!window_created) {
		return -1;
	}

	bool gui_created = gui_init_imgui(game);

	if (!gui_created) {
		return -1;
	}

	ma_engine* sound_engine = init_audio_manager();
	if (!sound_engine) {
		return -1;
	}

	ma_sound* wololo = load_sound(sound_engine, "wololo.mp3");
	play_sound(wololo);

	ecs_engine_t* ecs_engine = ecs_engine_new();
	if (!ecs_engine) {
		return -1;
	}

	shader_program_t* shader_program_default = shaders_collection_default();
	shader_program_t* shader_program_light	 = shaders_collection_light();

	renderer_init(game);
	renderer_load_shader(shader_program_default);
	renderer_load_shader(shader_program_light);

	model_object_t* monkey_obj = load_model("res/models", "monkey.obj");
	uint8_t			monkey_id  = renderer_load_model(monkey_obj);

	model_object_t* cube_obj = load_model("res/models", "cube.obj");
	uint8_t			cube_id	 = renderer_load_model(cube_obj);

	// TODO: Remove
	remove_but_load_camera(game->player_camera);

	component_group_t grp1 = { .bit_mask = { .render = 1 } };
	spawner_summon(
		ecs_engine, grp1, monkey_id, shader_program_default->id,
		shader_program_default->textures[0].id, shader_program_default->textures[1].id
	);

	// component_group_t grp2 = { .bit_mask = { .render = 1, .transform = 1 } };
	// spawner_summon(ecs_engine, grp2, cube_id, shader_program_light->id, 0, 0);

	while (game_is_running()) {

		game_update();
		process_inputs(game);

		gui_update_imgui(game);

		ecs_engine_tick(ecs_engine, game->delta_time);

		gui_render_imgui();

		renderer_update(game);
	}

	gui_terminate_imgui();
	renderer_destroy();
	window_manager_destroy_main(game);

	return 0;
}

void process_inputs(game_t* game) {

	GLFWwindow* main_window = game->main_window;

	// ---------------
	// Close da window
	// ---------------

	if (glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		game->mouse_locked = !game->mouse_locked;

		// clang-format off
		int cursor_mode = game->mouse_locked ?
				GLFW_CURSOR_DISABLED : // Camera mode
				GLFW_CURSOR_NORMAL;    // UI mode
		// clang-format on

		glfwSetInputMode(main_window, GLFW_CURSOR, cursor_mode);
	}

	// ----------------
	// Process movemint
	// ----------------

	if (glfwGetKey(main_window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_process_keyboard(game, FORWARD);
	}

	if (glfwGetKey(main_window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_process_keyboard(game, LEFT);
	}

	if (glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_process_keyboard(game, BACKWARD);
	}

	if (glfwGetKey(main_window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_process_keyboard(game, RIGHT);
	}

	if (glfwGetKey(main_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera_process_keyboard(game, UP);
	}

	if (glfwGetKey(main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera_process_keyboard(game, DOWN);
	}
}
