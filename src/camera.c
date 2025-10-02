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

	camera->yaw				 = DEFAULT_YAW;
	camera->pitch			 = DEFAULT_Y5_PITCH;
	camera->move_speed		 = DEFAULT_MOVE_SPEED;
	camera->look_sensitivity = DEFAULT_LOOK_SENSITIVITY;
	camera->zoom			 = DEFAULT_ZOOM;

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

	float velocity		= camera->move_speed * deltaTime;
	vec3  tempDirection = GLM_VEC3_ZERO_INIT;

	fprintf(stdout, "DT: %f\tV:%f\n", deltaTime, velocity);

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

void camera_process_mouse_movement(float xOffset, float yOffset, struct Camera* camera) {

	xOffset *= camera->look_sensitivity;
	yOffset *= camera->look_sensitivity;

	camera->yaw += xOffset;

	if (fabsf(camera->pitch + yOffset) <= 89.0f)
		camera->pitch += yOffset;

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
