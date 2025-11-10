#include "tiny_obj_loader.h"

#include <string.h>

extern "C" {

typedef struct ModelObject {
	tinyobj::attrib_t* attrib;

	tinyobj::shape_t* shapes;
	size_t*			  shapes_size;

	tinyobj::material_t* materials;
	size_t*				 materials_size;
} ModelObject;

typedef struct ModelMetadata {
	char* warning;
	char* error;
	char* file_path;
} ModelMetadata;

bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata) {

	tinyobj::attrib_t*				 attrib = static_cast<tinyobj::attrib_t*>(malloc(sizeof(attrib)));
	std::vector<tinyobj::shape_t>	 shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	bool object_loaded =
		tinyobj::LoadObj(attrib, &shapes, &materials, &warning, &error, metadata->file_path);

	if (!object_loaded)
		return false;

	if (!warning.empty())
		metadata->warning = strdup(warning.c_str());

	if (!error.empty()) {
		metadata->error = strdup(error.c_str());
		return false;
	}

	// TODO: ABI compliant stuff AAAAAAAAAAAAAAAaaa

	model->attrib = attrib;

	return true;
}
}
