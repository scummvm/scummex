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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/image.h,v 1.13 2003/09/27 14:56:11 yoshizf Exp $
 *
 */

#ifndef image_h
#define image_h

#include "scummsys.h"
#include "codec37.h"
#include "codec47.h"
#include "bomp.h"

struct rgbtable {
	int red;
	int green;
	int blue;
};

struct points {
	uint x;
	uint y;
};

class Image {
private:
	struct rgbtable _rgbTable[256];
	struct points _points[40][4];
	uint32 _palette[256];
	int _transp;
	uint32 *_offsets;
	int _width;
	int _height;
	int _imageWindowId;
	byte _decomp_shr;
	byte _decomp_mask;
	int _vertStripNextInc;
	Resource *_resource;
	Codec37Decoder _codec37;
	Codec47Decoder _codec47;
	ImageWindow *_image;

	void drawLine(int xStart, int yStart, int xEnd, int yEnd, int red, int green, int blue);
	void setupEGAPalette();
	void setPalColor(int idx, int r, int g, int b);
	void GetStrip(byte *dst, const byte *src, int numLinesToProcess);
	void decodeCodec44(byte *dst, const byte *src, uint32 length);
	void decodeCodec1(byte *dst, byte *src, int height);
	void decodeStripEGA(byte *dst, const byte *src, int height);
	void unkDecodeA(byte *dst, const byte *src, int height);
	void unkDecodeB(byte *dst, const byte *src, int height);
	void unkDecodeC(byte *dst, const byte *src, int height);
	void unkDecode7(byte *dst, const byte *src, int height);
	void unkDecode8(byte *dst, const byte *src, int height);
	void unkDecode9(byte *dst, const byte *src, int height);
	void unkDecode10(byte *dst, const byte *src, int height);
	void unkDecode11(byte *dst, const byte *src, int height);

public:
	Image();
	~Image();
	void drawPalette(BlockTable *_blockTable, int id, File& _input);
	void drawBG(File& _input, BlockTable *_blockTable, int id);
	void drawObject(File& _input, BlockTable *_blockTable, int id);
	void drawSmushFrame(BlockTable *_blockTable, int id, File& _input);
	void drawBoxes(BlockTable *_blockTable, int id, File& _input);
};

#endif
