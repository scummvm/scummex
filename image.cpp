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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/image.cpp,v 1.11 2003/09/23 00:47:23 fingolfin Exp $
 *
 */

#include "file.h"
#include "resource.h"
#include "image.h"

uint32 offset;

Image::Image() {
	_transp = 0;
	_width = 0;
	_height = 0;
	_resource = new Resource();
}

Image::~Image() {
	delete _resource;
}

int Image::drawPalette(BlockTable *_blockTable, int id, File& _input)
{
	int addindex = 0;
	int index = 0;

	_gui->DisplayImage("Block Palette", 384, 384);

	_input.seek(_blockTable[id].offset + 8, SEEK_SET);

	if (_blockTable[id].blockTypeID == AHDR)
		_input.seek(6, SEEK_CUR);

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte(); // green
		_rgbTable[j].blue = _input.readByte();	// blue
	}

	int x = 0, y = 0;
	for (int v = 0; v < 16; v++) {
		for (int k = 0; k < 384 / 16; k++) {
			for (int l = 0; l < 16; l++) {
				for (int j = 0; j < 384 / 16; j++) {
					index = l + addindex;
					_gui->PutPixel(x++, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
			}
			y++;
			x = 0;
		}
		addindex += 16;
	}

	_gui->DrawImage();
	return 0;
}

void Image::drawLine(int xStart, int yStart, int xEnd, int yEnd, int red, int green, int blue) {

	int i, len, xlen, ylen;
	double x, xinc, y, yinc;

	if (xStart == xEnd && yStart == yEnd) {
		_gui->PutPixel(xEnd, yEnd, red, green, blue);
		return;
	}

	xlen = xEnd - xStart;
	ylen = yEnd - yStart;

	if (abs(xlen) > abs(ylen)) {
		len = abs(xlen);
	} else {
		len = abs(ylen);
	}

	xinc = xlen / (double) len;
	yinc = ylen / (double) len;
	x = xStart + 0.5;
	y = yStart + 0.5;

	for (i=0; i<=len; ++i) { 
		_gui->PutPixel((int)x, (int)y, red, green, blue);
		x += xinc;
		y += yinc;
	}

}

int Image::drawBoxes(BlockTable *_blockTable, int id, File& _input, int newWindow) {
	int nBox, RMHDindex, width, height, v8 = 0;

	RMHDindex = _resource->findBlock(0, _blockTable, id, "RMHD", NULL);
	width = _blockTable[RMHDindex].width;
	height = _blockTable[RMHDindex].height;

	if (newWindow == 0) {
		if ( _resource->findBlock(0, _blockTable, id, "IMAG", NULL) != -1) {
			v8 = 1;
			id = _resource->findBlock(1, _blockTable, id, "BOXD", NULL);
		} else {
			id = _resource->findBlock(0, _blockTable, id, "BOXD", NULL);
		}
	}
	
	if (v8) { 
		_input.seek(_blockTable[id].offset + 12, SEEK_SET);
	} else {
		_input.seek(_blockTable[id].offset + 10, SEEK_SET);
	}
	
	nBox = _blockTable[id].numFiles;

	if (v8) {
		for (int i=0; i<nBox; i++) {
			_points[i][0].x = (short)_input.readUint32LE();
			_points[i][0].y = (short)_input.readUint32LE();
			_points[i][1].x = (short)_input.readUint32LE();
			_points[i][1].y = (short)_input.readUint32LE();
			_points[i][2].x = (short)_input.readUint32LE();
			_points[i][2].y = (short)_input.readUint32LE();
			_points[i][3].x = (short)_input.readUint32LE();
			_points[i][3].y = (short)_input.readUint32LE();
			_input.seek(20, SEEK_CUR);
		}
	} else {
		for (int i=0; i<nBox; i++) {
			_points[i][0].x = _input.readUint16LE();
			_points[i][0].y = _input.readUint16LE();
			_points[i][1].x = _input.readUint16LE();
			_points[i][1].y = _input.readUint16LE();
			_points[i][2].x = _input.readUint16LE();
			_points[i][2].y = _input.readUint16LE();
			_points[i][3].x = _input.readUint16LE();
			_points[i][3].y = _input.readUint16LE();
			_input.readUint16LE();
			_input.readUint16LE();
		}
	}
	

	if (newWindow == 1) {
		_gui->DisplayImage("Boxes", width, height);
	}
	
	for (int i=0; i<nBox; i++) {
		for (int j=0; j<3; j++) {
			drawLine(_points[i][j].x, _points[i][j].y, _points[i][j+1].x, _points[i][j+1].y, 255, 255, 255);
		}
		drawLine(_points[i][3].x, _points[i][3].y, _points[i][0].x, _points[i][0].y, 255, 255, 255);
	}
	
	if (newWindow == 1) {
		_gui->DrawImage();
	} else {
		_gui->UpdateImage();
	}
	return 0;
}

int Image::drawSmushFrame(BlockTable *_blockTable, int id, File& _input) {
	int index;
	int x = 0, y = 0;
	byte *dst, *dstorg, *chunk_buffer;

	index = _resource->findBlock(0, _blockTable, id, "NPAL", "AHDR", NULL);
	if (_blockTable[index].blockTypeID == AHDR) {
		_input.seek(_blockTable[index].offset + 14, SEEK_SET);
	} else {
		_input.seek(_blockTable[index].offset + 8, SEEK_SET);
	}

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte(); // green
		_rgbTable[j].blue = _input.readByte();	// blue
	}

	_input.seek(_blockTable[id].offset + 22, SEEK_SET);
	_codec37.init(_blockTable[id].width, _blockTable[id].height);
	_codec47.init(_blockTable[id].width, _blockTable[id].height);
	chunk_buffer = (byte *)malloc(_blockTable[id].blockSize - 22);
	dstorg = dst = (byte *)malloc(_blockTable[id].width*_blockTable[id].height);
	_input.read(chunk_buffer, _blockTable[id].blockSize - 22);

	switch (_blockTable[id].variables) {
		case 1:
			bompDecodeLine(dst, chunk_buffer, _blockTable[id].blockSize - 22);
			break;

		case 37:
			_codec37.decode(dst, chunk_buffer);
			break;

		case 47:
			_codec47.decode(dst, chunk_buffer);
			break;
	}

	_gui->DisplayImage("SMUSH Frame", _blockTable[id].width, _blockTable[id].height);
	
	for (y=0; y<_blockTable[id].height; y++) {
		for (x=0; x<_blockTable[id].width; x++) {
			int color = *dst++;
			_gui->PutPixel(x, y, _rgbTable[color].red, _rgbTable[color].green, _rgbTable[color].blue);
		}
	}
	
	free(chunk_buffer);
	free(dstorg);
	_gui->DrawImage();
	return 0;
}

