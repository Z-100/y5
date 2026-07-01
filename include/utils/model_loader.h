#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "utils/collection_sys.h"

model_object_t* model_loader_load(char* directory, char* filename);
void			model_loader_load_all();

#endif
