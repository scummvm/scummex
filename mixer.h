#ifndef mixer_h
#define mixer_h

#include "SDL.h"
#include "SDL_mixer.h"
#include "scummsys.h"

void sampleDone(int channel);

class Mixer {
private:
	int audioinit;
	void initSound();

public:
	Mixer();
	void playSound(SDL_RWops *src, int freesrc);
	void stopSounds();
};

#endif
