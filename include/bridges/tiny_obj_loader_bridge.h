#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// See https://github.com/tinyobjloader/tinyobjloader
#ifdef TINYOBJLOADER_USE_DOUBLE
typedef double real_t;
#else
typedef float real_t;
#endif

typedef unsigned long long size_t;

typedef enum {
	TEXTURE_TYPE_NONE = 0,
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
	real_t*		   sharpness;
	real_t*		   brightness;
	real_t*		   contrast;
	real_t*		   origin_offset[3];
	real_t*		   scale[3];
	real_t*		   turbulence[3];
	int*		   texture_resolution;
	bool*		   clamp;
	char*		   imfchan;
	bool*		   blendu;
	bool*		   blendv;
	real_t*		   bump_multiplier;
	char*		   colorspace;
} tinyobj_texture_option_t;

typedef struct {
	char**	keys;
	char**	values;
	size_t* count;
} tinyobj_unknown_params_t;

typedef struct {
	char* name;

	real_t* ambient[3];
	real_t* diffuse[3];
	real_t* specular[3];
	real_t* transmittance[3];
	real_t* emission[3];

	real_t* shininess;
	real_t* ior;
	real_t* dissolve;
	int*	illum;

	char* ambient_texname;
	char* diffuse_texname;
	char* specular_texname;
	char* specular_highlight_texname;
	char* bump_texname;
	char* displacement_texname;
	char* alpha_texname;
	char* reflection_texname;

	tinyobj_texture_option_t* ambient_texopt;
	tinyobj_texture_option_t* diffuse_texopt;
	tinyobj_texture_option_t* specular_texopt;
	tinyobj_texture_option_t* specular_highlight_texopt;
	tinyobj_texture_option_t* bump_texopt;
	tinyobj_texture_option_t* displacement_texopt;
	tinyobj_texture_option_t* alpha_texopt;
	tinyobj_texture_option_t* reflection_texopt;

	real_t* roughness;
	real_t* metallic;
	real_t* sheen;
	real_t* clearcoat_thickness;
	real_t* clearcoat_roughness;
	real_t* anisotropy;
	real_t* anisotropy_rotation;

	char* roughness_texname;
	char* metallic_texname;
	char* sheen_texname;
	char* emissive_texname;
	char* normal_texname;

	tinyobj_texture_option_t* roughness_texopt;
	tinyobj_texture_option_t* metallic_texopt;
	tinyobj_texture_option_t* sheen_texopt;
	tinyobj_texture_option_t* emissive_texopt;
	tinyobj_texture_option_t* normal_texopt;

	tinyobj_unknown_params_t* unknown_parameter;
} tinyobj_material_t;

typedef struct {
	char* name;

	int*	intValues;
	size_t* intValues_count;

	real_t* floatValues;
	size_t* floatValues_count;

	char**	stringValues;
	size_t* stringValues_count;
} tinyobj_tag_t;

typedef struct {
	int*	joint_id;
	real_t* weight;
} tinyobj_joint_and_weight_t;

typedef struct {
	int*						vertex_id;
	tinyobj_joint_and_weight_t* weightValues;
	size_t*						weightValues_count;
} tinyobj_skin_weight_t;

typedef struct {
	int* vertex_index;
	int* normal_index;
	int* texcoord_index;
} tinyobj_index_t;

typedef struct {
	tinyobj_index_t* indices;
	size_t*			 indices_count;

	unsigned int* num_face_vertices;
	size_t*		  num_face_vertices_count;

	int*	material_ids;
	size_t* material_ids_count;

	unsigned int* smoothing_group_ids;
	size_t*		  smoothing_group_ids_count;

	tinyobj_tag_t* tags;
	size_t*		   tags_count;
} tinyobj_mesh_t;

typedef struct {
	tinyobj_index_t* indices;
	size_t*			 indices_count;
	int*			 num_line_vertices;
	size_t*			 num_line_vertices_count;
} tinyobj_lines_t;

typedef struct {
	tinyobj_index_t* indices;
	size_t*			 indices_count;
} tinyobj_points_t;

typedef struct {
	char*			  name;
	tinyobj_mesh_t*	  mesh;
	tinyobj_lines_t*  lines;
	tinyobj_points_t* points;
} tinyobj_shape_t;

typedef struct {
	real_t* vertices;
	size_t* vertices_count;

	real_t* vertex_weights;
	size_t* vertex_weights_count;

	real_t* normals;
	size_t* normals_count;

	real_t* texcoords;
	size_t* texcoords_count;

	real_t* texcoord_ws;
	size_t* texcoord_ws_count;

	real_t* colors;
	size_t* colors_count;

	tinyobj_skin_weight_t* skin_weights;
	size_t*				   skin_weights_count;
} tinyobj_attrib_t;

typedef struct ModelObject {
	tinyobj_attrib_t* attrib;

	tinyobj_shape_t* shapes;
	size_t*			 shapes_size;

	tinyobj_material_t* materials;
	size_t*				materials_size;

} ModelObject;

typedef struct ModelMetadata {
	char* warning;
	char* error;
	char* file_path;
	char* materials_path;
} ModelMetadata;

bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata);

#ifdef __cplusplus
}
#endif
