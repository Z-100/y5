#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

typedef struct Game Game;

bool window_manager_create_main(Game* game);
bool window_manager_destroy_main(Game* game);

#endif
