#include "tiny_obj_loader.h"
#include "bridges/tiny_obj_loader_bridge.h"

#include "utils/logger_utils.h"

#include <csignal>
#include <cstring>
#include <functional>

void memcpy_attrib_t(
	tinyobj::attrib_t* attrib,
	tinyobj_attrib_t*  c_attrib,
	ModelMetadata*	   metadata
);

void memcpy_shape_t(tinyobj::shape_t* shape, tinyobj_shape_t* c_shape, ModelMetadata* metadata);

void memcpy_material_t(
	tinyobj::material_t* material,
	tinyobj_material_t*	 c_material,
	ModelMetadata*		 metadata
);

template <typename T> T* get_as_pointer(T t_val) {

	T* t_ptr = static_cast<T*>(malloc(sizeof(T)));

	if (!t_ptr)
		return nullptr;

	*t_ptr = t_val;
	return t_ptr;
}

void add_error(ModelMetadata* metadata, const std::string& msg) {

	if (!metadata->error) {
		metadata->error = strdup(msg.c_str());
		return;
	}

	std::string combined_msgs = std::string(metadata->error) + ";" + msg;

	free(metadata->error);
	metadata->error = strdup(combined_msgs.c_str());
}

extern "C" bool tiny_obj_load_obj(ModelObject* model, ModelMetadata* metadata) {

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

	if (!warning.empty())
		metadata->warning = strdup(warning.c_str());

	if (!error.empty() || !object_loaded) {
		metadata->error = strdup(error.c_str());
		return false;
	}

	// =====================
	// = Translate attrib_t =
	// =====================

	model->attrib			   = static_cast<tinyobj_attrib_t*>(malloc(sizeof(tinyobj_attrib_t)));
	tinyobj_attrib_t* c_attrib = model->attrib;

	if (!c_attrib) {
		add_error(metadata, "Failed allocating c_attrib");
		return false;
	}

	memcpy_attrib_t(&attrib, c_attrib, metadata);

	// =============================
	// = Translate vector<shape_t> =
	// =============================

	model->shapes_size = get_as_pointer(shapes.size());
	size_t shapes_size = *model->shapes_size;

	model->shapes = static_cast<tinyobj_shape_t*>(malloc(sizeof(tinyobj_shape_t) * shapes_size));
	tinyobj_shape_t* c_shapes = model->shapes;

	if (!c_shapes) {
		add_error(metadata, "Failed allocating c_shapes");
		free(c_attrib);
		return false;
	}

	for (int sh_i = 0; sh_i < shapes_size; sh_i++)
		memcpy_shape_t(&shapes[sh_i], &c_shapes[sh_i], metadata);

	// ================================
	// = Translate vector<material_t> =
	// ================================

	model->materials_size = get_as_pointer(materials.size());
	size_t materials_size = *model->materials_size;

	model->materials =
		static_cast<tinyobj_material_t*>(malloc(sizeof(tinyobj_material_t) * materials_size));
	tinyobj_material_t* c_materials = model->materials;

	if (!c_materials) {
		add_error(metadata, "Failed allocating c_materials");
		free(c_attrib);
		free(c_shapes);
		return false;
	}

	for (int sh_i = 0; sh_i < materials_size; sh_i++)
		memcpy_material_t(&materials[sh_i], &c_materials[sh_i], metadata);

	return true;
}

// =========================================
// =                                       =
// = Helpers to transform data for the ABI =
// =                                       =
// =========================================

std::function string_transformer = [](const std::string& t) { return strdup(t.c_str()); };

template <typename In, typename Out = In>
void memcpy_vector_to_array(
	std::vector<In>&					 source,
	Out**								 target,
	size_t**							 target_size,
	const std::function<Out(const In&)>& transformer = [](const In& in_val) -> Out {
		return in_val;
	}
) {

	size_t source_size = source.size();
	Out*   target_vals = static_cast<Out*>(malloc(sizeof(Out) * source_size));

	for (int i = 0; i < source_size; i++) {
		const In& source_val = source[i];
		target_vals[i]		 = transformer(source_val);
	}

	*target		 = target_vals;
	*target_size = get_as_pointer<>(source_size);
}

