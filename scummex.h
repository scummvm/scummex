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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/scummex.h,v 1.4 2003/09/19 15:47:42 fingolfin Exp $
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
	byte _encbyte;
	
	Image *_image;
	Sound *_sound;
	Resource *_resource;
	GUI_wxWindows *_gui;

public:
	File _input;
	File _output;
	uint32 block_id;
	

	ScummEX();
	void fileView();
	void Descumm();
	void FileInfo();
	void UpdateInfosFromTree(int blockid); 
	void getFileType(const char *filename);
	void updateLabel(char *label, char *title, uint32 text);
	void add_tree_elements(char *itemName, int blockid, int level);
	void warning(const char *s, ...);
	void debug(int level, const char *s, ...);
	void error(const char *s, ...);
	void FileDump(const char *filename);
	void iMUSEPlay();
	void iMUSEDump(const char *filename);
	void SOUPlay();
	void paletteDraw();
	void bgDraw();
	void objectDraw();
};

#endif
