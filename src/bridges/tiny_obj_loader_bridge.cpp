#include "tiny_obj_loader.h"
#include "bridges/tiny_obj_loader_bridge.h"

#include <csignal>
#include <cstring>

extern "C" {

size_t* get_size_as_pointer(size_t size);

void memcpy_attrib_t(tinyobj::attrib_t* attrib, tinyobj_attrib_t* c_attrib);
void memcpy_shape_t(tinyobj::shape_t* shape, tinyobj_shape_t* c_shape);
void memcpy_material_t(tinyobj::material_t* material, tinyobj_material_t* c_material);

bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata) {

	metadata->warning = nullptr;
	metadata->error	  = nullptr;

	tinyobj::attrib_t				 attrib;
	std::vector<tinyobj::shape_t>	 shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	bool object_loaded = tinyobj::LoadObj(
		&attrib, &shapes, &materials, &warning, &error, metadata->file_path,
		metadata->materials_path
	);

	if (!object_loaded)
		return false;

	if (!warning.empty())
		metadata->warning = strdup(warning.c_str());

	if (!error.empty()) {
		metadata->error = strdup(error.c_str());
		return false;
	}

	// =====================
	// = Translate attrib_t =
	// =====================

	auto* c_attrib = static_cast<tinyobj_attrib_t*>(malloc(sizeof(tinyobj_attrib_t)));
	if (!c_attrib) {
		metadata->error = strdup("Failed allocating c_attrib");
		return false;
	}

	memcpy_attrib_t(&attrib, c_attrib);
	model->attrib = c_attrib;

	// =============================
	// = Translate vector<shape_t> =
	// =============================

	std::size_t shapes_size = shapes.size();
	auto* c_shapes = static_cast<tinyobj_shape_t*>(malloc(sizeof(tinyobj_shape_t) * shapes_size));
	if (!c_shapes) {
		metadata->error = strdup("Failed allocating c_shapes");
		free(c_attrib);
		return false;
	}

	for (int sh_i = 0; sh_i < shapes_size; sh_i++)
		memcpy_shape_t(&shapes[sh_i], &c_shapes[sh_i]);

	model->shapes	   = c_shapes;
	model->shapes_size = get_size_as_pointer(shapes_size);

	// ================================
	// = Translate vector<material_t> =
	// ================================

	std::size_t materials_size = materials.size();
	auto*		c_materials =
		static_cast<tinyobj_material_t*>(malloc(sizeof(tinyobj_material_t) * materials_size));
	if (!c_materials) {
		metadata->error = strdup("Failed allocating c_materials");
		free(c_attrib);
		free(c_shapes);
		return false;
	}

	for (int sh_i = 0; sh_i < materials_size; sh_i++)
		memcpy_material_t(&materials[sh_i], &c_materials[sh_i]);

	model->materials	  = c_materials;
	model->materials_size = get_size_as_pointer(materials_size);

	return true;
}

int* get_int_as_pointer(int size) {

	auto* p_int = static_cast<int*>(malloc(sizeof(int)));

	if (!p_int)
		return nullptr;

	*p_int = size;
	return p_int;
}

size_t* get_size_as_pointer(size_t size) {

	auto* p_size = static_cast<size_t*>(malloc(sizeof(size_t)));

	if (!p_size)
		return nullptr;

	*p_size = size;
	return p_size;
}

void memcpy_real_vector_to_array(
	std::vector<real_t>& source,
	real_t**			 target,
	size_t**			 target_size
) {

	size_t source_size = source.size();
	auto*  target_vals = static_cast<real_t*>(malloc(sizeof(real_t) * source_size));
	for (int i = 0; i < source_size; i++) {
		tinyobj::real_t source_val = source[i];
		target_vals[i]			   = source_val;
	}

	*target		 = target_vals;
	*target_size = get_size_as_pointer(source_size);
}

void memcpy_int_vector_to_array(std::vector<int>& source, int** target, size_t** target_size) {

	size_t source_size = source.size();
	auto*  target_vals = static_cast<int*>(malloc(sizeof(int) * source_size));
	for (int i = 0; i < source_size; i++) {
		int source_val = source[i];
		target_vals[i] = source_val;
	}

	*target		 = target_vals;
	*target_size = get_size_as_pointer(source_size);
}

void memcpy_str_vector_to_array(
	std::vector<std::string>& source,
	char***					  target,
	size_t**				  target_size
) {

	size_t source_size = source.size();
	auto*  target_vals = static_cast<char**>(malloc(sizeof(char*) * source_size));
	for (size_t i = 0; i < source_size; i++) {
		const std::string& source_val = source[i];
		target_vals[i]				  = strdup(source_val.c_str());
	}

	*target		 = target_vals;
	*target_size = get_size_as_pointer(source_size);
}

void memcpy_uint_vector_to_array(
	std::vector<unsigned int>& source,
	unsigned int**			   target,
	size_t**				   target_size
) {

	size_t source_size = source.size();
	auto*  target_vals = static_cast<unsigned int*>(malloc(sizeof(unsigned int) * source_size));
	for (int i = 0; i < source_size; i++) {
		unsigned int source_val = source[i];
		target_vals[i]			= source_val;
	}

	*target		 = target_vals;
	*target_size = get_size_as_pointer(source_size);
}

