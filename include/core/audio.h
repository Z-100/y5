#ifndef AUDIO_H
#define AUDIO_H

#include "utils/collection_sys.h"

typedef struct audio_manager_t {
	ma_engine*	engine;
	map_root_t* sounds_map;
} audio_manager_t;

audio_manager_t* init_audio_manager();
bool			 destroy_audio_manager(audio_manager_t* audio_manager);

void audio_sound_load_all(audio_manager_t* audio_manager);

bool audio_sound_load(audio_manager_t* audio_manager, char* sound_name, char* sound_file_name);
void audio_sound_play(audio_manager_t* audio_manager, char* sound_name);

#endif
