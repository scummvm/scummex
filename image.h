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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/image.h,v 1.4 2003/09/19 15:47:42 fingolfin Exp $
 *
 */

#ifndef image_h
#define image_h

#include "scummsys.h"

struct rgbtable {
	int red;
	int green;
	int blue;
};

class Image {
private:
	struct rgbtable _rgbTable[256];
	uint32 _palette[256];
	int _transp;
	uint32 *_offsets;
	int _width;
	int _height;
	Resource *_resource;
	GUI_wxWindows *_gui;

public:
	Image();
	~Image();
	int drawPalette(BlockTable *_blockTable, int id, File& _input);
	int drawBG(File& _input, BlockTable *_blockTable, int id, char* filename);
	int drawObject(File& _input, BlockTable *_blockTable, int id);
	void decode_uncompressed(uint16 height, File& _input);
	void decode_horiz(uint16 height, uint8 compr, File& _input);
	void decode_vert(uint16 height, uint8 compr, File& _input);
	void decode_horiz_transp(uint16 height, uint8 compr, File& _input);
	void decode_vert_transp(uint16 height, uint8 compr, File& _input);
	void decode2(uint16 height, uint8 compr, File& _input);
	void decode2transp(uint16 height, uint8 compr, File& _inpuit);
	void GetStrip( uint8 pos, File& _input);
};

#endif
