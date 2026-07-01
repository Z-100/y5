#include "core/audio.h"
#include "utils/collection_hdr.h"

audio_manager_t* init_audio_manager() {

	audio_manager_t* audio_manager = malloc(sizeof(ma_engine));
	if (!audio_manager->engine) {
		log_error("Failed allocating sound_engine memory");
		return nullptr;
	}

	audio_manager->engine = malloc(sizeof(ma_engine));
	if (!audio_manager->engine) {
		log_error("Failed allocating sound_engine memory");
		free(audio_manager);
		return nullptr;
	}

	ma_engine_config config = ma_engine_config_init();

	ma_result result = ma_engine_init(nullptr, audio_manager->engine);
	if (result != MA_SUCCESS) {
		log_error("Failed initializing miniaudio");
		free(audio_manager->engine);
		free(audio_manager);
		return nullptr;
	}

	// Map<string, ma_sound[]>
	audio_manager->sounds_map = map_init_string();
	if (!audio_manager->sounds_map) {
		log_error("Failed allocating sounds_map memory");
		free(audio_manager->engine);
		free(audio_manager);
		return nullptr;
	}

	return audio_manager;
}

bool destroy_audio_manager(audio_manager_t* audio_manager) {

	log_info("Destroying audio_manager");
	free(audio_manager->engine);

	// TODO: Check map more
	free(audio_manager->sounds_map);
	free(audio_manager);

	return true;
}

void audio_sound_load_all(audio_manager_t* audio_manager) {

	bool sound_loaded = audio_sound_load(audio_manager, "wololo", "wololo.mp3");
	if (!sound_loaded) {
		log_error("Failed loading sound !");
	}
}

bool audio_sound_load(audio_manager_t* audio_manager, char* sound_name, char* sound_file_name) {

	ma_sound* sound = malloc(sizeof(ma_sound));
	if (!sound) {
		log_error("Failed allocating sound memory");
		return false;
	}

	char* sound_path = concat_dir_file("res/audio", sound_file_name);
	if (sound_path == nullptr) {
		return false;
	}

	ma_result result =
		ma_sound_init_from_file(audio_manager->engine, sound_path, 0, nullptr, nullptr, sound);

	if (result != MA_SUCCESS) {
		log_error_f("Failed loading sound '%s'", sound_file_name);
		return false;
	}

	map_put(audio_manager->sounds_map, sound_name, sound);
	return true;
}

void audio_sound_play(audio_manager_t* audio_manager, char* sound_name) {

	ma_sound* sound = map_get(audio_manager->sounds_map, sound_name);
	if (!sound) {
		log_debug_f("Sound '%s' not found", sound_name);
		return;
	}

	log_debug_f("Sound '%s' playing", sound_name);
	ma_sound_start(sound);
}