int Image::drawBG(File& _input, BlockTable *_blockTable, int id)
{
	int RMHDindex, CLUTindex, SMAPindex, TRNSindex;
	
	if (_blockTable[id].blockTypeID == BM) {
		RMHDindex = _resource->findBlock(0, _blockTable, id, "HD", NULL);
	} else if (_blockTable[id-1].blockTypeID == IMHD) {
		RMHDindex = id-1;
	} else {
		RMHDindex = _resource->findBlock(0, _blockTable, id, "RMHD", NULL);
	}
	_width = _blockTable[RMHDindex].width;
	_height = _blockTable[RMHDindex].height;

	_gui->DisplayImage("Room Image", _width, _height, IMAGE_BOXES);
	
	if (_blockTable[id].blockTypeID != BM) {
		TRNSindex = _resource->findBlock(0, _blockTable, id, "TRNS", NULL);
		_transp = _blockTable[TRNSindex].trans;
		CLUTindex = _resource->findBlock(0, _blockTable, id, "CLUT", "APAL", "NPAL", NULL);
		_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);
	} else {
		_transp = 260;
		CLUTindex = _resource->findBlock(0, _blockTable, id, "PA", NULL);
		_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);
	}

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte();	// green
		_rgbTable[j].blue = _input.readByte();	// blue
	}
	
	if (_blockTable[id].blockTypeID != BM) {
		SMAPindex = _resource->findBlock(1, _blockTable, id, "SMAP", NULL);
	} else {
		SMAPindex = id;
	}

	if (_blockTable[id].blockTypeID == IMAG) {
		SMAPindex = _resource->findBlock(1, _blockTable, SMAPindex, "OFFS", NULL);
	}

	_offsets = new uint32[_width/8];
	
	if (_blockTable[id].blockTypeID != BM) {
		_input.seek(_blockTable[SMAPindex].offset + 8, SEEK_SET);
	} else {
		_input.seek(_blockTable[SMAPindex].offset + 10, SEEK_SET);
	}

	for (int x = 0; x < _width/8; x++) 
		_offsets[x] = _input.readUint32LE() + _blockTable[SMAPindex].offset;

	for (int x = 0; x < _width/8; x++)
	{
		offset = x*8;

		GetStrip(x, _input);
	}

	_gui->DrawImage();

	return 0;
}

