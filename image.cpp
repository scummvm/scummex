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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/image.cpp,v 1.4 2003/09/19 15:47:41 fingolfin Exp $
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

int Image::drawBG(File& _input, BlockTable *_blockTable, int id, char* filename)
{
	int RMHDindex, CLUTindex, SMAPindex, TRNSindex;
	
	if (_blockTable[id].blockTypeID == 209) {
		RMHDindex = _resource->findBlock(0, _blockTable, _input, id, "HD", "-1");
	} else {
		RMHDindex = _resource->findBlock(0, _blockTable, _input, id, "RMHD", "-1");
	}
	_width = _blockTable[RMHDindex].width;
	_height = _blockTable[RMHDindex].height;

	_gui->DisplayImage("Room Image", _width, _height);
	
	if (_blockTable[id].blockTypeID != 209) {
		TRNSindex = _resource->findBlock(0, _blockTable, _input, id, "TRNS", "-1");
		_transp = _blockTable[TRNSindex].trans;
		CLUTindex = _resource->findBlock(0, _blockTable, _input, id, "CLUT", "APAL", "NPAL", "-1");
		_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);
	} else {
		_transp = 260;
		CLUTindex = _resource->findBlock(0, _blockTable, _input, id, "PA", "-1");
		_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);
	}

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte();	// green
		_rgbTable[j].blue = _input.readByte();	// blue
	}
	
	if (_blockTable[id].blockTypeID != 209) {
		SMAPindex = _resource->findBlock(1, _blockTable, _input, id, "SMAP", "-1");
	} else {
		SMAPindex = id;
	}

	_offsets = new uint32[_width/8];
	
	if (_blockTable[id].blockTypeID != 209) {
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

	RMHDindex = _resource->findBlock(1, _blockTable, _input, id, "IMHD", "-1");
	
	_width = _blockTable[RMHDindex].width;
	_height = _blockTable[RMHDindex].height;
	
	_gui->DisplayImage("Object", _width, _height);
	
	TRNSindex = _resource->findBlock(0, _blockTable, _input, id, "TRNS", "-1");

	_transp = _blockTable[TRNSindex].trans;
	
	CLUTindex = _resource->findBlock(0, _blockTable, _input, id, "CLUT", "APAL", "NPAL", "-1");

	_input.seek(_blockTable[CLUTindex].offset + 8, SEEK_SET);

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();	// red
		_rgbTable[j].green = _input.readByte();	// green
		_rgbTable[j].blue = _input.readByte();	// blue
	}
	
	SMAPindex = _resource->findBlock(1, _blockTable, _input, id, "SMAP", "-1");

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
