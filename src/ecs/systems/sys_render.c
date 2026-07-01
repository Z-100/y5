#include "utils/collection_hdr.h"
#include "ecs/systems/sys_render.h"

// TODO: Remove
constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

// TODO: Remove
Light light = { .position = { 0.0f, 5.0f, 0.0f },
				.ambient  = { 0.8f, 0.8f, 0.8f },
				.diffuse  = { 0.5f, 0.5f, 0.5f },
				.specular = { 1.0f, 1.0f, 1.0f } };

#define freeAndExit(archetypes) \
	do { \
		free(archetypes); \
		return; \
	} while (0)

void sys_renderer_update(ecs_engine_t* engine, const camera_t* player_camera) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Remove
	// light.position[0] = 7.5f * sinf(game_last_frame());
	// light.position[2] = 7.5f * cosf(game_last_frame());

	size_t		  out_count		 = 0;
	archetype_t** matching_archs = ecs_get_matching_archetypes(engine, filter_renderer, &out_count);

	if (!matching_archs || out_count == 0) {
		freeAndExit(matching_archs);
	}

	// 1 performances pls
	mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(
		glm_rad(player_camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f,
		projectionTransform
	);

	mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
	camera_get_view_matrix(player_camera, &viewTransform);

	GLuint active_shader_id = -1;

	for (size_t i = 0; i < out_count; i++) {

		archetype_t* archetype = matching_archs[i];

		if (archetype->row_count == 0) {
			continue;
		}

		int32_t render_col_idx	  = archetype->column_map[RENDER];
		int32_t transform_col_idx = archetype->column_map[TRANSFORM];
		int32_t rotation_col_idx  = archetype->column_map[ROTATION];

		if (render_col_idx == -1 || transform_col_idx == -1 || rotation_col_idx == -1) {
			continue;
		}

		component_render_t*	   render_components	= archetype->columns[render_col_idx];
		component_transform_t* transform_components = archetype->columns[transform_col_idx];
		component_rotation_t*  rotation_components	= archetype->columns[rotation_col_idx];

		for (size_t row = 0; row < archetype->row_count; row++) {

			component_render_t comp_render = render_components[row];

			bool shader_changed = active_shader_id != comp_render.shader_id;
			if (shader_changed) {

				active_shader_id = comp_render.shader_id;
				use_shader(&active_shader_id);

				set_uniform_vec3(&active_shader_id, "u_viewPos", &player_camera->position);
				set_uniform_vec3(&active_shader_id, "u_light.position", &light.position);
				set_uniform_vec3(&active_shader_id, "u_light.ambient", &light.ambient);
				set_uniform_vec3(&active_shader_id, "u_light.diffuse", &light.diffuse);
				set_uniform_vec3(&active_shader_id, "u_light.specular", &light.specular);

				set_uniform_mat4(&active_shader_id, "u_projectionTransform", &projectionTransform);
				set_uniform_mat4(&active_shader_id, "u_viewTransform", &viewTransform);

				// TODO: Actual materials
				set_uniform_material(&active_shader_id, "u_material", materials_default());
			}

			component_transform_t transform = transform_components[row];
			component_rotation_t  rotation	= rotation_components[row];

			mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

			glm_translate(model_matrix, (vec3) { transform.x, transform.y, transform.z });

			// Rotate anyway, default is { 0, 0, 0, 1 }
			glm_quat_rotate(
				model_matrix, (vec4) { rotation.x, rotation.y, rotation.z, rotation.w },
				model_matrix
			);

			set_uniform_mat4(&active_shader_id, "u_modelTransform", &model_matrix);

			if (comp_render.diffuse_id) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, comp_render.diffuse_id);
			}

			if (comp_render.specular_id) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, comp_render.specular_id);
			}

			renderer_draw(comp_render.model_id);
		}
	}

	free(matching_archs);
}
