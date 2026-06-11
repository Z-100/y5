#include "ecs/systems/sys_render.h"
#include "utils/headers_collection.h"

// TODO: Remove
constexpr int WIDTH	 = 1280;
constexpr int HEIGHT = 720;

// TODO: Remove
Light light = {
	.position = { 0.0f, 5.0f, 0.0f },
	.ambient =  { 0.8f, 0.8f, 0.8f },
	.diffuse =  { 0.5f, 0.5f, 0.5f },
	.specular = { 1.0f, 1.0f, 1.0f }
};

// TODO: Remove
static void _start() {
	//
	// glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, textures[0]);
	//
	// glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_2D, textures[1]);
}

// TODO: Remove
static void _render_light_cube() {

	// use_shader(&shader_light_source);
	// set_uniform_vec3(&shader_light_source, "u_lightSourceColor", &light.ambient);
	//
	// set_uniform_mat4(&shader_light_source, "u_projectionTransform", &projectionTransform);
	// set_uniform_mat4(&shader_light_source, "u_viewTransform", &viewTransform);
	//
	// mat4 modelTransform = GLM_MAT4_IDENTITY_INIT;
	// glm_translate(modelTransform, light.position);
	//
	// set_uniform_mat4(&shader_light_source, "u_modelTransform", &modelTransform);
	//
	// glBindVertexArray(render_components[1]->vao);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_components[1]->ebo);
	// glDrawElements(GL_TRIANGLES, models[1]->index_count, GL_UNSIGNED_INT, nullptr);
}

#define freeAndExit(archetypes) \
	do { \
		free(archetypes); \
		return; \
	} while (0)

// TODO: Remove
static Camera* player_camera;
void remove_but_load_camera(Camera* camera) {
	player_camera = camera;
}

void sys_renderer_update(ecs_engine_t* engine) {

	// TODO: Remove
	light.position[0] = 7.5f * sinf(game_last_frame());
	light.position[2] = 7.5f * cosf(game_last_frame());

	size_t		  out_count		 = 0;
	archetype_t** matching_archs = ecs_get_matching_archetypes(engine, filter_render, &out_count);

	if (!matching_archs || out_count == 0) {
		freeAndExit(matching_archs);
	}

	for (size_t i = 0; i < out_count; i++) {

		archetype_t* archetype = matching_archs[i];

		if (archetype->row_count == 0) {
			continue;
		}

		int32_t render_col_idx = archetype->column_map[RENDER];
		if (render_col_idx == -1) {
			continue;
		}

		component_render_t* render_components = archetype->columns[render_col_idx];

		for (size_t row = 0; row < archetype->row_count; row++) {

			component_render_t component = render_components[row];

			uint32_t model_id	 = component.model_id;
			uint32_t material_id = component.material_id;
			uint32_t shader_id	 = component.shader_id;

			set_uniform_vec3(&shader_id, "u_viewPos", &player_camera->position);

			set_uniform_vec3(&shader_id, "u_light.position", &light.position);
			set_uniform_vec3(&shader_id, "u_light.ambient", &light.ambient);
			set_uniform_vec3(&shader_id, "u_light.diffuse", &light.diffuse);
			set_uniform_vec3(&shader_id, "u_light.specular", &light.specular);

			mat4 projectionTransform = GLM_MAT4_IDENTITY_INIT;
			glm_perspective(
				glm_rad(player_camera->zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f,
				projectionTransform
			);
			set_uniform_mat4(&shader_id, "u_projectionTransform", &projectionTransform);

			mat4 viewTransform = GLM_MAT4_IDENTITY_INIT;
			camera_get_view_matrix(player_camera, &viewTransform);
			set_uniform_mat4(&shader_id, "u_viewTransform", &viewTransform);

			// TODO: Actual materials
			set_uniform_material(&shader_id, "u_material", materials_default());

			mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;

			float angle = 20.0f * (float) i;
			glm_rotate(
				modelMatrix, glm_rad(game_last_frame() * angle), (vec3) { 1.0f, 0.3f, 0.5f }
			);

			set_uniform_mat4(&shader_id, "u_modelTransform", &modelMatrix);

			renderer_draw(model_id);
		}
	}

	free(matching_archs);
}
