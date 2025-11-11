#include "tiny_obj_loader.h"
#include "bridges/tiny_obj_loader_bridge.h"

#include <signal.h>
#include <string.h>

extern "C" {

size_t* get_heap_size(const size_t size) {
	size_t* p_size = static_cast<size_t*>(malloc(sizeof(size_t)));
	if (!p_size)
		return nullptr;
	*p_size = size;
	return p_size;
}

bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata) {

	tinyobj::attrib_t				 attrib;
	std::vector<tinyobj::shape_t>	 shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	bool object_loaded = tinyobj::LoadObj(
		&attrib, &shapes, &materials, &warning, &error, metadata->file_path, "res/models/materials"
	);

	if (!object_loaded)
		return false;

	if (!warning.empty())
		metadata->warning = strdup(warning.c_str());
	else
		metadata->warning = nullptr;

	if (!error.empty()) {
		metadata->error = strdup(error.c_str());
		return false;
	} else {
		metadata->error = nullptr;
	}

	// ======================
	// = Translate attrib_t =
	// ======================

	size_t	vertices_size = attrib.vertices.size();
	real_t* c_vertices	  = static_cast<real_t*>(malloc(sizeof(real_t) * vertices_size));
	for (int i = 0; i < vertices_size; i++) {
		tinyobj::real_t vertex = attrib.vertices[i];
		c_vertices[i]		   = vertex;
	}

	size_t	normals_size = attrib.normals.size();
	real_t* c_normals	 = static_cast<real_t*>(malloc(sizeof(real_t) * normals_size));
	for (int i = 0; i < normals_size; i++) {
		tinyobj::real_t normal = attrib.normals[i];
		c_normals[i]		   = normal;
	}

	tinyobj_attrib_t* c_attrib = static_cast<tinyobj_attrib_t*>(malloc(sizeof(tinyobj_attrib_t)));

	if (!c_attrib) {
		metadata->error = strdup("Failed allocating c_attrib");
		return false;
	}

	c_attrib->vertices		 = c_vertices;
	c_attrib->vertices_count = get_heap_size(vertices_size);

	c_attrib->normals		= c_normals;
	c_attrib->normals_count = get_heap_size(normals_size);

	// =====================
	// = Translate shape_t =
	// =====================

	std::size_t		 shapes_size = shapes.size();
	tinyobj_shape_t* c_shapes =
		static_cast<tinyobj_shape_t*>(malloc(sizeof(tinyobj_shape_t) * shapes_size));

	for (int idx_s = 0; idx_s < shapes_size; idx_s++) {

		tinyobj::shape_t shape = shapes[idx_s];

		c_shapes[idx_s].name = strdup(shape.name.c_str());

		tinyobj::mesh_t mesh = shape.mesh;
		tinyobj_mesh_t* c_mesh = static_cast<tinyobj_mesh_t*>(malloc(sizeof(tinyobj_mesh_t)));
		c_shapes[idx_s].mesh   = c_mesh;

		size_t			 indices_size = mesh.indices.size();
		tinyobj_index_t* c_mesh_indices =
			static_cast<tinyobj_index_t*>(malloc(sizeof(tinyobj_index_t) * indices_size));

		for (int idx_i = 0; idx_i < indices_size; idx_i++) {

			auto [vertex_index, normal_index, texcoord_index] = mesh.indices[idx_i];

			c_mesh_indices[idx_i].vertex_index	 = vertex_index;
			c_mesh_indices[idx_i].normal_index	 = normal_index;
			c_mesh_indices[idx_i].texcoord_index = texcoord_index;
		}

		c_mesh->indices		  = c_mesh_indices;
		c_mesh->indices_count = get_heap_size(indices_size);

		size_t num_faces_size	   = mesh.num_face_vertices.size();
		int*   c_num_face_vertices = static_cast<int*>(malloc(sizeof(int) * num_faces_size));
		for (int i = 0; i < normals_size; i++) {
			unsigned int num_face_vertex = mesh.num_face_vertices[i];
			c_num_face_vertices[i]		 = num_face_vertex;
		}

		c_mesh->num_face_vertices		= c_num_face_vertices;
		c_mesh->num_face_vertices_count = get_heap_size(num_faces_size);
	}

	model->attrib	   = c_attrib;
	model->shapes	   = c_shapes;
	model->shapes_size = get_heap_size(shapes_size);

	return true;
}
}
