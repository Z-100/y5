#ifndef CAMERA_H
#define CAMERA_H
#include "cglm/call/ivec3.h"

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

constexpr float DEFAULT_YAW			= -90.0f;
constexpr float DEFAULT_PITCH		= 0.0f;
constexpr float DEFAULT_MOVE_SPEED		= 1.5f;
constexpr float DEFAULT_LOOK_SENSITIVITY = 0.1f;
constexpr float DEFAULT_ZOOM		= 45.0f;

struct Camera {
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	float yaw;
	float pitch;

	float move_speed;
	float look_sensitivity;
	float zoom;
};

struct Camera* create_default_camera();

void camera_get_view_matrix(struct Camera* camera, mat4* destViewMatrix);

void camera_process_keyboard(enum Camera_Movement movement, float deltaTime, struct Camera* camera);

void camera_process_mouse_movement(float xOffset, float yOffset, struct Camera* camera);

void camera_process_mouse_scroll(float yOffset, struct Camera* camera);

#endif