int Image::drawObject(File& _input, BlockTable *_blockTable, int id)
{
	int RMHDindex, CLUTindex, SMAPindex, TRNSindex;

	RMHDindex = _resource->findBlock(1, _blockTable, id, "IMHD", NULL);
	
	_width = _blockTable[RMHDindex].width;
	_height = _blockTable[RMHDindex].height;
	
	_gui->DisplayImage("Object", _width, _height);
	
	TRNSindex = _resource->findBlock(0, _blockTable, id, "TRNS", NULL);

	_transp = _blockTable[TRNSindex].trans;
	
	CLUTindex = _resource->findBlock(0, _blockTable, id, "CLUT", "APAL", "NPAL", NULL);

	_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte();	// green
		_rgbTable[j].blue = _input.readByte();	// blue
	}
	
	SMAPindex = _resource->findBlock(1, _blockTable, id, "SMAP", NULL);

	_offsets = new uint32[_width/8];
	
	_input.seek(_blockTable[SMAPindex].offset + 8, SEEK_SET);
	for (int x = 0; x < _width/8; x++) 
		_offsets[x] = _input.readUint32LE() + _blockTable[SMAPindex].offset;

	for (int x = 0; x < _width/8; x++)
	{
		offset = x*8;

		GetStrip(x, _input);
	}
	
	_gui->DrawImage();
	return 0;
}

void Image::GetStrip( uint8 pos, File& _input)
{
	uint8 compr_method, parameter;
	bool horiz;	
		
	_input.seek(_offsets[pos], SEEK_SET);
	
	/* Initializes bit stream */
	_input.getbit(0);
					
	/* and parameter.					 */
	compr_method = _input.readByte();	
	parameter = (compr_method % 10);
				
	horiz = 1 - ((compr_method / 10) & 1);
	
	switch (compr_method / 10) 
	{
		case 0:
			/* Uncompressed */
			decode_uncompressed(_height, _input);
			break;
					
		case 1:
	 	case 2:
	 	case 3:
	 	case 4:	 
	 	
			/* 1st compression method */
			if (horiz)
				decode_horiz(_height, parameter, _input);
			else 
				decode_vert(_height, parameter, _input);
			break;
	 
		default:
			/* 2nd compression method */
			if ((compr_method >= 0x54) && (compr_method <= 0x60))
				decode2transp(_height, parameter, _input); 
			else
				decode2(_height, parameter, _input);
			break;
	}
	
}

/* Decoding Functions */

void Image::decode_uncompressed(uint16 height, File& _input)
{
	uint8 index = 0;
	for (uint16 y = 0; y < height; y++) 
	{
		for (uint8 x = 0; x < 8; x++)
		{
			index = _input.readByte();
			_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
		}
	 }
}

