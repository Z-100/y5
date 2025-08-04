#include "camera.h"

// ========================
// = private declarations =
// ========================

void update_camera_vectors(struct Camera* camera);

// ===================
// = implementations =
// ===================

struct Camera* create_default_camera() {

	struct Camera* camera = malloc(sizeof(struct Camera));

	if (!camera) {
		fprintf(stderr, "Error allocating camera");
		return nullptr;
	}

	glm_vec3_zero(camera->position);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, camera->front);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, camera->up);

	glm_vec3_copy(camera->up, camera->worldUp);

	camera->yaw			= DEFAULT_YAW;
	camera->pitch		= DEFAULT_PITCH;
	camera->speed		= DEFAULT_SPEED;
	camera->sensitivity = DEFAULT_SENSITIVITY;
	camera->zoom		= DEFAULT_ZOOM;

	update_camera_vectors(camera);

	return camera;
}

void camera_get_view_matrix(struct Camera* camera, mat4* destViewMatrix) {

	vec3 center = GLM_VEC3_ZERO_INIT;
	glm_vec3_add(camera->position, camera->front, center);

	glm_lookat(camera->position, center, camera->up, *destViewMatrix);
}

void camera_process_keyboard(
	enum Camera_Movement movement,
	float				 deltaTime,
	struct Camera*		 camera
) {

	float velocity		= camera->speed * deltaTime;
	vec3  tempDirection = GLM_VEC3_ZERO_INIT;

	switch (movement) {
		case FORWARD:
			glm_vec3_scale(camera->front, velocity, tempDirection);
			glm_vec3_add(camera->position, camera->front, tempDirection);
			break;

		case BACKWARD:
			glm_vec3_scale(camera->front, -velocity, tempDirection);
			glm_vec3_sub(camera->position, camera->front, tempDirection);
			break;

		case RIGHT:
			glm_vec3_scale(camera->right, velocity, tempDirection);
			glm_vec3_add(camera->position, camera->right, tempDirection);
			break;

		case LEFT:
			glm_vec3_scale(camera->right, -velocity, tempDirection);
			glm_vec3_sub(camera->position, camera->right, tempDirection);
			break;

		case UP:
			fprintf(stderr, "Invalid call to process_keyboard#UP\n");
			break;

		case DOWN:
			fprintf(stderr, "Invalid call to process_keyboard#DOWN\n");
			break;
	}

	glm_vec3_copy(tempDirection, camera->position);
}

void camera_process_mouse_movement(float xOffset, float yOffset, struct Camera* camera) {

	xOffset *= camera->sensitivity;
	yOffset *= camera->sensitivity;

	camera->yaw += xOffset;
	camera->pitch += yOffset;

	if (true) {
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		else if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;
	}

	update_camera_vectors(camera);
}

void camera_process_mouse_scroll(float yOffset, struct Camera* camera) {

	camera->zoom -= yOffset;

	if (camera->zoom < 1.0f)
		camera->zoom = 1.0f;
	else if (camera->zoom > 45.0f)
		camera->zoom = 45.0f;
}

void update_camera_vectors(struct Camera* camera) {

	camera->front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
	camera->front[1] = sinf(glm_rad(camera->pitch));
	camera->front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));

	glm_vec3_normalize(camera->front);

	glm_vec3_cross(camera->front, camera->worldUp, camera->right);
	glm_normalize(camera->right);

	glm_vec3_cross(camera->right, camera->front, camera->up);
	glm_normalize(camera->up);
}
