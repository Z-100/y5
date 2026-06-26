#ifndef AUDIO_H
#define AUDIO_H

#include "utils/collection_sys.h"

typedef struct audio_manager_t {
	ma_engine*	engine;
	map_root_t* sounds_map;
} audio_manager_t;

audio_manager_t* init_audio_manager();
bool			 destroy_audio_manager(audio_manager_t* audio_manager);

bool load_sound(audio_manager_t* audio_manager, char* sound_name, char* sound_file_name);
void play_sound(audio_manager_t* audio_manager, char* sound_name);

#endif