void memcpy_attrib_t(tinyobj::attrib_t* attrib, tinyobj_attrib_t* c_attrib) {

	// clang-format off
	memcpy_real_vector_to_array(attrib->vertices,       &c_attrib->vertices,       &c_attrib->vertices_count);
	memcpy_real_vector_to_array(attrib->vertex_weights, &c_attrib->vertex_weights, &c_attrib->vertex_weights_count);
	memcpy_real_vector_to_array(attrib->normals,        &c_attrib->normals,        &c_attrib->normals_count);
	memcpy_real_vector_to_array(attrib->texcoords,      &c_attrib->texcoords,      &c_attrib->texcoords_count);
	memcpy_real_vector_to_array(attrib->texcoord_ws,    &c_attrib->texcoord_ws,    &c_attrib->texcoord_ws_count);
	memcpy_real_vector_to_array(attrib->colors,         &c_attrib->colors,         &c_attrib->colors_count);
	// clang-format on

	size_t skin_weights_size = attrib->skin_weights.size();
	auto*  c_skin_weights	 = static_cast<tinyobj_skin_weight_t*>(
		malloc(sizeof(tinyobj_skin_weight_t) * skin_weights_size)
	);

	for (int sw_i = 0; sw_i < skin_weights_size; sw_i++) {

		tinyobj::skin_weight_t skin_weight = attrib->skin_weights[sw_i];

		size_t joint_and_weight_size = skin_weight.weightValues.size();
		auto*  joint_and_weights	 = static_cast<tinyobj_joint_and_weight_t*>(
			 malloc(sizeof(tinyobj_joint_and_weight_t) * joint_and_weight_size)
		 );

		c_skin_weights[sw_i].vertex_id = get_int_as_pointer(skin_weight.vertex_id);

		for (int i = 0; i < joint_and_weight_size; i++) {
			auto [joint_id, weight]		  = skin_weight.weightValues[i];
			joint_and_weights[i].joint_id = joint_id;
			joint_and_weights[i].weight	  = weight;
		}

		c_skin_weights[sw_i].weightValues		= joint_and_weights;
		c_skin_weights[sw_i].weightValues_count = get_size_as_pointer(joint_and_weight_size);
	}

	c_attrib->skin_weights		 = c_skin_weights;
	c_attrib->skin_weights_count = get_size_as_pointer(skin_weights_size);
}

void memcpy_shape_t(tinyobj::shape_t* shape, tinyobj_shape_t* c_shape) {

	/* name */
	c_shape->name = strdup(shape->name.c_str());

	/* mesh_t */
	tinyobj::mesh_t mesh   = shape->mesh;
	auto*			c_mesh = static_cast<tinyobj_mesh_t*>(malloc(sizeof(tinyobj_mesh_t)));

	size_t indices_size = mesh.indices.size();
	auto* c_indices = static_cast<tinyobj_index_t*>(malloc(sizeof(tinyobj_index_t) * indices_size));
	for (int i = 0; i < indices_size; i++) {
		auto [vertex_index, normal_index, texcoord_index] = mesh.indices[i];
		c_indices[i].vertex_index						  = vertex_index;
		c_indices[i].normal_index						  = normal_index;
		c_indices[i].texcoord_index						  = texcoord_index;
	}

	c_mesh->indices		  = c_indices;
	c_mesh->indices_count = get_size_as_pointer(indices_size);

	// clang-format off
	memcpy_uint_vector_to_array(mesh.num_face_vertices,   &c_mesh->num_face_vertices,   &c_mesh->num_face_vertices_count);
	memcpy_int_vector_to_array(mesh.material_ids,         &c_mesh->material_ids,        &c_mesh->material_ids_count);
	memcpy_uint_vector_to_array(mesh.smoothing_group_ids, &c_mesh->smoothing_group_ids, &c_mesh->smoothing_group_ids_count);
	// clang-format on

	size_t tags_size = mesh.tags.size();
	auto*  c_tags	 = static_cast<tinyobj_tag_t*>(malloc(sizeof(tinyobj_tag_t) * tags_size));
	for (int i = 0; i < tags_size; i++) {

		auto [name, intValues, floatValues, stringValues] = mesh.tags[i];

		c_tags[i].name = strdup(name.c_str());

		// clang-format off
		memcpy_int_vector_to_array(intValues,    &c_tags[i].intValues,    &c_tags[i].intValues_count);
		memcpy_real_vector_to_array(floatValues, &c_tags[i].floatValues,  &c_tags[i].floatValues_count);
		memcpy_str_vector_to_array(stringValues, &c_tags[i].stringValues, &c_tags[i].stringValues_count);
		// clang-format on
	}

	c_mesh->tags	   = c_tags;
	c_mesh->tags_count = get_size_as_pointer(tags_size);

	c_shape->mesh = c_mesh;

	/* lines_t */
	tinyobj::lines_t lines = shape->lines;

	/* points_t */
	tinyobj::points_t points = shape->points;
}

void memcpy_material_t(tinyobj::material_t* material, tinyobj_material_t* c_material) {
	// shit
}
}
