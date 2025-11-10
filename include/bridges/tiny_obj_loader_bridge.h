#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TINYOBJLOADER_USE_DOUBLE
typedef double real_t;
#else
typedef float real_t;
#endif

typedef unsigned int	   u_int;
typedef unsigned long long size_t;

typedef struct {
	int vertex_index;
	int normal_index;
	int texcoord_index;
} index_t;

typedef struct {
	int	   joint_id;
	real_t weight;
} joint_and_weight_t;

typedef struct {
	int vertex_id;

	joint_and_weight_t* weightValues;
} skin_weight_t;

typedef struct {
	real_t* vertices;

	real_t* vertex_weights;
	real_t* normals;
	real_t* texcoords;

	real_t* texcoord_ws;
	real_t* colors;

	skin_weight_t* skin_weights;
} attrib_t;

typedef struct {
	char*	name;
	int*	intValues;
	real_t* floatValues;
	char*	stringValues;
} tag_t;

typedef struct {
	index_t* indices;
	u_int*	 num_face_vertices;
	int*	 material_ids;
	u_int*	 smoothing_group_ids;
	tag_t*	 tags;
} mesh_t;

typedef struct {
	index_t* indices;
	int*	 num_line_vertices;
} lines_t;

typedef struct {
	index_t* indices;
} points_t;

typedef struct {
	char*	 name;
	mesh_t	 mesh;
	lines_t	 lines;
	points_t points;
} shape_t;

typedef enum {
	TEXTURE_TYPE_NONE,
	TEXTURE_TYPE_SPHERE,
	TEXTURE_TYPE_CUBE_TOP,
	TEXTURE_TYPE_CUBE_BOTTOM,
	TEXTURE_TYPE_CUBE_FRONT,
	TEXTURE_TYPE_CUBE_BACK,
	TEXTURE_TYPE_CUBE_LEFT,
	TEXTURE_TYPE_CUBE_RIGHT
} texture_type_t;

typedef struct {
	texture_type_t type;
	real_t		   sharpness;
	real_t		   brightness;
	real_t		   contrast;
	real_t		   origin_offset[3];
	real_t		   scale[3];
	real_t		   turbulence[3];
	int			   texture_resolution;

	bool   clamp;
	char   imfchan;
	bool   blendu;
	bool   blendv;
	real_t bump_multiplier;

	char* colorspace;

} texture_option_t;

typedef struct {
	char* name;

	real_t ambient[3];
	real_t diffuse[3];
	real_t specular[3];
	real_t transmittance[3];
	real_t emission[3];
	real_t shininess;
	real_t ior;
	real_t dissolve;

	int illum;

	int dummy;

	char* ambient_texname;
	char* diffuse_texname;
	char* specular_texname;
	char* specular_highlight_texname;
	char* bump_texname;
	char* displacement_texname;
	char* alpha_texname;
	char* reflection_texname;

	texture_option_t ambient_texopt;
	texture_option_t diffuse_texopt;
	texture_option_t specular_texopt;
	texture_option_t specular_highlight_texopt;
	texture_option_t bump_texopt;
	texture_option_t displacement_texopt;
	texture_option_t alpha_texopt;
	texture_option_t reflection_texopt;

	real_t roughness;
	real_t metallic;
	real_t sheen;
	real_t clearcoat_thickness;
	real_t clearcoat_roughness;
	real_t anisotropy;
	real_t anisotropy_rotation;
	real_t pad0;
	char*  roughness_texname;
	char*  metallic_texname;
	char*  sheen_texname;
	char*  emissive_texname;
	char*  normal_texname;

	texture_option_t roughness_texopt;
	texture_option_t metallic_texopt;
	texture_option_t sheen_texopt;
	texture_option_t emissive_texopt;
	texture_option_t normal_texopt;

	int pad2;

	char* unknown_parameter;

} material_t;

typedef struct ModelObject {
	attrib_t* attrib;

	shape_t* shapes;
	size_t*	 shapes_size;

	material_t* materials;
	size_t*		materials_size;

} ModelObject;

typedef struct ModelMetadata {
	char* warning;
	char* error;
	char* file_path;
} ModelMetadata;

bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata);

#ifdef __cplusplus
}
#endif
