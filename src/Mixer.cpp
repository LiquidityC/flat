#include <iostream>
#include <string>
#include "Mixer.h"

namespace flat2d
{
	Mixer::~Mixer()
	{
		clearAllSound();
	}

	void Mixer::clearAllSound()
	{
		for (auto it = effects.begin(); it != effects.end(); it++) {
			Mix_FreeChunk(it->second);
		}
		effects.clear();
		for (auto it = music.begin(); it != music.end(); it++) {
			Mix_FreeMusic(it->second);
		}
		music.clear();
	}

	bool Mixer::loadEffect(int id, std::string path)
	{
		if (effects.find(id) != effects.end()) {
			Mix_FreeChunk(effects[id]);
		}

		Mix_Chunk* effect = Mix_LoadWAV(path.c_str());
		if (effect == nullptr) {
			std::cerr << "Unable to load sound effect: " << path << std::endl;
			return false;
		}
		effects[id] = effect;
		return true;
	}

	void Mixer::playEffect(int id)
	{
		if (effects.find(id) == effects.end()) {
			std::cerr << "No sound effect found for id: " << id << std::endl;
			return;
		}

		Mix_PlayChannel(-1, effects[id], 0);
	}

	bool Mixer::loadMusic(int id, std::string path)
	{
		if (music.find(id) != music.end()) {
			Mix_FreeMusic(music[id]);
		}

		Mix_Music* tune = Mix_LoadMUS(path.c_str());
		if (tune == nullptr) {
			std::cerr << "Unable to load music " << path << ": " << Mix_GetError() << std::endl;
			return false;
		}
		music[id] = tune;
		return true;
	}

	void Mixer::playMusic(int id)
	{
		if (music.find(id) == music.end()) {
			std::cerr << "No music found for id: " << id << std::endl;
			return;
		}

		Mix_PlayMusic(music[id], -1);
	}

	bool Mixer::playingMusic()
	{
		return Mix_PlayingMusic();
	}

	void Mixer::pauseMusic()
	{
		if (playingMusic()) {
			Mix_PausedMusic();
		}
	}

	bool Mixer::pausedMusic()
	{
		return Mix_PausedMusic();
	}

	void Mixer::unpauseMusic()
	{
		if (pausedMusic()) {
			Mix_ResumeMusic();
		}
	}

	void Mixer::stopMusic()
	{
		if (playingMusic() || pausedMusic()) {
			Mix_HaltMusic();
		}
	}
} // namespace flat2d
