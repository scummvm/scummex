#include "mixer.h"

Mix_Chunk *sample = NULL;

Mixer::Mixer()
{
	audioinit = 0;
}

void Mixer::initSound()
{
	int audio_rate = 44100;
	uint16 audio_format = AUDIO_S16;
	int audio_channels = 2;
	int audio_buffers = 4096;

	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		audioinit = 0;
		return;
	}
	
	audioinit = 1;
	
	return;
}
	
void Mixer::playSound(SDL_RWops *src, int freesrc)
{
	if (audioinit == 0)
		initSound();

	if (audioinit == 0)
		return;

	if (sample == NULL) {
		sample = Mix_LoadWAV_RW(src, freesrc);
		Mix_PlayChannel(-1,sample,0);

		Mix_ChannelFinished(sampleDone);

	} else {
		Mix_HaltChannel(-1);
		Mix_FreeChunk(sample);

		sample = NULL;
		
		sample = Mix_LoadWAV_RW(src, freesrc);

		Mix_PlayChannel(-1,sample,0);

		Mix_ChannelFinished(sampleDone);
		
	}
}

void Mixer::stopSounds() {
	if (sample) {
		Mix_HaltChannel(-1);
		Mix_FreeChunk(sample);

		sample = NULL;
	}
}

void sampleDone(int channel)
{
	Mix_FreeChunk(sample);
	sample = NULL;

}
