#ifndef MATERIALS_H
#define MATERIALS_H

typedef float vec3[3];

typedef struct Material {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;
} Material;

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

Material* materials_default();
Material* materials_emerald();
Material* materials_jade();
Material* materials_obsidian();
Material* materials_pearl();
Material* materials_ruby();
Material* materials_turquoise();

// ===============
// = Metals      =
// ===============

Material* materials_brass();
Material* materials_bronze();
Material* materials_chrome();
Material* materials_copper();
Material* materials_gold();
Material* materials_silver();

// ===============
// = Plastics    =
// ===============

Material* materials_black_plastic();
Material* materials_cyan_plastic();
Material* materials_green_plastic();
Material* materials_red_plastic();
Material* materials_white_plastic();
Material* materials_yellow_plastic();

// ===============
// = Rubbers     =
// ===============

Material* materials_black_rubber();
Material* materials_cyan_rubber();
Material* materials_green_rubber();
Material* materials_red_rubber();
Material* materials_white_rubber();
Material* materials_yellow_rubber();

#endif
