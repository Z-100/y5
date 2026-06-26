#include "utils/collection_hdr.h"

static void process_inputs(game_t* game);

int main() {

	log_info("Starting y5 :PPPP");

	game_t* game = game_init();
	if (!game) {
		log_error("Failed initializing game!");
		return -1;
	}

	glfwSetKeyCallback(game->main_window, key_callback);

	bool sound_loaded = load_sound(game->audio_manager, "wololo", "wololo.mp3");
	if (!sound_loaded) {
		log_error("Failed loading sound !");
	} else {
		play_sound(game->audio_manager, "wololo");
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

	// clang-format off
	component_group_t grp1 = { .bit_mask = { .render = 1, .transform = 1, .rotation = 1 } };
	spawn_info_t spawn1 = {
		.model_id = monkey_id,
		.shader_id = shader_program_default->id,
		.diffuse_id = shader_program_default->textures[0].id,
		.specular_id = shader_program_default->textures[1].id,
		.initial_pos = { 0, 0, 0 },
		.initial_rotation = { 0, 0, 0, 1 }
	};
	// clang-format on
	spawner_summon(game->ecs_engine, grp1, &spawn1);

	component_group_t grp2 = { .bit_mask = { .render = 1, .transform = 1, .rotation = 1 } };
	// clang-format off
	spawn_info_t spawn2 = {
		.model_id = cube_id,
		.shader_id = shader_program_default->id,
		.diffuse_id = shader_program_default->textures[0].id,
		.specular_id = shader_program_default->textures[1].id,
		.initial_pos = { 1, 0, 0 },
		.initial_rotation = { 0, 0, 0, 1 }
	};
	// clang-format on
	spawner_summon(game->ecs_engine, grp2, &spawn2);

	// component_group_t grp2 = { .bit_mask = { .render = 1, .transform = 1 } };
	// spawner_summon(ecs_engine, grp2, cube_id, shader_program_light->id, 0, 0);

	while (game_is_running(game)) {

		game_update(game);
		process_inputs(game);

		gui_update_imgui(game);

		ecs_engine_tick(game->ecs_engine, game->delta_time);

		gui_render_imgui();

		renderer_update(game);
	}

	gui_terminate_imgui(game->gui);
	renderer_destroy();
	window_manager_destroy_main(game);

	return 0;
}

static void process_inputs(game_t* game) {

	GLFWwindow* main_window = game->main_window;

	if (!game->mouse_locked) {
		return;
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
