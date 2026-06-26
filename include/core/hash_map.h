#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "utils/collection_sys.h"

typedef uint32_t (*hash_function)(const void* key);
typedef bool (*equals_function)(const void* key1, const void* key2);

typedef struct map_root_t {
	// Config
	int	  max_size;
	float load_factor;

	hash_function	fn_hash;
	equals_function fn_equals;

	// Values
	void** keys;
	void** values;
	bool*  deleted;
	int	   size;
} map_root_t;

map_root_t* map_init(int size, float load_factor, hash_function fn_hash, equals_function fn_equals);
map_root_t* map_init_default(hash_function fn_hash, equals_function fn_equals);
map_root_t* map_init_int();
map_root_t* map_init_string();

void  map_put(map_root_t* root, void* key, void* value);
void* map_remove(map_root_t* root, void* key);
void* map_get(map_root_t* root, void* key);

#endif
