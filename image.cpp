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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/image.cpp,v 1.25 2003/11/08 02:38:00 khalek Exp $
 *
 */

#include "file.h"
#include "resource.h"
#include "image.h"
#include "scaler.h"

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

void Image::setupEGAPalette() {
	setPalColor( 0,   0,   0,   0);
	setPalColor( 1,   0,   0, 168);
	setPalColor( 2,   0, 168,   0);
	setPalColor( 3,   0, 168, 168);
	setPalColor( 4, 168,   0,   0);
	setPalColor( 5, 168,   0, 168); 
	setPalColor( 6, 168,  84,   0);
	setPalColor( 7, 168, 168, 168);
	setPalColor( 8,  84,  84,  84);
	setPalColor( 9,  84,  84, 252);
	setPalColor(10,  84, 252,  84);
	setPalColor(11,  84, 252, 252);
	setPalColor(12, 252,  84,  84);
	setPalColor(13, 252,  84, 252);
	setPalColor(14, 252, 252,  84);
	setPalColor(15, 252, 252, 252);
}

void Image::setPalColor(int idx, int r, int g, int b) {
	_rgbTable[idx].red = r;
	_rgbTable[idx].green = g;
	_rgbTable[idx].blue = b;
}

void Image::drawPalette(BlockTable *_blockTable, int id, File& _input)
{
	int addindex = 0;
	int index = 0;
	unsigned int h;

	if (_blockTable[id].image == NULL)
		_image = _blockTable[id].image = new ImageWindow("Block Palette", wxSize(384, 384), id, FLAG_NONE, 1);
	else
		_image = _blockTable[id].image;

	_input.seek(_blockTable[id].offset + 8, SEEK_SET);

	if (_blockTable[id].blockTypeID == AHDR)
		_input.seek(6, SEEK_CUR);

	if (_blockTable[id].blockTypeID == RGBS) {
		for (h = 0; h < (_blockTable[id].blockSize - 8) / 3; h++) {
			_rgbTable[h].red = _input.readByte();	// red
			_rgbTable[h].green = _input.readByte(); // green
			_rgbTable[h].blue = _input.readByte();	// blue
		}
		for (; h < 256; h++) {
			_rgbTable[h].red = 255;
			_rgbTable[h].green = 255;
			_rgbTable[h].blue = 255;
		}
	} else {				
		for (int j = 0; j < 256; j++) {
			_rgbTable[j].red = _input.readByte();	// red
			_rgbTable[j].green = _input.readByte(); // green
			_rgbTable[j].blue = _input.readByte();	// blue
		}
	}

	int x = 0, y = 0;
	for (int v = 0; v < 16; v++) {
		for (int k = 0; k < 384 / 16; k++) {
			for (int l = 0; l < 16; l++) {
				for (int j = 0; j < 384 / 16; j++) {
					index = l + addindex;
					_image->PutPixel(x++, y, _rgbTable[index].red, _rgbTable[index].green, _rgbTable[index].blue);
				}
			}
			y++;
			x = 0;
		}
		addindex += 16;
	}

	_image->DrawImage();
}

void Image::drawLine(int xStart, int yStart, int xEnd, int yEnd, int red, int green, int blue) {

	int i, len, xlen, ylen;
	double x, xinc, y, yinc;

	if (xStart == xEnd && yStart == yEnd) {
		_image->PutPixel(xEnd, yEnd, red, green, blue);
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
		_image->PutPixel((int)x, (int)y, red, green, blue);
		x += xinc;
		y += yinc;
	}

}

