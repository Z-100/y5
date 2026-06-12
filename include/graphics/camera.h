#ifndef CAMERA_H
#define CAMERA_H

#include "utils/collection_sys.h"

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

#define DEFAULT_YAW -90.0f;
#define DEFAULT_Y5_PITCH 0.0f;
#define DEFAULT_MOVE_SPEED 1.5f;
#define DEFAULT_LOOK_SENSITIVITY 0.1f;
#define DEFAULT_ZOOM 45.0f;

typedef struct camera_t {
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

	float last_x;
	float last_y;
} camera_t;

int	 camera_create_player_camera(game_t* game);
void camera_get_view_matrix(camera_t* camera, mat4* dest_view_matrix);
void camera_process_keyboard(game_t* game, enum Camera_Movement movement);
void camera_process_mouse_movement(float xPos, float yPos, camera_t* camera);
void camera_process_mouse_scroll(float yOffset, camera_t* camera);

#endif
