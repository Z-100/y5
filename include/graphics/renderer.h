#ifndef RENDERER_H
#define RENDERER_H

typedef struct Game		   Game;
typedef struct ModelObject ModelObject;

void renderer_init(const Game* game);
void renderer_update(const Game* game);
void renderer_destroy();

void renderer_init_model(ModelObject* model);
void renderer_init_shaders();

void renderer_game_loop(const Game* game);

#endif
