#pragma once

#include "SDL_mixer.h"
#include <map>

class AudioManager
{
	
public:
	virtual ~AudioManager() {}

	bool init(int vol = 100, int music_vol = 5);
	void destroy();

	void setVolume(int sound_vol, int music_vol);

	// Loads a given audio file for playback later
	bool loadWAV(const char* path, int soundId, int vol = 15);
	bool loadMusic(const char* path, int soundId);

	// Plays an a loaded audio file identified by the id used when loaded, returns audio channel used
	int playSound(int soundId, bool loop = false);
	bool playMusic(int soundId, int fadein_ms = 0, bool loop = false);

	// Pause audio playback of an audio channel
	void pauseChannel(int channel);

	// Pause music playback
	void pauseMusic();

	void fadeOutMusic(int ms);

	struct AudioInfo {
		int frequenzy;
		Uint16 format;
		int channels;
		int chunksize;
	};

private:
	AudioInfo audio_info;
	int volume_music;
	int volume_sound_effect;

	std::map<unsigned int, Mix_Chunk*> sound;
	std::map<unsigned int, Mix_Music*> music;
};
