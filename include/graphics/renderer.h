#ifndef RENDERER_H
#define RENDERER_H

typedef struct Game Game;

void renderer_init_default(const Game* game);
void renderer_update(const Game* game);
void renderer_destroy();

void renderer_game_loop(const Game* game);

void renderer_initialize_cubes();
int	 elmo_vbo_vao_ebo(
	 unsigned int* vertexBuffer,
	 unsigned int* vertexArray,
	 unsigned int* elementBuffer,
	 unsigned int* trianglesSize,
	 unsigned int* texture1,
	 unsigned int* texture2
 );

#endif
