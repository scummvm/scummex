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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/scummex.h,v 1.14 2003/09/24 11:49:30 yoshizf Exp $
 *
 */

#ifndef scummex_h
#define scummex_h

#include "resource.h"
#include "image.h"
#include "sound.h"
#include "wxwindows.h"

class ScummEX {
private:
	File _input;
	File _output;
		
	BlockTable _blockTable[60000];
	
	byte _encbyte;
	
	Image *_image;
	Sound *_sound;
	Resource *_resource;

public:
	int _scummVersion;

	ScummEX();
	~ScummEX();

	void loadFile(const char *filename);

	void warning(const char *s, ...);
	void debug(int level, const char *s, ...);
	void error(const char *s, ...);

	void fileView(int blockid);
	void FileDump(int blockid, const char *filename);
	void iMUSEDump(int blockid, const char *filename);

	void Descumm(int blockid);
	void iMUSEPlay(int blockid);
	void SOUPlay(int blockid);
	void paletteDraw(int blockid);
	void bgDraw(int blockid);
	void SmushFrameDraw(int blockid);
	void objectDraw(int blockid);
	void boxesDraw(int blockid);
	void boxesDrawOverlay(int imageWindowId, int blockid);
	void bgReDraw(int imageWindowId, int blockid);
	
	const BlockTable &getBlockTable(int blockid) const { return _blockTable[blockid]; }
	
	int getInputFileSize() { return _input.size(); }
	byte getEncByte() const { return _encbyte; }
};

#endif
