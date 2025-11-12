#include "utils/model_loader.h"
#include "bridges/tiny_obj_loader_bridge.h"
#include "utils/headers_collection.h"

ModelObject* load_model(char* directory, char* filename) {

	ModelObject* model = malloc(sizeof(ModelObject));
	if (!model) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return nullptr;
	}

	ModelMetadata* metadata = malloc(sizeof(ModelMetadata));
	if (!metadata) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return nullptr;
	}

	size_t len_path	 = strlen(directory) + strlen(filename) + 2;
	char*  file_path = malloc(len_path);
	if (!file_path) {
		fprintf(stderr, "Failed malloc for building '%s/%s'\n", directory, filename);
		return nullptr;
	}

	snprintf(file_path, len_path, "%s/%s", directory, filename);
	metadata->file_path		 = file_path;
	metadata->materials_path = "res/models/materials/";

	bool load_done = tiny_obj_load_obj(model, metadata);

	if (!load_done || metadata->error) {
		log_error_f("load_obj error: %s", metadata->error);
		return nullptr;
	}

	if (metadata->warning)
		log_error_f("load_obj warning: %s", metadata->warning);

	// ============================
	// = OpenGL bus prep for data =
	// ============================

	tinyobj_attrib_t* attrib = model->attrib;

	size_t total_indices = 0;

	for (size_t s = 0; s < *model->shapes_size; s++) {

		tinyobj_shape_t* shape = &model->shapes[s];

		size_t num_faces = *shape->mesh->num_face_vertices_count;

		for (size_t f = 0; f < num_faces; f++)
			total_indices += shape->mesh->num_face_vertices[f];
	}

	const size_t floats_per_vertex = 8;

	model->vertices		= malloc(sizeof(float) * total_indices * floats_per_vertex);
	model->indices		= malloc(sizeof(unsigned int) * total_indices);
	model->vertex_count = total_indices * floats_per_vertex;
	model->index_count	= total_indices;

	if (!model->vertices || !model->indices) {
		fprintf(stderr, "Failed malloc for flat arrays");
		free(file_path);
		free(metadata);
		free(model->vertices);
		free(model->indices);
		free(model);
		return nullptr;
	}

	size_t vertex_i = 0;
	size_t index_i	= 0;

	for (size_t s = 0; s < *model->shapes_size; s++) {
		tinyobj_shape_t* shape		  = &model->shapes[s];
		size_t			 num_faces	  = *shape->mesh->num_face_vertices_count;
		size_t			 index_offset = 0;

		for (size_t f = 0; f < num_faces; f++) {
			unsigned int vertices_in_face = shape->mesh->num_face_vertices[f];

			for (unsigned int v = 0; v < vertices_in_face; v++) {

				tinyobj_index_t idx = shape->mesh->indices[index_offset + v];

				int vi = idx.vertex_index ? *idx.vertex_index : -1;
				if (vi >= 0) {
					int v_idx					= 3 * vi;
					model->vertices[vertex_i++] = attrib->vertices[v_idx + 0];
					model->vertices[vertex_i++] = attrib->vertices[v_idx + 1];
					model->vertices[vertex_i++] = attrib->vertices[v_idx + 2];
				} else {
					model->vertices[vertex_i++] = 0.0f;
					model->vertices[vertex_i++] = 0.0f;
					model->vertices[vertex_i++] = 0.0f;
				}

				int ti = idx.texcoord_index ? *idx.texcoord_index : -1;
				if (ti >= 0) {
					int t_idx					= 2 * ti;
					model->vertices[vertex_i++] = attrib->texcoords[t_idx + 0];
					model->vertices[vertex_i++] = attrib->texcoords[t_idx + 1];
				} else {
					model->vertices[vertex_i++] = 0.0f;
					model->vertices[vertex_i++] = 0.0f;
				}

				int normal_index = idx.normal_index ? *idx.normal_index : -1;
				if (normal_index >= 0) {
					int n_idx					= 3 * normal_index;
					model->vertices[vertex_i++] = attrib->normals[n_idx + 0];
					model->vertices[vertex_i++] = attrib->normals[n_idx + 1];
					model->vertices[vertex_i++] = attrib->normals[n_idx + 2];
				} else {
					model->vertices[vertex_i++] = 0.0f;
					model->vertices[vertex_i++] = 0.0f;
					model->vertices[vertex_i++] = 0.0f;
				}

				unsigned int vertex_id	  = (unsigned int) (vertex_i / floats_per_vertex - 1);
				model->indices[index_i++] = vertex_id;
			}

			index_offset += vertices_in_face;
		}
	}

	printf(
		"Loaded %zu vertices (%zu indices)\n", model->vertex_count / floats_per_vertex,
		model->index_count
	);

	return model;
}
