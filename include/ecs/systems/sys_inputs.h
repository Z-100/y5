#ifndef SYS_INPUTS_H
#define SYS_INPUTS_H

#include "utils/collection_sys.h"

void sys_inputs_process(
	ecs_engine_t* engine,
	camera_t*	  camera,
	bool		  mouse_locked,
	GLFWwindow*	  main_window
);

#endif
