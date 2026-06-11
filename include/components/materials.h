#ifndef MATERIALS_H
#define MATERIALS_H

typedef float vec3[3];

typedef struct {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;
} material_t;

// TODO: Move?
typedef struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} Light;

// ===============
// = Stones      =
// ===============

material_t* materials_default();
material_t* materials_emerald();
material_t* materials_jade();
material_t* materials_obsidian();
material_t* materials_pearl();
material_t* materials_ruby();
material_t* materials_turquoise();

// ===============
// = Metals      =
// ===============

material_t* materials_brass();
material_t* materials_bronze();
material_t* materials_chrome();
material_t* materials_copper();
material_t* materials_gold();
material_t* materials_silver();

// ===============
// = Plastics    =
// ===============

material_t* materials_black_plastic();
material_t* materials_cyan_plastic();
material_t* materials_green_plastic();
material_t* materials_red_plastic();
material_t* materials_white_plastic();
material_t* materials_yellow_plastic();

// ===============
// = Rubbers     =
// ===============

material_t* materials_black_rubber();
material_t* materials_cyan_rubber();
material_t* materials_green_rubber();
material_t* materials_red_rubber();
material_t* materials_white_rubber();
material_t* materials_yellow_rubber();

#endif
