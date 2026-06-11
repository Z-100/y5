#ifndef HEADERS_COLLECTION_H
#define HEADERS_COLLECTION_H

#define GLFW_INCLUDE_NONE

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

#include "components/materials.h"
#include "graphics/shader_loader.h"

#include "bridges/tiny_obj_loader_bridge.h"

#include "components/materials.h"

#include "core/callbacks.h"
#include "core/game.h"
#include "core/hash_map.h"
#include "core/types.h"
#include "core/window_manager.h"

#include "ecs/component.h"
#include "ecs/ecs_engine.h"
#include "ecs/spawner.h"
#include "ecs/components/components_collection.h"
#include "ecs/systems/systems_collection.h"

#include "graphics/camera.h"
#include "graphics/mesh_generator.h"
#include "graphics/renderer.h"
#include "graphics/shader_loader.h"

#include "gui/gui.h"

#include "utils/audio_manager.h"
#include "utils/file_reader.h"
#include "utils/logger_utils.h"
#include "utils/math_utils.h"
#include "utils/model_loader.h"
#include "utils/shaders_collection.h"
#include "utils/string_utils.h"

#endif
