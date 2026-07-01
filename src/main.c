#include "utils/collection_hdr.h"

int main() {

	log_info("Starting y5 :PPPP");

	game_t* game = game_init();
	if (!game) {
		log_error("Failed initializing game!");
		return -1;
	}

	audio_sound_load_all(game->audio_manager);

	shader_program_t* shader_program_default = shaders_collection_default();
	shader_program_t* shader_program_light	 = shaders_collection_light();

	renderer_init(game);
	renderer_load_shader(shader_program_default);
	renderer_load_shader(shader_program_light);

	model_object_t* monkey_obj = model_loader_load("res/models", "cat.obj");
	uint8_t			monkey_id  = renderer_load_model(monkey_obj);

	model_object_t* cube_obj = model_loader_load("res/models", "cube.obj");
	uint8_t			cube_id	 = renderer_load_model(cube_obj);

	component_group_t group_render = { .bit_mask = { .render = 1, .transform = 1, .rotation = 1 } };

	// clang-format off
	spawn_info_t spawn_monkey = {
		.model_id = monkey_id,
		.shader_id = shader_program_default->id,
		.diffuse_id = shader_program_default->textures[0].id,
		.specular_id = shader_program_default->textures[1].id,
		.initial_pos = { 0, 0, 0 },
		.initial_rotation = { 0, 0, 0, 1 }
	};
	// clang-format on
	spawner_summon(game->ecs_engine, group_render, &spawn_monkey);

	// clang-format off
	spawn_info_t spawn_light_cube = {
		.model_id = cube_id,
		.shader_id = shader_program_light->id,
		// .diffuse_id = shader_program_default->textures[0].id,
		// .specular_id = shader_program_default->textures[1].id,
		.initial_pos = { 1, 0, 0 },
		.initial_rotation = { 0, 0, 0, 1 }
	};
	// clang-format on
	// spawner_summon(game->ecs_engine, group_render, &spawn_light_cube);

	while (game_is_running(game)) {

		game_update(game);

		gui_update_imgui(game);

		ecs_engine_tick(game);

		gui_render_imgui();

		renderer_update(game);
	}

	gui_terminate_imgui(game->gui);
	renderer_destroy();
	window_manager_destroy_main(game);

	return 0;
}
