#include "core/hash_map.h"
#include "utils/collection_hdr.h"

static int	find_pos(map_root_t* root, void* key);
static int	hash(hash_function fn_hash, int max_size, void* key);
static void check_and_rehash(map_root_t* root);

map_root_t*
map_init(int size, float load_factor, hash_function fn_hash, equals_function fn_equals) {

	map_root_t* root = malloc(sizeof(map_root_t));

	root->max_size	  = size;
	root->load_factor = load_factor;
	root->fn_hash	  = fn_hash;
	root->fn_equals	  = fn_equals;

	root->keys	  = calloc(size, sizeof(void*));
	root->values  = calloc(size, sizeof(void*));
	root->deleted = calloc(size, sizeof(bool));
	root->size	  = 0;

	if (!root || !root->keys || !root->values) {
		log_error("Failed allocating hash map");
		return nullptr;
	}

	return root;
}

map_root_t* map_init_default(hash_function fn_hash, equals_function fn_equals) {
	return map_init(16, 0.75, fn_hash, fn_equals);
}

// =====================
// HashMap<int, [...]>
// =====================

uint32_t hash_int(const void* key) {

	uint32_t x = *(const uint32_t*) key;

	x = ((x >> 16) ^ x) * 0x42069aa;
	x = ((x >> 16) ^ x) * 0x42069aa;
	x = (x >> 16) ^ x;

	return x;
}

bool equals_int(const void* key1, const void* key2) {
	return *(const uint32_t*) key1 == *(const uint32_t*) key2;
}

map_root_t* map_init_int() {
	return map_init_default(hash_int, equals_int);
}

// =====================
// HashMap<char*, [...]>
// =====================

uint32_t hash_string(const void* key) {
	const char* str	 = key;
	uint32_t	hash = 5381;
	int			c;
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

bool equals_string(const void* key1, const void* key2) {
	return strcmp(key1, key2) == 0;
}

map_root_t* map_init_string() {
	return map_init_default(hash_string, equals_string);
}

void map_put(map_root_t* root, void* key, void* value) {

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

void* map_remove(map_root_t* root, void* key) {

	if (!root) {
		log_error("Map root pointer cannot be null");
		return nullptr;
	}

	int pos = find_pos(root, key);

	void* old_key	= root->keys[pos];
	void* old_value = root->values[pos];

	if (!old_key) {
		// Nothing found
		return nullptr;
	}

	root->keys[pos]	   = nullptr;
	root->values[pos]  = nullptr;
	root->deleted[pos] = true;
	root->size--;

	return old_value;
}

void* map_get(map_root_t* root, void* key) {

	if (!root) {
		log_error("Map root pointer cannot be null");
		return nullptr;
	}

	int	  pos	= find_pos(root, key);
	void* value = root->values[pos];

	return value ? value : nullptr;
}

static int find_pos(map_root_t* root, void* key) {

	int max_size = root->max_size;
	int pos		 = hash(root->fn_hash, max_size, key);

	int firstDeleted = -1;

	for (int i = 0; i < max_size; i++) {

		if (!root->keys[pos]) {

			if (!root->deleted[pos]) {
				return firstDeleted != -1 ? firstDeleted : pos;
			}

			if (firstDeleted == -1) {
				firstDeleted = pos;
			}

		} else if (root->fn_equals(root->keys[pos], key)) {
			return pos;
		}

		pos = (pos + 2 * (i + 1)) % max_size;
	}

	return firstDeleted != -1 ? firstDeleted : pos;
}

static int hash(hash_function fn_hash, int max_size, void* key) {
	return (fn_hash(key) & 0x7fffffff) % max_size;
}

static void check_and_rehash(map_root_t* root) {

	int	  size		  = root->size;
	int	  max_size	  = root->max_size;
	float load_factor = root->load_factor;

	// Skip rehash if load_factor not reached
	if (size <= load_factor * max_size) {
		return;
	}

	int new_size = max_size * 2;

	void** old_keys	   = root->keys;
	void** old_values  = root->values;
	bool*  old_deleted = root->deleted;

	root->max_size = new_size;
	root->keys	   = calloc(new_size, sizeof(void*));
	root->values   = calloc(new_size, sizeof(void*));
	root->deleted  = calloc(new_size, sizeof(bool));

	for (int i = 0; i < max_size; i++) {
		if (old_keys[i] && !old_deleted[i]) {
			map_put(root, old_keys[i], old_values[i]);
		}
	}

	free(old_keys);
	free(old_values);
	free(old_deleted);
}