void Image::decode2(uint16 height, uint8 parameter, File& _input)
{	
	uint8 index = 0;
		
	for (uint16 y = 0; y < height; y++) 
		{
			uint8 x = 0;
			if (y == 0) 
			{ 
				index = _input.readByte();
				_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				x++; 
			}	
			while ( x < 8 ) 
			{				
				if (_input.getbit(1) == 0) 
					_gui->PutPixel(x++ + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				else 
				{
					if (_input.getbit(1) == 0) 
					{
						index = 0;
						for (uint8 cx = 0; cx < parameter; cx++) 
							index += (_input.getbit(1) << cx);
						_gui->PutPixel(x++ + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}
					else
					{						
						uint8 command = 0;
						for (uint8 val = 0; val < 3; val++) 
							command += (_input.getbit(1) << val);				
					
						if (command == 4) 
						{
						 	uint8 run = 0;
						 	for (uint8 bits = 0; bits < 8; bits++) 
						 		run += (_input.getbit(1) << bits);	 					 	
						 	for (uint8 c = 0; c < run; c++) 
						 	{
						 		if (x > 7)
						 		{
						 			x = 0;
						 			y++;
						 		} 
						 		_gui->PutPixel(x++ + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);						 		
						 	}
						}
						else 
						{
						 	index += command - 4;
							_gui->PutPixel(x++ + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
						}							
					}
				} 
			}
		}
}

void Image::decode2transp(uint16 height, uint8 parameter, File& _input)
{
	uint8 index = 0;
		
	for (uint16 y = 0; y < height; y++) 
		{
			uint8 x = 0;
			if (y == 0) 
			{ 
				index = _input.readByte();
				if (index != _transp)
					_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
				x++;
			}	
			while ( x < 8 ) 
			{										
				if (_input.getbit(1) == 0) 
					_gui->PutPixel(x++ + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				else 
					if (_input.getbit(1) == 0) 
					{
						index = 0;
						for (uint8 cx = 0; cx < parameter; cx++) 
							index += (_input.getbit(1) << cx);
						if (index != _transp)
							_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
						x++;
					}
					else
					{						
						uint8 command = 0;
						for (uint8 val = 0; val < 3; val++) 
							command += (_input.getbit(1) << val);				
						if (command < 4) 
						{
							index -= 4-command;
							if (index != _transp)
								_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
							x++;
						}
						else if (command == 4) 
						{
						 	uint8 run = 0;
						 	for (uint8 bits = 0; bits < 8; bits++) 
						 		run += (_input.getbit(1) << bits);									 	 					 	
						 	for (uint8 c = 0; c < run; c++) 
						 	{	
						 		if (x == 8) 
								{ 
									x = 0; 
									y++; 
								}
								if (index != _transp)
									_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
								x++;
						 		
						 	}
						}
						else 
						{
						 	index += command-4;
							if (index != _transp)
								_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
							x++;
						}							
					}
				} 
		}
}

void Image::decode_horiz(uint16 height, uint8 parameter, File& _input)
{	
	uint8 index = 0;
	int subt = 1;
		
	for (uint16 y = 0; y < height; y++ ) 
	{
		for (uint8 x = 0 ; x < 8; x++) 
		{
			if ((y == 0) && (x == 0)) 
			{
				index = _input.readByte();
				_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
				x++;
			}				
			if (_input.getbit(1) == 0) 
				_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
			else 
			{
				if (_input.getbit(1) == 0) 
				{
					index = 0;
				 	for (uint8 cx = 0; cx < parameter; cx++)
						index += (_input.getbit(1) << cx );
					subt = 1;
				 	_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
				else
				{	
				 	if (_input.getbit(1) == 0) 
				 	{						 
						index -= subt;
						_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}	
					else
					{
						subt =- subt;
						index -= subt;
						_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}
				} 
			} 
		} 
	}
}

void Image::decode_vert(uint16 height, uint8 parameter, File& _input)
{
	uint8 index = 0;
	int8 subt = 1;
		
	for (uint16 y = 0; y < 8; y++ ) 
	{
		for (uint16 x = 0 ; x < height; x++) 
		{
			if ((y == 0) && (x == 0)) 
			{
				index = _input.readByte();
				_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
				x++;
			}				
			if (_input.getbit(1) == 0) 
				_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
			else 
			{
				if (_input.getbit(1) == 0) 
				{
					index = 0;
				 	for (uint8 cx = 0; cx < parameter; cx++) 
						index += (_input.getbit(1) << cx );
					subt = 1;
				 	_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
				else
				{	
				 	if (_input.getbit(1) == 0) 
				 	{						 
						index -= subt;
						_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}	
					else
					{
						subt =- subt;
						index -= subt;
						_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}
				} 
			} 
		} 
	}
}

void Image::decode_horiz_transp(uint16 height, uint8 parameter, File& _input)
{
	uint8 index = 0;
	int8 subt = 1;
		
	for (uint16 y = 0; y < height; y++ ) 
	{
		for (uint8 x = 0 ; x < 8; x++) 
		{
			if ((y == 0) && (x == 0)) 
			{
				index = _input.readByte();
				if (index != _transp)
					_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
				x++;
			}				
			if (_input.getbit(1) == 0) 
				if (index != _transp)
					_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
			else 
			{
				if (_input.getbit(1) == 0) 
				{
					index = 0;
				 	for (uint8 cx = 0; cx < parameter; cx++) 
						index += (_input.getbit(1) << cx );
					subt = 1;
				 	if (index != _transp)
				 		_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
				else
				{	
				 	if (_input.getbit(1) == 0) 
				 	{						 
						index -= subt;
						if (index != _transp)
							_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}	
					else
					{
						subt =- subt;
						index -= subt;
						if (index != _transp)
							_gui->PutPixel(x + offset, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}
				} 
			} 
		} 
	}
}

void Image::decode_vert_transp(uint16 height, uint8 parameter, File& _input)	
{
	uint8 index = 0;
	int8 subt = 1;
		
	for (uint16 y = 0; y < 8; y++ ) 
	{
		for (uint8 x = 0 ; x < height; x++) 
		{
			if ((y == 0) && (x == 0)) 
			{
				index = _input.readByte();
				if (index != _transp)
					_gui->PutPixel(0 + offset, 0, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue); 
				x++;
			}				
			if (_input.getbit(1) == 0) 
				if (index != _transp)
					_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
			else 
			{
				if (_input.getbit(1) == 0) 
				{
					index = 0;
				 	for (uint8 cx = 0; cx < parameter; cx++) 
						index += (_input.getbit(1) << cx );
					subt = 1;
				 	if (index != _transp)
				 		_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
				else
				{	
				 	if (_input.getbit(1) == 0) 
				 	{						 
						index -= subt;
						if (index != _transp)
							_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}	
					else
					{
						subt =- subt;
						index -= subt;
						if (index != _transp)
							_gui->PutPixel(y + offset, x, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
					}
				} 
			} 
		} 
	}
}
