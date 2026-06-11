#include "components/materials.h"

// clang-format off

// ===============
// = Stones      =
// ===============

material_t DEFAULT_MATERIAL = {
	.ambient =  { 0.75f, 0.75f, 0.75f },
	.diffuse =  { 0.75f, 0.75f, 0.75f },
	.specular = { 0.5f,  0.5f,  0.5f  },
	.shininess = 0.25f
};

material_t MATERIAL_EMERALD = {
	.ambient =  { 0.0215f,  0.1745f,   0.0215f  },
	.diffuse =  { 0.07568f, 0.61424f,  0.07568f },
	.specular = { 0.633f,   0.727811f, 0.633f   },
	.shininess = 0.6f
};

material_t MATERIAL_JADE = {
	.ambient =  { 0.135f, 0.2225f, 0.1575f },
	.diffuse =  { 0.54f,  0.89f,   0.63f   },
	.specular = { 0.316228f, 0.316228f, 0.316228f },
	.shininess = 0.1f
};

material_t MATERIAL_OBSIDIAN = {
	.ambient =  { 0.05375f, 0.05f,    0.06625f },
	.diffuse =  { 0.18275f, 0.17f,    0.22525f },
	.specular = { 0.332741f, 0.328634f, 0.346435f },
	.shininess = 0.3f
};

material_t MATERIAL_PEARL = {
	.ambient =  { 0.25f,    0.20725f, 0.20725f },
	.diffuse =  { 1.0f,     0.829f,   0.829f   },
	.specular = { 0.296648f, 0.296648f, 0.296648f },
	.shininess = 0.088f
};

material_t MATERIAL_RUBY = {
	.ambient =  { 0.1745f, 0.01175f, 0.01175f },
	.diffuse =  { 0.61424f, 0.04136f, 0.04136f },
	.specular = { 0.727811f, 0.626959f, 0.626959f },
	.shininess = 0.6f
};

material_t MATERIAL_TURQUOISE = {
	.ambient =  { 0.1f,    0.18725f, 0.1745f },
	.diffuse =  { 0.396f,  0.74151f, 0.69102f },
	.specular = { 0.297254f, 0.30829f, 0.306678f },
	.shininess = 0.1f
};

// ===============
// = Metals      =
// ===============

material_t MATERIAL_BRASS = {
	.ambient =  { 0.329412f, 0.223529f, 0.027451f },
	.diffuse =  { 0.780392f, 0.568627f, 0.113725f },
	.specular = { 0.992157f, 0.941176f, 0.807843f },
	.shininess = 0.21794872f
};

material_t MATERIAL_BRONZE = {
	.ambient =  { 0.2125f, 0.1275f, 0.054f },
	.diffuse =  { 0.714f,  0.4284f, 0.18144f },
	.specular = { 0.393548f, 0.271906f, 0.166721f },
	.shininess = 0.2f
};

material_t MATERIAL_CHROME = {
	.ambient =  { 0.25f, 0.25f, 0.25f },
	.diffuse =  { 0.4f,  0.4f,  0.4f  },
	.specular = { 0.774597f, 0.774597f, 0.774597f },
	.shininess = 0.6f
};

material_t MATERIAL_COPPER = {
	.ambient =  { 0.19125f, 0.0735f, 0.0225f },
	.diffuse =  { 0.7038f,  0.27048f, 0.0828f },
	.specular = { 0.256777f, 0.137622f, 0.086014f },
	.shininess = 0.1f
};

material_t MATERIAL_GOLD = {
	.ambient =  { 0.24725f, 0.1995f, 0.0745f },
	.diffuse =  { 0.75164f, 0.60648f, 0.22648f },
	.specular = { 0.628281f, 0.555802f, 0.366065f },
	.shininess = 0.4f
};

material_t MATERIAL_SILVER = {
	.ambient =  { 0.19225f, 0.19225f, 0.19225f },
	.diffuse =  { 0.50754f, 0.50754f, 0.50754f },
	.specular = { 0.508273f, 0.508273f, 0.508273f },
	.shininess = 0.4f
};

// ===============
// = Plastics    =
// ===============

material_t MATERIAL_BLACK_PLASTIC = {
	.ambient =  { 0.0f, 0.0f, 0.0f },
	.diffuse =  { 0.01f, 0.01f, 0.01f },
	.specular = { 0.5f, 0.5f, 0.5f },
	.shininess = 0.25f
};

material_t MATERIAL_CYAN_PLASTIC = {
	.ambient =  { 0.0f, 0.1f, 0.06f },
	.diffuse =  { 0.0f, 0.50980392f, 0.50980392f },
	.specular = { 0.50196078f, 0.50196078f, 0.50196078f },
	.shininess = 0.25f
};

material_t MATERIAL_GREEN_PLASTIC = {
	.ambient =  { 0.0f, 0.0f, 0.0f },
	.diffuse =  { 0.1f, 0.35f, 0.1f },
	.specular = { 0.45f, 0.55f, 0.45f },
	.shininess = 0.25f
};

