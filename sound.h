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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/Attic/sound.h,v 1.3 2003/09/21 00:45:16 yoshizf Exp $
 *
 */

#ifndef sound_h
#define sound_h

#include "SDL.h"
#include "scummsys.h"
#include "file.h"
#include "resource.h"
#include "mixer.h"
#include "wxwindows.h"

struct CompTable {
	int32 offset;
	int32 size;
	int32 codec;
};
	
class Sound {
private:
	Mixer *_mixer;
	Resource *_resource;
	GUI_wxWindows *_gui;

	static int32 Sound::compDecode(byte * src, byte * dst);

public:
	Sound();
	~Sound();
	int playiMUSE(File& _input, BlockTable *_blockTable, int index, File& _output, int save = 0);
	int playSOU(BlockTable *_blockTable, File& _input, int index, File& _output, int save = 0);

};

#endif
