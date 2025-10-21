#include "components/materials.h"
#include "utils/headers_collection.h"

// clang-format off
const Material DEFAULT_MATERIAL = {
	.ambient =  { 0.75f, 0.75f, 0.75f },
	.diffuse =  { 0.75f, 0.75f, 0.75f },
	.specular = { 0.5f,  0.5f,  0.5f  },
	.shininess = 0.25f
};

const Material MATERIAL_EMERALD = {
	.ambient =  { 0.0215f,  0.1745f,   0.0215f  },
	.diffuse =  { 0.07568f, 0.61424f,  0.07568f },
	.specular = { 0.633f,   0.727811f, 0.633f   },
	.shininess = 0.6f
};
// clang-format on


Material* materials_default() {
	return &DEFAULT_MATERIAL;
}
Material* materials_emerald() {
	return &MATERIAL_EMERALD;
}