material_t MATERIAL_RED_PLASTIC = {
	.ambient =  { 0.0f, 0.0f, 0.0f },
	.diffuse =  { 0.5f, 0.0f, 0.0f },
	.specular = { 0.7f, 0.6f, 0.6f },
	.shininess = 0.25f
};

material_t MATERIAL_WHITE_PLASTIC = {
	.ambient =  { 0.0f, 0.0f, 0.0f },
	.diffuse =  { 0.55f, 0.55f, 0.55f },
	.specular = { 0.7f, 0.7f, 0.7f },
	.shininess = 0.25f
};

material_t MATERIAL_YELLOW_PLASTIC = {
	.ambient =  { 0.0f, 0.0f, 0.0f },
	.diffuse =  { 0.5f, 0.5f, 0.0f },
	.specular = { 0.6f, 0.6f, 0.5f },
	.shininess = 0.25f
};

// ===============
// = Rubbers     =
// ===============

material_t MATERIAL_BLACK_RUBBER = {
	.ambient =  { 0.02f, 0.02f, 0.02f },
	.diffuse =  { 0.01f, 0.01f, 0.01f },
	.specular = { 0.4f, 0.4f, 0.4f },
	.shininess = 0.078125f
};

material_t MATERIAL_CYAN_RUBBER = {
	.ambient =  { 0.0f, 0.05f, 0.05f },
	.diffuse =  { 0.4f, 0.5f, 0.5f },
	.specular = { 0.04f, 0.7f, 0.7f },
	.shininess = 0.078125f
};

material_t MATERIAL_GREEN_RUBBER = {
	.ambient =  { 0.0f, 0.05f, 0.0f },
	.diffuse =  { 0.4f, 0.5f, 0.4f },
	.specular = { 0.04f, 0.7f, 0.04f },
	.shininess = 0.078125f
};

material_t MATERIAL_RED_RUBBER = {
	.ambient =  { 0.05f, 0.0f, 0.0f },
	.diffuse =  { 0.5f, 0.4f, 0.4f },
	.specular = { 0.7f, 0.04f, 0.04f },
	.shininess = 0.078125f
};

material_t MATERIAL_WHITE_RUBBER = {
	.ambient =  { 0.05f, 0.05f, 0.05f },
	.diffuse =  { 0.5f, 0.5f, 0.5f },
	.specular = { 0.7f, 0.7f, 0.7f },
	.shininess = 0.078125f
};

material_t MATERIAL_YELLOW_RUBBER = {
	.ambient =  { 0.05f, 0.05f, 0.0f },
	.diffuse =  { 0.5f, 0.5f, 0.4f },
	.specular = { 0.7f, 0.7f, 0.04f },
	.shininess = 0.078125f
};
// clang-format on

// ===============
// = Stones      =
// ===============

material_t* materials_default() {
	return &DEFAULT_MATERIAL;
}

material_t* materials_emerald() {
	return &MATERIAL_EMERALD;
}

material_t* materials_jade() {
	return &MATERIAL_JADE;
}

material_t* materials_obsidian() {
	return &MATERIAL_OBSIDIAN;
}

material_t* materials_pearl() {
	return &MATERIAL_PEARL;
}

material_t* materials_ruby() {
	return &MATERIAL_RUBY;
}

material_t* materials_turquoise() {
	return &MATERIAL_TURQUOISE;
}

// ===============
// = Metals      =
// ===============

material_t* materials_brass() {
	return &MATERIAL_BRASS;
}

material_t* materials_bronze() {
	return &MATERIAL_BRONZE;
}

material_t* materials_chrome() {
	return &MATERIAL_CHROME;
}

material_t* materials_copper() {
	return &MATERIAL_COPPER;
}

material_t* materials_gold() {
	return &MATERIAL_GOLD;
}

material_t* materials_silver() {
	return &MATERIAL_SILVER;
}

// ===============
// = Plastics    =
// ===============

material_t* materials_black_plastic() {
	return &MATERIAL_BLACK_PLASTIC;
}

material_t* materials_cyan_plastic() {
	return &MATERIAL_CYAN_PLASTIC;
}

material_t* materials_green_plastic() {
	return &MATERIAL_GREEN_PLASTIC;
}

material_t* materials_red_plastic() {
	return &MATERIAL_RED_PLASTIC;
}

material_t* materials_white_plastic() {
	return &MATERIAL_WHITE_PLASTIC;
}

material_t* materials_yellow_plastic() {
	return &MATERIAL_YELLOW_PLASTIC;
}

// ===============
// = Rubbers     =
// ===============

material_t* materials_black_rubber() {
	return &MATERIAL_BLACK_RUBBER;
}

material_t* materials_cyan_rubber() {
	return &MATERIAL_CYAN_RUBBER;
}

material_t* materials_green_rubber() {
	return &MATERIAL_GREEN_RUBBER;
}

material_t* materials_red_rubber() {
	return &MATERIAL_RED_RUBBER;
}

material_t* materials_white_rubber() {
	return &MATERIAL_WHITE_RUBBER;
}

material_t* materials_yellow_rubber() {
	return &MATERIAL_YELLOW_RUBBER;
}
