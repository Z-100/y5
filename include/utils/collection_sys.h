#ifndef COLLECTION_SYS_H
#define COLLECTION_SYS_H

#define GLFW_INCLUDE_NONE

// We are forward declaring some "system" stuff here rather than in the other one
typedef struct ImGuiContext ImGuiContext;
typedef struct ImGuiIO		ImGuiIO;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lodepng.h>
#include <stdint.h>
#include <miniaudio.h>

// Best fix ever fuck you
#include "utils/collection_fwd.h"

// We treat this as sys too
#include "utils/logger_utils.h"

#endif
