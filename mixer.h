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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/Attic/mixer.h,v 1.2 2003/09/18 19:37:14 fingolfin Exp $
 *
 */

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