void memcpy_indices(
	std::vector<tinyobj::index_t>& indices,
	tinyobj_index_t**			   c_indices,
	size_t*						   c_indices_count
) {

	*c_indices_count = indices.size();

	*c_indices = static_cast<tinyobj_index_t*>(malloc(sizeof(tinyobj_index_t) * *c_indices_count));

	for (int i = 0; i < *c_indices_count; i++) {

		auto [vertex_index, normal_index, texcoord_index] = indices[i];

		c_indices[i]->vertex_index	 = get_as_pointer(vertex_index);
		c_indices[i]->normal_index	 = get_as_pointer(normal_index);
		c_indices[i]->texcoord_index = get_as_pointer(texcoord_index);
	}
}

void memcpy_vec3(real_t* source, vec3** target, ModelMetadata* metadata) {

	vec3* c_target = static_cast<vec3*>(malloc(sizeof(vec3)));
	*target		   = c_target;

	if (!c_target) {
		metadata->error = strdup("Failed allocating memory for vec3");
		return;
	}

	*c_target[0] = source[0];
	*c_target[1] = source[1];
	*c_target[2] = source[2];
}

void memcpy_attrib_t(
	tinyobj::attrib_t* attrib,
	tinyobj_attrib_t*  c_attrib,
	ModelMetadata*	   metadata
) {

	// clang-format off
	memcpy_vector_to_array<>(attrib->vertices,       &c_attrib->vertices,       &c_attrib->vertices_count);
	memcpy_vector_to_array<>(attrib->vertex_weights, &c_attrib->vertex_weights, &c_attrib->vertex_weights_count);
	memcpy_vector_to_array<>(attrib->normals,        &c_attrib->normals,        &c_attrib->normals_count);
	memcpy_vector_to_array<>(attrib->texcoords,      &c_attrib->texcoords,      &c_attrib->texcoords_count);
	memcpy_vector_to_array<>(attrib->texcoord_ws,    &c_attrib->texcoord_ws,    &c_attrib->texcoord_ws_count);
	memcpy_vector_to_array<>(attrib->colors,         &c_attrib->colors,         &c_attrib->colors_count);
	// clang-format on

	c_attrib->skin_weights_count = get_as_pointer(attrib->skin_weights.size());
	size_t skin_weights_size	 = *c_attrib->skin_weights_count;

	c_attrib->skin_weights = static_cast<tinyobj_skin_weight_t*>(
		malloc(sizeof(tinyobj_skin_weight_t) * skin_weights_size)
	);
	tinyobj_skin_weight_t* c_skin_weights = c_attrib->skin_weights;

	for (int sw_i = 0; sw_i < skin_weights_size; sw_i++) {

		auto [vertex_id, weightValues] = attrib->skin_weights[sw_i];

		c_skin_weights[sw_i].weightValues_count = get_as_pointer(weightValues.size());
		size_t joint_and_weight_size			= *c_skin_weights[sw_i].weightValues_count;

		c_skin_weights[sw_i].weightValues = static_cast<tinyobj_joint_and_weight_t*>(
			malloc(sizeof(tinyobj_joint_and_weight_t) * joint_and_weight_size)
		);
		tinyobj_joint_and_weight_t* c_weightValues = c_skin_weights[sw_i].weightValues;

		c_skin_weights[sw_i].vertex_id = get_as_pointer(vertex_id);

		for (int jw_i = 0; jw_i < joint_and_weight_size; jw_i++) {

			auto [joint_id, weight] = weightValues[jw_i];

			c_weightValues[jw_i].joint_id = get_as_pointer(joint_id);
			c_weightValues[jw_i].weight	  = get_as_pointer(weight);
		}
	}
}