void Image::drawBoxes(BlockTable *_blockTable, int id, File& _input) {
	int nBox, headerID, version = 5;

	if (_blockTable[id].blockTypeID == BM || _blockTable[id].blockTypeID == BX) {
		version = 3;
		headerID = _resource->findBlock(0, _blockTable, id, "HD", NULL);
	} else {
		headerID = _resource->findBlock(0, _blockTable, id, "RMHD", NULL);
	}

	_width = _blockTable[headerID].width;
	_height = _blockTable[headerID].height;

	if ( _resource->findBlock(0, _blockTable, id, "IMAG", NULL) != -1) {
		version = 8;
	} else if ( version > 3 && _resource->findBlock(0, _blockTable, id, "PALS", NULL) != -1) {
		version = 7;
	}

	if (_blockTable[id].image == NULL)
		_image = _blockTable[id].image = new ImageWindow("Boxes", wxSize(_width, _height), id, IMAGE_SCALE);
	else
		_image = _blockTable[id].image;

	if (_blockTable[id].blockTypeID != BOXD && _blockTable[id].blockTypeID != BX) {
		if (version > 5) {
			id = _resource->findBlock(1, _blockTable, id, "BOXD", NULL);
		} else {
			id = _resource->findBlock(0, _blockTable, id, "BOXD", "BX", NULL);
		}
	}
	
	if (version == 8) { 
		_input.seek(_blockTable[id].offset + 12, SEEK_SET);
	} else if (version > 3) {
		_input.seek(_blockTable[id].offset + 10, SEEK_SET);
	} else {
		_input.seek(_blockTable[id].offset + 7, SEEK_SET);
	}
	
	nBox = _blockTable[id].numFiles;

	if (version == 8) {
		for (int i=0; i<nBox; i++) {
			_points[i][0].x = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][0].y = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][1].x = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][1].y = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][2].x = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][2].y = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][3].x = (short)_input.readUint32LE() * _image->_scaleFactor;
			_points[i][3].y = (short)_input.readUint32LE() * _image->_scaleFactor;
			_input.seek(20, SEEK_CUR);
		}

	} else if (version == 3 && _resource->findBlock(0, _blockTable, id, "LF", NULL) == -1 && _resource->findBlock(1, _blockTable, id, "PA", NULL) != -1) {
		for (int i=0; i<nBox; i++) {
			_points[i][0].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][0].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][1].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][1].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][2].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][2].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][3].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][3].y = _input.readUint16LE() * _image->_scaleFactor;
			_input.readUint16LE();
		}
	} else {
		for (int i=0; i<nBox; i++) {
			_points[i][0].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][0].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][1].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][1].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][2].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][2].y = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][3].x = _input.readUint16LE() * _image->_scaleFactor;
			_points[i][3].y = _input.readUint16LE() * _image->_scaleFactor;
			_input.readUint16LE();
			_input.readUint16LE();
		}
	}

	for (int i=0; i<nBox; i++) {
		for (int j=0; j<3; j++) {
			drawLine(_points[i][j].x, _points[i][j].y, _points[i][j+1].x, _points[i][j+1].y, 255, 255, 255);
		}
		drawLine(_points[i][3].x, _points[i][3].y, _points[i][0].x, _points[i][0].y, 255, 255, 255);
	}
	
	_image->DrawImage();
}

