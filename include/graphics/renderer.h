#ifndef RENDERER_H
#define RENDERER_H

typedef struct Game		   Game;
typedef struct ModelObject ModelObject;

void renderer_init_default(const Game* game);
void renderer_update(const Game* game);
void renderer_destroy();

void renderer_game_loop(const Game* game, ModelObject* model);

void renderer_initialize_cubes(ModelObject* model_object);
int	 elmo_vbo_vao_ebo(
	 ModelObject*  model_object,
	 unsigned int* vertexBuffer,
	 unsigned int* vertexArray,
	 unsigned int* elementBuffer,
	 unsigned int* trianglesSize,
	 unsigned int* texture1,
	 unsigned int* texture2
 );

#endif
