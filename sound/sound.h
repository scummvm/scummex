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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/sound/sound.h,v 1.1 2003/09/28 21:49:25 yoshizf Exp $
 *
 */

#ifndef sound_h
#define sound_h

#include "scummsys.h"
#include "file.h"
#include "resource.h"
#include "wxwindows.h"
#include "sound/mixer.h"

struct CompTable {
	int32 offset;
	int32 size;
	int32 codec;
};

class Sound {
private:
	SoundMixer *_mixer;
	Resource *_resource;

	static int32 compDecode(byte * src, byte * dst);
	void initializeImcTables();

public:
	Sound();
	~Sound();
	int playiMUSE(File& _input, BlockTable *_blockTable, int index, File& _output, int save = 0);
	int playSOU(BlockTable *_blockTable, File& _input, int index, File& _output, int save = 0);

};

#endif