void Image::drawSmushFrame(BlockTable *_blockTable, int id, File& _input) {
	int index;
	int x = 0, y = 0;
	byte *dst, *dstorg, *chunk_buffer, *dstFinal;

	index = _resource->findBlock(0, _blockTable, id, "NPAL", "AHDR", NULL);
	if (_blockTable[index].blockTypeID == AHDR) {
		_input.seek(_blockTable[index].offset + 14, SEEK_SET);
	} else {
		_input.seek(_blockTable[index].offset + 8, SEEK_SET);
	}

	for (int j = 0; j < 256; j++) {
		_rgbTable[j].red = _input.readByte();
		_rgbTable[j].green = _input.readByte();
		_rgbTable[j].blue = _input.readByte();
	}

	_width = _blockTable[id].width;
	_height = _blockTable[id].height;

	if (_blockTable[id].image == NULL)
		_image = _blockTable[id].image = new ImageWindow("SMUSH Frame", wxSize(_width, _height), id, IMAGE_SCALE);
	else
		_image = _blockTable[id].image;

	_input.seek(_blockTable[id].offset + 22, SEEK_SET);
	_codec37.init(_width, _height);
	_codec47.init(_width, _height);
	chunk_buffer = (byte *)malloc(_blockTable[id].blockSize - 22);
	dstorg = dst = (byte *)malloc(_width * _height + 1000);
	_input.read(chunk_buffer, _blockTable[id].blockSize - 22);

	switch (_blockTable[id].variables) {
		case 1:
			decodeCodec1(dst, chunk_buffer, _height);
			break;

		case 37:
			_codec37.decode(dst, chunk_buffer);
			break;

		case 44:
		case 21:
			decodeCodec44(dst, chunk_buffer, _blockTable[id].blockSize - 22);
			break;

		case 47:
			_codec47.decode(dst, chunk_buffer);
			break;
	}
	free(chunk_buffer);

	if (_image->_scaleFactor > 1) {
		dstorg = dstFinal = (byte *)malloc((_width * _image->_scaleFactor) * (_height * _image->_scaleFactor));
		scale(_image->_scaleFactor, dst, _width, dstFinal, _width * _image->_scaleFactor, _width, _height);
		free(dst);
	} else {
		dstFinal = dst;
	}

	for (y=0; y<_height * _image->_scaleFactor; y++) {
		for (x=0; x<_width * _image->_scaleFactor; x++) {
			int color = *dstFinal++;
			_image->PutPixel(x, y, _rgbTable[color].red, _rgbTable[color].green, _rgbTable[color].blue);
		}
	}
	
	free(dstorg);
	_image->DrawImage();
}

void Image::drawBG(File& _input, BlockTable *_blockTable, int id)
{
	int headerID, paletteID, dataID, version;
	int32 blockSize;
	byte *dst, *dstorg, *src, *dstFinal;

	if (_blockTable[id].blockTypeID == BM) {
		version = 3;
		headerID = _resource->findBlock(0, _blockTable, id, "HD", NULL);
	} else if (_blockTable[id-1].blockTypeID == IMHD) {
		version = 8;
		headerID = id-1;
	} else {
		version = 5;
		headerID = _resource->findBlock(0, _blockTable, id, "RMHD", NULL);
	}
	
	_width = _blockTable[headerID].width;
	_height = _blockTable[headerID].height;

	if (_blockTable[id].image == NULL)
		_image = _blockTable[id].image = new ImageWindow("Room Image", wxSize(_width, _height), id, IMAGE_BOXES|IMAGE_SCALE);
	else
		_image = _blockTable[id].image;

	if (version > 4) {
		paletteID = _resource->findBlock(0, _blockTable, id, "CLUT", "APAL", "NPAL", NULL);
		_input.seek(_blockTable[paletteID].offset + 8, SEEK_SET);
	} else {
		_transp = 260;
		paletteID = _resource->findBlock(0, _blockTable, id, "PA", NULL);
		if (paletteID != -1) {
			version = 4;
			_input.seek(_blockTable[paletteID].offset + 8, SEEK_SET);
		} else {
			version = 3;
		}
	}

	if (version > 3) {
		for (int j = 0; j < 256; j++) {
			_rgbTable[j].red = _input.readByte();
			_rgbTable[j].green = _input.readByte();
			_rgbTable[j].blue = _input.readByte();
		}
	} else {
		setupEGAPalette();
	}

	if (version > 4) {
		dataID = _resource->findBlock(1, _blockTable, id, "SMAP", NULL);
	} else {
		dataID = id;
	}

	blockSize = _blockTable[dataID].blockSize;

	if (_blockTable[id].blockTypeID == IMAG) {
		version = 8;
		dataID = _resource->findBlock(1, _blockTable, dataID, "OFFS", NULL);
		blockSize = _blockTable[dataID-1].blockSize - 8;
		
	}

	_offsets = new uint32[_width/8];
	_input.seek(_blockTable[dataID].offset + 8, SEEK_SET);

	if (version > 4) {
		for (int x = 0; x < _width/8; x++)  
			_offsets[x] = _input.readUint32LE();
	} else if (version == 4) {
		for (int x = 0; x < _width/8; x++) {
			_input.seek(2, SEEK_CUR);
			_offsets[x] = _input.readUint16LE() + 6;
		}
	} else {
		for (int x = 0; x < _width/8; x++) 
			_offsets[x] = _input.readUint16LE() + 6;
	}

	_input.seek(_blockTable[dataID].offset, SEEK_SET);
	dstorg = dst = (byte *)malloc(_width * _height);
	src = (byte *)malloc(blockSize);
	_input.read(src, blockSize);
	
	for (int x = 0; x < _width/8; x++) {
		if (version < 4)
			decodeStripEGA(dst + (8 * x), src + _offsets[x], _height);
		else
			GetStrip(dst + (8 * x), src + _offsets[x], _height);
	}
	free(src);

	if (_image->_scaleFactor > 1) {
		dstorg = dstFinal = (byte *)malloc((_width * _image->_scaleFactor) * (_height * _image->_scaleFactor));
		scale(_image->_scaleFactor, dst, _width, dstFinal, _width * _image->_scaleFactor, _width, _height);
		free(dst);
	} else {
		dstFinal = dst;
	}
	
	for (int y=0; y<_height * _image->_scaleFactor; y++) {
		for (int x=0; x<_width * _image->_scaleFactor; x++) {
			int color = *dstFinal++;
			_blockTable[id].image->PutPixel(x, y, _rgbTable[color].red, _rgbTable[color].green, _rgbTable[color].blue);
		}
	}

	free(dstorg);
		
	_image->DrawImage();
}

