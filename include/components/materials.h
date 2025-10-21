#ifndef MATERIALS_H
#define MATERIALS_H

typedef float vec3[3];

typedef struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} Material;

// TODO: Move?
typedef struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} Light;

Material* materials_default();
Material* materials_emerald();

#endif
