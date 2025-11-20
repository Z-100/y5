#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

typedef struct ma_engine ma_engine;
typedef struct ma_sound	 ma_sound;

ma_engine* init_audio_manager();
int		   destroy_audio_manager(ma_engine* engine);

ma_sound* load_sound(ma_engine* engine, char* sound_name);
void	  play_sound(ma_sound* sound);

#endif