void Image::drawObject(File& _input, BlockTable *_blockTable, int id)
{
	int headerID, paletteID, dataID, version = 5;
	byte *dst, *dstorg, *src, *dstFinal;

	if (_blockTable[id].blockTypeID == OBIM) {
		headerID = _resource->findBlock(1, _blockTable, id, "IMHD", NULL);
	} else if (_blockTable[id].blockTypeID == OI) {
		version = 3;
		headerID = id;
		while (1) {
			headerID = _resource->findBlock(1, _blockTable, headerID, "OC", NULL);
				if (_blockTable[headerID].numFiles == _blockTable[id].numFiles)
					break;
		}
		paletteID = _resource->findBlock(0, _blockTable, id, "PA", NULL);
		if (paletteID != -1) {
			version = 4;
			_input.seek(_blockTable[paletteID].offset + 8, SEEK_SET);
		}
	} else {
		headerID = _resource->findBlock(0, _blockTable, id, "IMHD", NULL);
	}
	
	_width = _blockTable[headerID].width;
	_height = _blockTable[headerID].height;

	if (_blockTable[id].image == NULL)
		_image = _blockTable[id].image = new ImageWindow("Object", wxSize(_width, _height), id, IMAGE_SCALE);
	else
		_image = _blockTable[id].image;

	if (version > 4) {
		paletteID = _resource->findBlock(0, _blockTable, id, "CLUT", "APAL", "NPAL", NULL);
		_input.seek(_blockTable[paletteID].offset + 8, SEEK_SET);
	}
	
	if (version > 3) {
		for (int j = 0; j < 256; j++) {
			_rgbTable[j].red = _input.readByte();	// red
			_rgbTable[j].green = _input.readByte();	// green
			_rgbTable[j].blue = _input.readByte();	// blue
		}
	} else {
		setupEGAPalette();
	}
	
	if (version > 4) {
		dataID = _resource->findBlock(1, _blockTable, id, "SMAP", NULL);
		_input.seek(_blockTable[dataID].offset + 8, SEEK_SET);
	} else {
		dataID = id;
		_input.seek(_blockTable[dataID].offset + 10, SEEK_SET);
	}
	
	_offsets = new uint32[_width/8];

	if (version > 4) {
		for (int x = 0; x < _width/8; x++)  
			_offsets[x] = _input.readUint32LE();
	} else if (version == 4) {
		for (int x = 0; x < _width/8; x++) {
			_input.seek(2, SEEK_CUR);
			_offsets[x] = _input.readUint16LE() + 8;
		}
	} else {
		for (int x = 0; x < _width/8; x++) 
			_offsets[x] = _input.readUint16LE() + 8;
	}
	
	_input.seek(_blockTable[dataID].offset, SEEK_SET);
	dstorg = dst = (byte *)malloc(_width * _height);
	src = (byte *)malloc(_blockTable[dataID].blockSize);
	_input.read(src, _blockTable[dataID].blockSize);
	
	for (int x = 0; x < _width/8; x++) {
		if (version > 3)
			GetStrip(dst + (8 * x), src + _offsets[x], _height);
		else
			decodeStripEGA(dst + (8 * x), src + _offsets[x], _height);
	}
	free(src);

	if (_image->_scaleFactor > 1) {
		dstorg = dstFinal = (byte *)malloc((_width * _image->_scaleFactor) * (_height * _image->_scaleFactor));
		scale(_image->_scaleFactor, dst, _width, dstFinal, _width * _image->_scaleFactor, _width, _height);
		free(dst);
	} else {
		dstFinal = dst;
	}

	for (int y=0; y<_height * _image->_scaleFactor; y++) {
		for (int x=0; x<_width * _image->_scaleFactor; x++) {
			int color = *dstFinal++;
			_image->PutPixel(x, y, _rgbTable[color].red, _rgbTable[color].green, _rgbTable[color].blue);
		}
	}
	free(dstorg);
	
	_image->DrawImage();
}