void memcpy_shape_t(tinyobj::shape_t* shape, tinyobj_shape_t* c_shape, ModelMetadata* metadata) {

	/* name */
	c_shape->name = strdup(shape->name.c_str());

	/* mesh_t */
	tinyobj::mesh_t mesh   = shape->mesh;
	c_shape->mesh		   = static_cast<tinyobj_mesh_t*>(malloc(sizeof(tinyobj_mesh_t)));
	tinyobj_mesh_t* c_mesh = c_shape->mesh;

	memcpy_indices(mesh.indices, &c_mesh->indices, c_mesh->indices_count);

	// clang-format off
	memcpy_vector_to_array<>(mesh.num_face_vertices,   &c_mesh->num_face_vertices,   &c_mesh->num_face_vertices_count);
	memcpy_vector_to_array<>(mesh.material_ids,        &c_mesh->material_ids,        &c_mesh->material_ids_count);
	memcpy_vector_to_array<>(mesh.smoothing_group_ids, &c_mesh->smoothing_group_ids, &c_mesh->smoothing_group_ids_count);
	// clang-format on

	c_mesh->tags_count = get_as_pointer(mesh.tags.size());
	size_t tags_size   = *c_mesh->tags_count;

	c_mesh->tags		  = static_cast<tinyobj_tag_t*>(malloc(sizeof(tinyobj_tag_t) * tags_size));
	tinyobj_tag_t* c_tags = c_mesh->tags;

	for (int t_i = 0; t_i < tags_size; t_i++) {

		auto [name, intValues, floatValues, stringValues] = mesh.tags[t_i];

		c_tags[t_i].name = strdup(name.c_str());

		// clang-format off
		memcpy_vector_to_array<>(intValues,    &c_tags[t_i].intValues,    &c_tags[t_i].intValues_count);
		memcpy_vector_to_array<>(floatValues,  &c_tags[t_i].floatValues,  &c_tags[t_i].floatValues_count);
		memcpy_vector_to_array<>(stringValues, &c_tags[t_i].stringValues, &c_tags[t_i].stringValues_count, string_transformer);
		// clang-format on
	}

	/* lines_t */
	tinyobj::lines_t lines	 = shape->lines;
	c_shape->lines			 = static_cast<tinyobj_lines_t*>(malloc(sizeof(tinyobj_lines_t)));
	tinyobj_lines_t* c_lines = c_shape->lines;

	memcpy_indices(lines.indices, &c_lines->indices, c_lines->indices_count);

	memcpy_vector_to_array<>(
		lines.num_line_vertices, &c_lines->num_line_vertices, &c_lines->num_line_vertices_count
	);

	/* points_t */
	tinyobj::points_t points   = shape->points;
	c_shape->points			   = static_cast<tinyobj_points_t*>(malloc(sizeof(tinyobj_points_t)));
	tinyobj_points_t* c_points = c_shape->points;

	memcpy_indices(points.indices, &c_points->indices, c_points->indices_count);
}

void memcpy_texture_option_t(
	tinyobj::texture_option_t& texture_option,
	tinyobj_texture_option_t*  c_texture_option,
	ModelMetadata*			   metadata
) {

	texture_type_t c_texture_type;
	switch (texture_option.type) {
		case tinyobj::TEXTURE_TYPE_NONE:
			c_texture_type = TEXTURE_TYPE_NONE;
			break;
		case tinyobj::TEXTURE_TYPE_SPHERE:
			c_texture_type = TEXTURE_TYPE_SPHERE;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_TOP:
			c_texture_type = TEXTURE_TYPE_CUBE_TOP;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_BOTTOM:
			c_texture_type = TEXTURE_TYPE_CUBE_BOTTOM;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_FRONT:
			c_texture_type = TEXTURE_TYPE_CUBE_FRONT;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_BACK:
			c_texture_type = TEXTURE_TYPE_CUBE_BACK;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_LEFT:
			c_texture_type = TEXTURE_TYPE_CUBE_LEFT;
			break;
		case tinyobj::TEXTURE_TYPE_CUBE_RIGHT:
			c_texture_type = TEXTURE_TYPE_CUBE_RIGHT;
			break;
		default:
			c_texture_type = TEXTURE_TYPE_NONE;
	}

	c_texture_option->type = get_as_pointer(c_texture_type);

	c_texture_option->sharpness	 = get_as_pointer(texture_option.sharpness);
	c_texture_option->brightness = get_as_pointer(texture_option.brightness);
	c_texture_option->contrast	 = get_as_pointer(texture_option.contrast);

	// clang-format off
	memcpy_vec3(texture_option.origin_offset, &c_texture_option->origin_offset, metadata);
	memcpy_vec3(texture_option.scale,         &c_texture_option->scale, metadata);
	memcpy_vec3(texture_option.turbulence,    &c_texture_option->turbulence, metadata);
	// clang-format on

	c_texture_option->texture_resolution = get_as_pointer(texture_option.texture_resolution);
	c_texture_option->clamp				 = get_as_pointer(texture_option.clamp);
	c_texture_option->imfchan			 = get_as_pointer(texture_option.imfchan);
	c_texture_option->blendu			 = get_as_pointer(texture_option.blendu);
	c_texture_option->blendv			 = get_as_pointer(texture_option.blendv);
	c_texture_option->bump_multiplier	 = get_as_pointer(texture_option.bump_multiplier);
	c_texture_option->colorspace		 = strdup(texture_option.colorspace.c_str());
}

