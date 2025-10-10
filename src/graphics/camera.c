#include "graphics/camera.h"
#include "utils/headers_collection.h"

bool  camera_is_first_focus = true;
float camera_last_x			= .0f;
float camera_last_y			= .0f;

void update_camera_vectors(Camera* camera);

int camera_create_player_camera(Game* game) {

	Camera* player_camera = malloc(sizeof(Camera));
	game->player_camera = player_camera;

	if (!player_camera) {
		log_error_f("Failed allocating memory for Camera");
		return -1;
	}

	glm_vec3_zero(player_camera->position);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, player_camera->front);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, player_camera->up);

	glm_vec3_copy(player_camera->up, player_camera->worldUp);

	player_camera->yaw				= DEFAULT_YAW;
	player_camera->pitch			= DEFAULT_Y5_PITCH;
	player_camera->move_speed		= DEFAULT_MOVE_SPEED;
	player_camera->look_sensitivity = DEFAULT_LOOK_SENSITIVITY;
	player_camera->zoom				= DEFAULT_ZOOM;

	update_camera_vectors(player_camera);

	return 0;
}

void camera_get_view_matrix(Camera* camera, mat4* dest_view_matrix) {

	vec3 center = GLM_VEC3_ZERO_INIT;
	glm_vec3_add(camera->position, camera->front, center);

	glm_lookat(camera->position, center, camera->up, *dest_view_matrix);
}

void camera_process_keyboard(Game* game, enum Camera_Movement movement) {

	Camera* camera = game->player_camera;

	float velocity		= camera->move_speed * game->delta_time;
	vec3  tempDirection = GLM_VEC3_ZERO_INIT;

	switch (movement) {
		case FORWARD:
			glm_vec3_scale(camera->front, velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;

		case BACKWARD:
			glm_vec3_scale(camera->front, -velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;

		case RIGHT:
			glm_vec3_scale(camera->right, velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;

		case LEFT:
			glm_vec3_scale(camera->right, -velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;

		case UP:
			glm_vec3_scale(camera->up, velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;

		case DOWN:
			glm_vec3_scale(camera->up, -velocity, tempDirection);
			glm_vec3_add(camera->position, tempDirection, tempDirection);
			break;
	}

	glm_vec3_copy(tempDirection, camera->position);
}

void camera_process_mouse_movement(float xPos, float yPos, Camera* camera) {

	if (camera_is_first_focus) {
		camera_last_x		  = xPos;
		camera_last_y		  = yPos;
		camera_is_first_focus = false;
	}

	float xOffset = xPos - camera_last_x;
	float yOffset = camera_last_y - yPos;

	camera_last_x = xPos;
	camera_last_y = yPos;

	xOffset *= camera->look_sensitivity;
	yOffset *= camera->look_sensitivity;

	camera->yaw += xOffset;

	if (fabsf(camera->pitch + yOffset) <= 89.0f)
		camera->pitch += yOffset;

	update_camera_vectors(camera);
}

void camera_process_mouse_scroll(float yOffset, Camera* camera) {

	camera->zoom -= yOffset;

	if (camera->zoom < 1.0f)
		camera->zoom = 1.0f;
	else if (camera->zoom > 45.0f)
		camera->zoom = 45.0f;
}

void update_camera_vectors(Camera* camera) {

	camera->front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
	camera->front[1] = sinf(glm_rad(camera->pitch));
	camera->front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));

	glm_vec3_normalize(camera->front);

	glm_vec3_cross(camera->front, camera->worldUp, camera->right);
	glm_normalize(camera->right);

	glm_vec3_cross(camera->right, camera->front, camera->up);
	glm_normalize(camera->up);
}