void Image::GetStrip(byte *dst, const byte *src, int numLinesToProcess)
{
	byte code = *src++;
	_decomp_shr = (code % 10);
	_decomp_mask = 0xFF >> (8 - _decomp_shr);
				
	_vertStripNextInc = _height * _width - 1;
	
	switch (code) 
	{
		case 1:
			unkDecode7(dst, src, numLinesToProcess);
			break;
		
		case 2:
			unkDecode8(dst, src, numLinesToProcess);
			break;

		case 3:
			unkDecode9(dst, src, numLinesToProcess);
			break;
		
		case 4:
			unkDecode10(dst, src, numLinesToProcess);
			break;

		case 7:
			unkDecode11(dst, src, numLinesToProcess);
			break;

		case 10:
			decodeStripEGA(dst, src, numLinesToProcess);
			break;
		
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
			unkDecodeC(dst, src, numLinesToProcess);
			break;

		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
			unkDecodeB(dst, src, numLinesToProcess);
			break;
		
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 84:
		case 85:
		case 86:
		case 87:
		case 88:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 124:
		case 125:
		case 126:
		case 127:
		case 128:
			unkDecodeA(dst, src, numLinesToProcess);
			break;
		
		default:
			printf("Unknown codec: %d!\n", code);
	}
	
}

void Image::decodeCodec44(byte *dst, const byte *src, uint32 length) {
	byte val;
	uint16 size_line, num;

	do {
		size_line = READ_LE_UINT16(src);
		src += 2;
		length -= 2;

		while (size_line != 0) {
			num = *src++;
			val = *src++;
			memset(dst, val, num);
			dst += num;
			length -= 2;
			size_line -= 2;
			if (size_line != 0) {
				num = READ_LE_UINT16(src) + 1;
				src += 2;
				memcpy(dst, src, num);
				dst += num;
				src += num;
				length -= num + 2;
				size_line -= num + 2;
			}
		}
		dst--;

	} while (length > 1);
}

void Image::decodeCodec1(byte *dst, byte *src, int height) {
	byte val, code;
	int32 length;
	int h = height, size_line;

	for (h = 0; h < height; h++) {
		size_line = READ_LE_UINT16(src);
		src += 2;
		while (size_line > 0) {
			code = *src++;
			size_line--;
			length = (code >> 1) + 1;
			if (code & 1) {
				val = *src++;
				size_line--;
				if (val)
					memset(dst, val, length);
				dst += length;
			} else {
				size_line -= length;
				while (length--) {
					val = *src++;
					if (val)
						*dst = val;
					dst++;
				}
			}
		}
	}
}

