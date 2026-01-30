#include "utils/audio_manager.h"
#include "utils/headers_collection.h"

ma_engine* init_audio_manager() {

	ma_engine* sound_engine = malloc(sizeof(ma_engine));
	if (!sound_engine) {
		log_error("Failed allocating sound_engine memory");
		return nullptr;
	}

	ma_engine_config config = ma_engine_config_init();

	ma_result result = ma_engine_init(nullptr, sound_engine);
	if (result != MA_SUCCESS) {
		log_error("Failed initializing miniaudio");
		return nullptr;
	}

	return sound_engine;
}

int destroy_audio_manager(ma_engine* engine) {
	log_info("Destroying sound engine");
	free(engine);
	return 0;
}

ma_sound* load_sound(ma_engine* engine, char* sound_name) {

	ma_sound* sound = malloc(sizeof(ma_sound));
	if (!sound) {
		log_error("Failed allocating sound memory");
		return nullptr;
	}

	char* sound_path = concat_dir_file("res/audio", sound_name);
	if (sound_path == nullptr)
		return nullptr;

	ma_result result = ma_sound_init_from_file(engine, sound_path, 0, nullptr, nullptr, sound);
	if (result != MA_SUCCESS) {
		log_error_f("Failed loading sound %s", sound_name);
		return nullptr;
	}

	return sound;
}

void play_sound(ma_sound* sound) {
	log_info("Play sound");
	ma_sound_start(sound);
}