void memcpy_material_t(
	tinyobj::material_t* material,
	tinyobj_material_t*	 c_material,
	ModelMetadata*		 metadata
) {

	c_material->name = strdup(material->name.c_str());

	// clang-format off
	memcpy_vec3(material->ambient,       &c_material->ambient,       metadata);
	memcpy_vec3(material->diffuse,       &c_material->diffuse,       metadata);
	memcpy_vec3(material->specular,      &c_material->specular,      metadata);
	memcpy_vec3(material->transmittance, &c_material->transmittance, metadata);
	memcpy_vec3(material->emission,      &c_material->emission,      metadata);
	// clang-format on

	c_material->shininess = get_as_pointer(material->shininess);
	c_material->ior		  = get_as_pointer(material->ior);
	c_material->dissolve  = get_as_pointer(material->dissolve);
	c_material->illum	  = get_as_pointer(material->illum);

	c_material->ambient_texname			   = strdup(material->ambient_texname.c_str());
	c_material->diffuse_texname			   = strdup(material->diffuse_texname.c_str());
	c_material->specular_texname		   = strdup(material->specular_texname.c_str());
	c_material->specular_highlight_texname = strdup(material->specular_highlight_texname.c_str());
	c_material->bump_texname			   = strdup(material->bump_texname.c_str());
	c_material->displacement_texname	   = strdup(material->displacement_texname.c_str());
	c_material->alpha_texname			   = strdup(material->alpha_texname.c_str());
	c_material->reflection_texname		   = strdup(material->reflection_texname.c_str());

	// clang-format off
	memcpy_texture_option_t(material->ambient_texopt,            c_material->ambient_texopt,            metadata);
	memcpy_texture_option_t(material->diffuse_texopt,            c_material->diffuse_texopt,            metadata);
	memcpy_texture_option_t(material->specular_texopt,           c_material->specular_texopt,           metadata);
	memcpy_texture_option_t(material->specular_highlight_texopt, c_material->specular_highlight_texopt, metadata);
	memcpy_texture_option_t(material->bump_texopt,               c_material->bump_texopt,               metadata);
	memcpy_texture_option_t(material->displacement_texopt,       c_material->displacement_texopt,       metadata);
	memcpy_texture_option_t(material->alpha_texopt,              c_material->alpha_texopt,              metadata);
	memcpy_texture_option_t(material->reflection_texopt,         c_material->reflection_texopt,         metadata);
	// clang-format on

	c_material->roughness			= get_as_pointer(material->roughness);
	c_material->metallic			= get_as_pointer(material->metallic);
	c_material->sheen				= get_as_pointer(material->sheen);
	c_material->clearcoat_thickness = get_as_pointer(material->clearcoat_thickness);
	c_material->clearcoat_roughness = get_as_pointer(material->clearcoat_roughness);
	c_material->anisotropy			= get_as_pointer(material->anisotropy);
	c_material->anisotropy_rotation = get_as_pointer(material->anisotropy_rotation);

	c_material->roughness_texname = strdup(material->roughness_texname.c_str());
	c_material->metallic_texname  = strdup(material->metallic_texname.c_str());
	c_material->sheen_texname	  = strdup(material->sheen_texname.c_str());
	c_material->emissive_texname  = strdup(material->emissive_texname.c_str());
	c_material->normal_texname	  = strdup(material->normal_texname.c_str());

	// clang-format off
	memcpy_texture_option_t(material->roughness_texopt, c_material->roughness_texopt);
	memcpy_texture_option_t(material->metallic_texopt,  c_material->metallic_texopt);
	memcpy_texture_option_t(material->sheen_texopt,     c_material->sheen_texopt);
	memcpy_texture_option_t(material->emissive_texopt,  c_material->emissive_texopt);
	memcpy_texture_option_t(material->normal_texopt,    c_material->normal_texopt);
	// clang-format on

	/* TODO: Unknown parameters? */
}