void Image::decodeStripEGA(byte *dst, const byte *src, int height) {
	byte color = 0;
	int run = 0, x = 0, y = 0, z;

	while (x < 8) {
		color = *src++;
		
		if (color & 0x80) {
			run = color & 0x3f;

			if (color & 0x40) {
				color = *src++;

				if (run == 0) {
					run = *src++;
				}
				for (z = 0; z < run; z++) {
					*(dst + y * _width + x) = (z&1) ? color & 0xf : color >> 4;

					y++;
					if (y >= height) {
						y = 0;
						x++;
					}
				}
			} else {
				if (run == 0) {
					run = *src++;
				}

				for (z = 0; z < run; z++) {
					*(dst + y * _width + x) = *(dst + y * _width + x - 1);

					y++;
					if (y >= height) {
						y = 0;
						x++;
					}
				}
			}
		} else {
			run = color >> 4;
			if (run == 0) {
				run = *src++;
			}
			
			for (z = 0; z < run; z++) {
				*(dst + y * _width + x) = color & 0xf;

				y++;
				if (y >= height) {
					y = 0;
					x++;
				}
			}
		}
	}
}

#define READ_BIT (cl--, bit = bits&1, bits>>=1,bit)
#define FILL_BITS do {												\
										if (cl <= 8) {						\
											bits |= (*src++ << cl);	\
											cl += 8;								\
										}													\
									} while (0)

void Image::unkDecodeA(byte *dst, const byte *src, int height) {
	byte color = *src++;
	uint bits = *src++;
	byte cl = 8;
	byte bit;
	byte incm, reps;

	do {
		int x = 8;
		do {
			FILL_BITS;
			*dst++ = color;

		againPos:
			if (!READ_BIT) {
			} else if (!READ_BIT) {
				FILL_BITS;
				color = bits & _decomp_mask;
				bits >>= _decomp_shr;
				cl -= _decomp_shr;
			} else {
				incm = (bits & 7) - 4;
				cl -= 3;
				bits >>= 3;
				if (incm) {
					color = (byte)((color+incm)&0xFF);
				} else {
					FILL_BITS;
					reps = bits & 0xFF;
					do {
						if (!--x) {
							x = 8;
							dst += _width - 8;
							if (!--height)
								return;
						}
						*dst++ = color;
					} while (--reps);
					bits >>= 8;
					bits |= (*src++) << (cl - 8);
					goto againPos;
				}
			}
		} while (--x);
		dst += _width - 8;
	} while (--height);
}

void Image::unkDecodeB(byte *dst, const byte *src, int height) {
	byte color = *src++;
	uint bits = *src++;
	byte cl = 8;
	byte bit;
	int8 inc = -1;

	do {
		int x = 8;
		do {
			FILL_BITS;
			*dst++ = color;
			if (!READ_BIT) {
			} else if (!READ_BIT) {
				FILL_BITS;
				color = bits & _decomp_mask;
				bits >>= _decomp_shr;
				cl -= _decomp_shr;
				inc = -1;
			} else if (!READ_BIT) {
				color += inc;
			} else {
				inc = -inc;
				color += inc;
			}
		} while (--x);
		dst += _width - 8;
	} while (--height);
}

void Image::unkDecodeC(byte *dst, const byte *src, int height) {
	byte color = *src++;
	uint bits = *src++;
	byte cl = 8;
	byte bit;
	int8 inc = -1;

	int x = 8;
	do {
		int h = height;
		do {
			FILL_BITS;
			*dst = color;
			dst += _width;
			if (!READ_BIT) {
			} else if (!READ_BIT) {
				FILL_BITS;
				color = bits & _decomp_mask;
				bits >>= _decomp_shr;
				cl -= _decomp_shr;
				inc = -1;
			} else if (!READ_BIT) {
				color += inc;
			} else {
				inc = -inc;
				color += inc;
			}
		} while (--h);
		dst -= _vertStripNextInc;
	} while (--x);
}

