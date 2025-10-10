#ifndef CAMERA_H
#define CAMERA_H

typedef float vec3[3];
typedef float vec4[4];
typedef vec4  mat4[4];
typedef struct Game Game;

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

#define DEFAULT_YAW -90.0f;
#define DEFAULT_Y5_PITCH 0.0f;
#define DEFAULT_MOVE_SPEED 1.5f;
#define DEFAULT_LOOK_SENSITIVITY 0.1f;
#define DEFAULT_ZOOM 45.0f;

typedef struct Camera {
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
} Camera;

int	 camera_create_player_camera(Game* game);
void camera_get_view_matrix(Camera* camera, mat4* dest_view_matrix);
void camera_process_keyboard(Game* game, enum Camera_Movement movement);
void camera_process_mouse_movement(float xPos, float yPos, Camera* camera);
void camera_process_mouse_scroll(float yOffset, Camera* camera);

#endif
