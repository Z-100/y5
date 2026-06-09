#include "ecs/systems/sys_render.h"
#include "utils/headers_collection.h"

// Filters archetypes with renderer component
const static component_group_t render_comp_filter = { .bit_mask = { .renderer = 1 } };

// TODO: Remove
static void _start() {

	Camera* player_camera = game->player_camera;

	light.position[0] = 7.5f * sinf(game_last_frame());
	light.position[2] = 7.5f * cosf(game_last_frame());

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	use_shader(&shader_texture);
	set_uniform_vec3(&shader_texture, "u_viewPos", &player_camera->position);

	set_uniform_vec3(&shader_texture, "u_light.position", &light.position);
	set_uniform_vec3(&shader_texture, "u_light.ambient", &light.ambient);
	set_uniform_vec3(&shader_texture, "u_light.diffuse", &light.diffuse);
	set_uniform_vec3(&shader_texture, "u_light.specular", &light.specular);

	mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(
		glm_rad(player_camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f,
		projectionTransform
	);
	set_uniform_mat4(&shader_texture, "u_projectionTransform", &projectionTransform);

	mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
	camera_get_view_matrix(player_camera, &viewTransform);
	set_uniform_mat4(&shader_texture, "u_viewTransform", &viewTransform);
}

// TODO: Remove
static void _render_light_cube() {

	use_shader(&shader_light_source);
	set_uniform_vec3(&shader_light_source, "u_lightSourceColor", &light.ambient);

	set_uniform_mat4(&shader_light_source, "u_projectionTransform", &projectionTransform);
	set_uniform_mat4(&shader_light_source, "u_viewTransform", &viewTransform);

	mat4 modelTransform = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelTransform, light.position);

	set_uniform_mat4(&shader_light_source, "u_modelTransform", &modelTransform);

	glBindVertexArray(render_components[1]->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_components[1]->ebo);
	glDrawElements(GL_TRIANGLES, models[1]->index_count, GL_UNSIGNED_INT, nullptr);
}

void sys_renderer_update(ecs_engine_t* engine) {

	size_t out_count = 0;

	archetype_t** matching_archs =
		ecs_get_matching_archetypes(engine, render_comp_filter, &out_count);
	if (!matching_archs || out_count == 0) {
		free(matching_archs);
		return;
	}

	_start();

	for (size_t i = 0; i < out_count; i++) {

		archetype_t* arch = matching_archs[i];

		// Skip without render component
		if (arch->row_count == 0) {
			continue;
		}

		int32_t renderer_col_idx = arch->column_mapping[RENDERER];
		if (renderer_col_idx == -1) {
			continue;
		}

		component_renderer_t* renderers = arch->columns[renderer_col_idx];

		for (size_t row = 0; row < arch->row_count; row++) {

			component_renderer_t active_renderer = renderers[row];
			ModelObject*		 model			 = active_renderer.model;

			set_uniform_material(&shader_texture, "u_material", model->material);

			mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;

			float angle = 20.0f * (float) i;
			glm_rotate(
				modelMatrix, glm_rad(game_last_frame() * angle), (vec3) { 1.0f, 0.3f, 0.5f }
			);

			set_uniform_mat4(&shader_texture, "u_modelTransform", &modelMatrix);

			glBindVertexArray(active_renderer.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, active_renderer.ebo);
			glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, nullptr);
		}
	}

	_end();

	free(matching_archs);
}