#undef READ_BIT
#undef FILL_BITS

/* Ender - Zak256/Indy256 decoders */
#define READ_256BIT                        \
		do {                               \
			if ((mask <<= 1) == 256) {     \
				buffer = *src++;           \
				mask = 1;                  \
			}                              \
			bits = ((buffer & mask) != 0); \
		} while (0)

#define NEXT_ROW                           \
		do {                               \
			dst += _width;      \
			if (--h == 0) {                \
				if (!--x)                  \
					return;                \
				dst -= _vertStripNextInc;  \
				h = height;                \
			}                              \
		} while (0)

void Image::unkDecode7(byte *dst, const byte *src, int height) {
	uint h = height;

	//if (_vm->_features & GF_OLD256) {
		int x = 8;
		for (;;) {
			*dst = *src++;
			NEXT_ROW;
		}
		return;
	//}

	do {
#if defined(SCUMM_NEED_ALIGNMENT)
		memcpy(dst, src, 8);
#else
		((uint32 *)dst)[0] = ((const uint32 *)src)[0];
		((uint32 *)dst)[1] = ((const uint32 *)src)[1];
#endif
		dst += _width;
		src += 8;
	} while (--height);
}

void Image::unkDecode8(byte *dst, const byte *src, int height) {
	uint h = height;

	int x = 8;
	for (;;) {
		uint run = (*src++) + 1;
		byte color = *src++;

		do {
			*dst = color;
			NEXT_ROW;
		} while (--run);
	}
}

void Image::unkDecode9(byte *dst, const byte *src, int height) {
	unsigned char c, bits, color, run;
	int i, j;
	uint buffer = 0, mask = 128;
	int h = height;
	i = j = run = 0;

	int x = 8;
	for (;;) {
		c = 0;
		for (i = 0; i < 4; i++) {
			READ_256BIT;
			c += (bits << i);
		}

		switch (c >> 2) {
		case 0:
			color = 0;
			for (i = 0; i < 4; i++) {
				READ_256BIT;
				color += bits << i;
			}
			for (i = 0; i < ((c & 3) + 2); i++) {
				*dst = (run * 16 + color);
				NEXT_ROW;
			}
			break;

		case 1:
			for (i = 0; i < ((c & 3) + 1); i++) {
				color = 0;
				for (j = 0; j < 4; j++) {
					READ_256BIT;
					color += bits << j;
				}
				*dst = (run * 16 + color);
				NEXT_ROW;
			}
			break;

		case 2:
			run = 0;
			for (i = 0; i < 4; i++) {
				READ_256BIT;
				run += bits << i;
			}
			break;
		}
	}
}

void Image::unkDecode10(byte *dst, const byte *src, int height) {
	int i;
	unsigned char local_palette[256], numcolors = *src++;
	uint h = height;

	for (i = 0; i < numcolors; i++)
		local_palette[i] = *src++;

	int x = 8;

	for (;;) {
		byte color = *src++;
		if (color < numcolors) {
			*dst = local_palette[color];
			NEXT_ROW;
		} else {
			uint run = color - numcolors + 1;
			color = *src++;
			do {
				*dst = color;
				NEXT_ROW;
			} while (--run);
		}
	}
}

void Image::unkDecode11(byte *dst, const byte *src, int height) {
	int bits, i;
	uint buffer = 0, mask = 128;
	unsigned char inc = 1, color = *src++;
	int x = 8;
	do {
		int h = height;
		do {
			*dst = color;
			dst += _width;
			for (i = 0; i < 3; i++) {
				READ_256BIT;
				if (!bits)
					break;
			}
			switch (i) {
			case 1:
				inc = -inc;
				color -= inc;
				break;

			case 2:
				color -= inc;
				break;

			case 3:
				color = 0;
				inc = 1;
				for (i = 0; i < 8; i++) {
					READ_256BIT;
					color += bits << i;
				}
				break;
			}
		} while (--h);
		dst -= _vertStripNextInc;
	} while (--x);
}
