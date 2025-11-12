#include "utils/obj_loader.h"
#include "bridges/tiny_obj_loader_bridge.h"
#include "utils/headers_collection.h"

void load_obj(char* directory, char* filename) {

	ModelObject* model = malloc(sizeof(ModelObject));
	if (!model) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return;
	}

	ModelMetadata* metadata = malloc(sizeof(ModelMetadata));
	if (!metadata) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return;
	}

	size_t len_path	 = strlen(directory) + strlen(filename) + 2;
	char*  file_path = malloc(len_path);
	if (!file_path) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return;
	}

	snprintf(file_path, len_path, "%s/%s", directory, filename);
	metadata->file_path = file_path;
	metadata->materials_path = "res/models/materials/";

	bool load_done = tiny_obj_load_obj(model, metadata);

	if (!load_done || metadata->error) {
		log_error_f("load_obj error: %s", metadata->error);
		return;
	}

	if (metadata->warning)
		log_error_f("load_obj warning: %s", metadata->warning);

	// Shapes
	for (size_t s = 0; s < *model->shapes_size; s++) {

		tinyobj_shape_t current_shape = model->shapes[s];
		tinyobj_mesh_t* current_mesh  = current_shape.mesh;

		// Faces per shape
		size_t index_offset = 0;
		for (size_t f = 0; f < *current_mesh->num_face_vertices; f++) {

			size_t fv = sizeof(current_mesh->num_face_vertices[f]);

			// Vertices per face
			for (size_t v = 0; v < fv; v++) {

				tinyobj_index_t idx = current_mesh->indices[index_offset + v];

				tinyobj_attrib_t* current_attrib = model->attrib;

				real_t vx = current_attrib->vertices[3 * *idx.vertex_index + 0];
				real_t vy = current_attrib->vertices[3 * *idx.vertex_index + 1];
				real_t vz = current_attrib->vertices[3 * *idx.vertex_index + 2];

				if (idx.normal_index >= 0) {
					real_t nx = current_attrib->normals[3 * *idx.normal_index + 0];
					real_t ny = current_attrib->normals[3 * *idx.normal_index + 1];
					real_t nz = current_attrib->normals[3 * *idx.normal_index + 2];
				}

				if (idx.texcoord_index >= 0) {
					real_t tx = current_attrib->texcoords[2 * *idx.texcoord_index + 0];
					real_t ty = current_attrib->texcoords[2 * *idx.texcoord_index + 1];
				}
			}

			index_offset += fv;
		}
	}

	free(metadata);
	free(model);
}
