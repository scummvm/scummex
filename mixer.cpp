/* ScummEX - Viewer for Scumm data files
 * Copyright (C) 2003 Adrien Mercier
 * Copyright (C) 2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/Attic/mixer.cpp,v 1.2 2003/09/18 19:37:14 fingolfin Exp $
 *
 */

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
