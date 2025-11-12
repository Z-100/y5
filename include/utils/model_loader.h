#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

typedef struct ModelObject ModelObject;

ModelObject* load_model(char* directory, char* filename);

#endif
