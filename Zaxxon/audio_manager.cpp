#include "audio_manager.h"
#include "SDL.h"
#include "SDL_mixer.h"

bool AudioManager::init(int vol, int music_vol)
{
	SDL_Init(SDL_INIT_AUDIO);
	audio_info = AudioInfo{ 44100, MIX_DEFAULT_FORMAT, 2, 2048 }; // not very useful right now, just kept for future proofing at this point

	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	if (Mix_OpenAudio(audio_info.frequenzy, audio_info.format, audio_info.channels, audio_info.chunksize) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "SDL_mixer could not initialize! Mix_OpenAudio(): %s\n", Mix_GetError());
		return false;
	}

	int volume_sound_effect = vol;
	int volume_music = music_vol;
	setVolume(volume_sound_effect, volume_music);

	return true;
}

void AudioManager::destroy()
{
	for (auto& s : this->sound)
		Mix_FreeChunk(s.second);

	for (auto& m : this->music)
		Mix_FreeMusic(m.second);

	while (Mix_Init(0))
		Mix_Quit();

	//Mix_QuerySpec();
	Mix_CloseAudio();
}

void AudioManager::setVolume(int sound_vol, int music_vol)
{
	volume_sound_effect = (MIX_MAX_VOLUME / 100) * sound_vol;
	volume_music = (MIX_MAX_VOLUME / 100) * music_vol;
	Mix_Volume(-1, volume_sound_effect);
	Mix_VolumeMusic(volume_music);
}

bool AudioManager::loadWAV(const char* path, int soundId, int vol)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path);
	if (chunk == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to load beat music! Mix_LoadWAV(): %s\n", Mix_GetError());
		return false;
	}

	Mix_VolumeChunk(chunk, (MIX_MAX_VOLUME / 100) * vol);
	sound.insert({ soundId, chunk });

	return true;
}

bool AudioManager::loadMusic(const char* path, int soundId)
{
	Mix_Music* m = Mix_LoadMUS(path);
	if (m == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to load beat music! Mix_LoadMUS(): %s\n", Mix_GetError());
		return false;
	}

	music.insert({ soundId, m });

	return true;
}

int AudioManager::playSound(int soundId, bool loop)
{
	Mix_Chunk* chunk = sound.at(soundId);
	int ch = Mix_PlayChannel(-1, chunk, loop ? -1 : 0);

	if (ch == -1)
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play music! Mix_PlayChannel: %s\n", Mix_GetError());

	return ch;
}

bool AudioManager::playMusic(int soundId, int fadein_ms, bool loop)
{
	Mix_Music* m = music.at(soundId);

	if (fadein_ms > 0)
	{
		if (Mix_FadeInMusic(m, loop ? -1 : 0, fadein_ms) == -1) {
			SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play music! Mix_FadeInMusic: %s\n", Mix_GetError());
			return false;
		}
	}
	else
		if (Mix_PlayMusic(m, loop ? -1 : 0) == -1)
		{
			SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play music! Mix_PlayMusic: %s\n", Mix_GetError());
			return false;
		}

	return true;
}

void AudioManager::pauseChannel(int channel)
{
	Mix_Pause(channel);
}

void AudioManager::pauseMusic()
{
	Mix_PauseMusic();
}

void AudioManager::fadeOutMusic(int ms)
{
	Mix_FadeOutMusic(ms);

	// TODO: Should probably use somethign like this (but need to run on a different thread?)
	//while (!Mix_FadeOutMusic(ms) && Mix_PlayingMusic())
	//	SDL_Delay(100);
}
