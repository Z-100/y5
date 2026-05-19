#include "core/hash_map.h"
#include "utils/headers_collection.h"

static int	find_pos(map_root_t* root, int key);
static int	hash(int max_size, int key);
static void check_and_rehash(map_root_t* root);

map_root_t* map_init(int size, float load_factor) {

	map_root_t* root = malloc(sizeof(map_root_t));

	root->max_size	  = size;
	root->load_factor = load_factor;

	root->keys	  = malloc(sizeof(void*) * size);
	root->values  = malloc(sizeof(void*) * size);
	root->deleted = malloc(sizeof(bool) * size);

	if (!root || !root->keys || !root->values) {
		log_error("Failed allocating hash map");
		return nullptr;
	}

	return root;
}

void map_put(map_root_t* root, int key, void* value) {

	if (!root) {
		log_error("Map root pointer cannot be null");
		return;
	}

	check_and_rehash(root);

	int pos = find_pos(root, key);

	void* old_value = root->values[pos];
	if (!old_value) {
		root->size++;
	}

	root->keys[pos]	   = key;
	root->values[pos]  = value;
	root->deleted[pos] = false;
}

void* map_remove(map_root_t* root, int key) {

	if (!root) {
		log_error("Map root pointer cannot be null");
		return nullptr;
	}

	int pos = find_pos(root, key);

	int	  old_key	= root->keys[pos];
	void* old_value = root->values[pos];

	if (!old_key) {
		// Nothing found
		return nullptr;
	}

	root->keys[pos]	   = 0;
	root->values[pos]  = nullptr;
	root->deleted[pos] = true;
	root->size--;

	return old_value;
}

void* map_get(map_root_t* root, int key) {

	if (!root) {
		log_error("Map root pointer cannot be null");
		return nullptr;
	}

	int	  pos	= find_pos(root, key);
	void* value = root->values[pos];

	return value ? value : nullptr;
}

static int find_pos(map_root_t* root, int key) {

	int max_size = root->max_size;
	int pos		 = hash(max_size, key);

	int firstDeleted = -1;

	for (int i = 0; i < max_size; i++) {

		if (!root->keys[pos]) {

			if (!root->deleted[pos]) {
				return firstDeleted != -1 ? firstDeleted : pos;
			}

			if (firstDeleted == -1) {
				firstDeleted = pos;
			}

		} else if (root->keys[pos] == key) {
			return pos;
		}

		pos = (pos + 2 * (i + 1)) % max_size;
	}

	return firstDeleted != -1 ? firstDeleted : pos;
}

static int hash(int max_size, int key) {
	return (key & 0x7fffffff) % max_size;
}

static void check_and_rehash(map_root_t* root) {

	int	  size		  = root->size;
	int	  max_size	  = root->max_size;
	float load_factor = root->load_factor;

	// Skip rehash if load_factor not reached
	if (size <= load_factor * max_size) {
		return;
	}

	int new_size = size * 2;

	int*   old_keys	   = root->keys;
	void** old_values  = root->values;
	bool*  old_deleted = root->deleted;

	root->max_size = new_size;
	root->keys	   = malloc(sizeof(int) * new_size);
	root->values   = malloc(sizeof(void*) * new_size);
	root->deleted  = malloc(sizeof(bool) * new_size);

	for (int i = 0; i < size; i++) {
		if (old_keys[i] && !old_deleted[i]) {
			map_put(root, old_keys[i], old_values[i]);
		}
	}

	free(old_keys);
	free(old_values);
	free(old_deleted);
}
