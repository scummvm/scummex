/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2003 The ScummVM project
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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/codec37.h,v 1.1 2003/09/21 23:50:28 yoshizf Exp $
 *
 */

#ifndef SMUSH_CODEC37_H
#define SMUSH_CODEC37_H

#include "scummsys.h"

class Codec37Decoder {
private:

	int32 _deltaSize;
	byte *_deltaBufs[2];
	byte *_deltaBuf;
	int16 *_offsetTable;
	int _curtable;
	uint16 _prevSeqNb;
	int _tableLastPitch;
	int _tableLastIndex;
	int32 _frameSize;
	int _width, _height;

public:
	Codec37Decoder();
	~Codec37Decoder();
	void init(int width, int height);
	void deinit();
protected:
	void maketable(int, int);
	void proc3WithFDFE(byte *dst, const byte *src, int32, int, int, int, int16 *);
	void proc3WithoutFDFE(byte *dst, const byte *src, int32, int, int, int, int16 *);
	void proc4WithFDFE(byte *dst, const byte *src, int32, int, int, int, int16 *);
	void proc4WithoutFDFE(byte *dst, const byte *src, int32, int, int, int, int16 *);
public:
	void decode(byte *dst, const byte *src);
};

#endif
