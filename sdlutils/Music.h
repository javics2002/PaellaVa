// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <string>
#include "../SDL2_mixer-2.0.2/include/SDL_mixer.h"

class Music {
	
public:
	static bool sound_initialized;
	// cannot copy objects of this type!
	Music& operator=(Music &other) = delete;
	Music(const Music&) = delete;

	Music(const std::string &fileName) {
		if (!sound_initialized) return;
		music_ = Mix_LoadMUS(fileName.c_str());
		assert(music_ != nullptr);
	}

	Music(Music &&other) noexcept {
		if (!sound_initialized) return;
		music_ = other.music_;
		other.music_ = nullptr;
	}
	//static bool isSoundInitialized() { return sound_initialized; }
	Music& operator=(Music &&other) noexcept {
		if (!sound_initialized) return *this;
		this->~Music();
		music_ = other.music_;
		other.music_ = nullptr;
		return *this;
	}

	virtual ~Music() {
		if (!sound_initialized) return;
		if (music_ != nullptr)
			Mix_FreeMusic(music_);
	}

	inline void play(int loops = -1) const {
		if (!sound_initialized) return;
		assert(loops >= -1 && music_ != nullptr);
		Mix_PlayMusic(music_, loops);
	}

	inline static int setMusicVolume(int volume) {
		if (!sound_initialized) return 0;
		assert(volume >= 0 && volume <= 128);
		return Mix_VolumeMusic(volume);
	}

	inline static void haltMusic() {
		if (!sound_initialized) return;
		Mix_HaltMusic();
	}

	inline static void pauseMusic() {
		if (!sound_initialized) return;
		Mix_PauseMusic();
	}

	inline static void resumeMusic() {
		if (!sound_initialized) return;
		Mix_ResumeMusic();
	}


private:
	Mix_Music *music_;
};

