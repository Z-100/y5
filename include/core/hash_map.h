#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct {
	// Config
	int	  max_size;
	float load_factor;

	// Values
	int*   keys;
	void** values;
	bool*  deleted;
	int	   size;
} map_root_t;

map_root_t* map_init(int size, float load_factor);
void		map_put(map_root_t* root, int key, void* data);
void*		map_remove(map_root_t* root, int key);
void*		map_get(map_root_t* root, int key);

#endif
