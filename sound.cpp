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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/Attic/sound.cpp,v 1.2 2003/09/18 19:37:14 fingolfin Exp $
 *
 */

#include "sound.h"

static const int16 imcTable[] = {
    0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x0010,
    0x0011,
    0x0013, 0x0015, 0x0017, 0x0019, 0x001C, 0x001F, 0x0022, 0x0025, 0x0029,
    0x002D,
    0x0032, 0x0037, 0x003C, 0x0042, 0x0049, 0x0050, 0x0058, 0x0061, 0x006B,
    0x0076,
    0x0082, 0x008F, 0x009D, 0x00AD, 0x00BE, 0x00D1, 0x00E6, 0x00FD, 0x0117,
    0x0133,
    0x0151, 0x0173, 0x0198, 0x01C1, 0x01EE, 0x0220, 0x0256, 0x0292, 0x02D4,
    0x031C,
    0x036C, 0x03C3, 0x0424, 0x048E, 0x0502, 0x0583, 0x0610, 0x06AB, 0x0756,
    0x0812,
    0x08E0, 0x09C3, 0x0ABD, 0x0BD0, 0x0CFF, 0x0E4C, 0x0FBA, 0x114C, 0x1307,
    0x14EE,
    0x1706, 0x1954, 0x1BDC, 0x1EA5, 0x21B6, 0x2515, 0x28CA, 0x2CDF, 0x315B,
    0x364B,
    0x3BB9, 0x41B2, 0x4844, 0x4F7E, 0x5771, 0x602F, 0x69CE, 0x7462, 0x7FFF
};

static const byte imxOtherTable[6][128] = {
    {
     0xFF, 0x04, 0xFF, 0x04},

    {
     0xFF, 0xFF, 0x02, 0x08, 0xFF, 0xFF, 0x02, 0x08},

    {
     0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x02, 0x04, 0x06,
     0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x02, 0x04, 0x06},

    {
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x04, 0x06, 0x08, 0x0C, 0x10, 0x20,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x04, 0x06, 0x08, 0x0C, 0x10, 0x20},

    {
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
     0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x20,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
     0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x20},

    {
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
     0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
     0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
     0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
     0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
     0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
     0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20}
};

const byte imxShortTable[] = {
    0, 0, 1, 3, 7, 15, 31, 63
};

	byte _destImcTable[93];
	uint32 _destImcTable2[5697];
	CompTable *_compMusicTable;


Sound::Sound() {

	_mixer = new Mixer();
}

Sound::~Sound() {
	delete _mixer;
}

void Sound::parseSOU(BlockTable *_blockTable, File& _input) {

	int index = 0, level = 1, block_type;
	uint size = 0;
	int rate, comp, real_rate;
	byte work[8];
	char ident[8];
	int filesize, curpos = 0;
	
	enum {
		SOUND_HEADER_SIZE = 26,
		SOUND_HEADER_BIG_SIZE = 26 + 8
	};

	filesize = _input.size();
	
	// SOU block
	_input.read(_blockTable[index].blockName, 3);
	_blockTable[index].blockName[3] = '\0';
	_input.seek(5, SEEK_CUR);
	strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
	_blockTable[index].blockTypeID = _resource->getBlockType(_blockTable[index].blockName);
	strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);
	_blockTable[index].blockSize = filesize;
	_blockTable[index].offset = 0;

	_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
	index++;
	level++;
	
	while (curpos < filesize) {
		// VCTL block
		while (memcmp(ident, "VCTL", 4)) {
			_input.read(ident, 4);
		}
		sprintf(_blockTable[index].blockName, "VCTL");
		strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
		_blockTable[index].blockTypeID = _resource->getBlockType(_blockTable[index].blockName);
		strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);
		_blockTable[index].offset = _input.pos() - 4;

		int ret = 0;
		int count = 4;
		while (count > 0) {
			int c = _input.readByte();
			ret <<= 8;
			ret |= c;
			count--;
		}
		_blockTable[index].blockSize = ret;

		_input.seek(_blockTable[index].blockSize-8, SEEK_CUR);
		_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
		index++;
	
		// Crea or VTLK block
		if (_input.read(ident, 8) != 8)
			return;

		_blockTable[index].offset = _input.pos() - 8;

		if (!memcmp(ident, "VTLK", 4)) {
			sprintf(_blockTable[index].blockName, "VTLK");
			_input.seek(SOUND_HEADER_BIG_SIZE - 8, SEEK_CUR);
		} else if (!memcmp(ident, "Creative", 8)) {
			sprintf(_blockTable[index].blockName, "Crea");
			_input.seek(SOUND_HEADER_SIZE - 8, SEEK_CUR);
		} else {
			printf("parseSOU: invalid header at pos: %d\n", _input.pos() - 8);
			return;
		}
		
		_blockTable[index].blockTypeID = _resource->getBlockType(_blockTable[index].blockName);
		strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);
		strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
	
		block_type = _input.readByte();
		if (block_type != 1) {
			printf("Expecting block_type == 1, got %d\n", block_type);
			return;
		}
		
		_input.read(work, 3);

		size = (work[0] | (work[1] << 8) | (work[2] << 16)) - 2;
		size += 28;
		_blockTable[index].blockSize = size;
		rate = _input.readByte();
		comp = _input.readByte();
	
		if (comp != 0) {
			printf("Unsupported compression type %d\n", comp);
			return;
		}

		real_rate = 1000000 / (256 - rate);

		_input.seek(_blockTable[index].blockSize + _blockTable[index].offset + 5, SEEK_SET);
	
		curpos = _input.pos();
	
		_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
		index++;
	}
	
}

int Sound::playSOU(BlockTable *_blockTable, File& _input, int index, File& _output, int save)
{
	SDL_RWops *rw;
	byte *data;

	_input.seek(_blockTable[index].offset, SEEK_SET);

	data = (byte *)malloc(_blockTable[index].blockSize);
	
	if (_input.read(data, _blockTable[index].blockSize) != _blockTable[index].blockSize) {
		printf("cannot read %d bytes", _blockTable[index].blockSize);
	}

	rw = SDL_RWFromMem(data, _blockTable[index].blockSize);
	_mixer->playSound(rw, 1);
	free(data);

	return 0;
}

void initializeImcTables()
{
    int32 destTablePos = 0;
    int32 imcTable1Pos = 0;
    do {
	byte put = 1;
	int32 tableValue = ((imcTable[imcTable1Pos] << 2) / 7) >> 1;
	if (tableValue != 0) {
	    do {
		tableValue >>= 1;
		put++;
	    }
	    while (tableValue != 0);
	}
	if (put < 3) {
	    put = 3;
	}
	if (put > 8) {
	    put = 8;
	}
	put--;
	_destImcTable[destTablePos] = put;
	destTablePos++;
    }
    while (++imcTable1Pos <= 88);
    _destImcTable[89] = 0;

    for (int n = 0; n < 64; n++) {
	imcTable1Pos = 0;
	destTablePos = n;
	do {
	    int32 count = 32;
	    int32 put = 0;
	    int32 tableValue = imcTable[imcTable1Pos];
	    do {
		if ((count & n) != 0) {
		    put += tableValue;
		}
		count >>= 1;
		tableValue >>= 1;
	    }
	    while (count != 0);
	    _destImcTable2[destTablePos] = put;
	    destTablePos += 64;
	}
	while (++imcTable1Pos <= 88);
    }
}


#define NextBit bit = mask & 1; mask >>= 1;				\
								if (!--bitsleft) {								\
									mask = READ_LE_UINT16(srcptr);	\
									srcptr += 2;										\
									bitsleft = 16;									\
								}

int32 Sound::compDecode(byte * src, byte * dst)
{
    byte *result, *srcptr = src, *dstptr = dst;
    int data, size, bit, bitsleft = 16, mask = READ_LE_UINT16(srcptr);
    srcptr += 2;

    while (1) {
	NextBit if (bit) {
	    *dstptr++ = *srcptr++;
	} else {
	    NextBit if (!bit) {
		NextBit size = bit << 1;
		NextBit size = (size | bit) + 3;
		data = *srcptr++ | 0xffffff00;
	    } else {
		data = *srcptr++;
		size = *srcptr++;

		data |= 0xfffff000 + ((size & 0xf0) << 4);
		size = (size & 0x0f) + 3;

		if (size == 3)
		    if (((*srcptr++) + 1) == 1)
			return dstptr - dst;
	    }
	    result = dstptr + data;
	    while (size--)
		*dstptr++ = *result++;
	}
    }
}

#undef NextBit


int Sound::playiMUSE(File& _input, BlockTable *_blockTable, int index, File& _output, int save)
{
    SDL_RWops *rw;
    int32 i = 0;
    int tag, num, input_size, codec;
    uint32 size = 0, rate = 0, chan = 0, bits = 0, s_size = 0;
    byte *comp_input, *comp_output, *CompFinal, *buffer = NULL;
    int32 output_size, channels;
    int32 offset1, offset2, offset3, length, k, c, s, j, r, t, z;
    byte *src, *t_table, *p, *ptr;
    byte t_tmp1, t_tmp2;
    CompFinal = (byte *) malloc(30000000);
    int32 finalSize;
    finalSize = 0;
    int voice = 0;

    initializeImcTables();

	_input.seek(_blockTable[index].offset, SEEK_SET);
	tag = _input.readUint32BE();
	num = _input.readUint32BE();
	_input.readUint32BE();
	_input.readUint32BE();

	if (tag != MKID_BE('COMP')) {
	    printf("Bundle: Compressed sound %d invalid (%c%c%c%c)\n",
		   index, tag >> 24, tag >> 16, tag >> 8, tag);
	    return 0;
	}

	free(_compMusicTable);
	_compMusicTable = (CompTable *) malloc(sizeof(CompTable) * num);

	for (i = 0; i < num; i++) {
	    _compMusicTable[i].offset = _input.readUint32BE();
	    _compMusicTable[i].size = _input.readUint32BE();
	    _compMusicTable[i].codec = _input.readUint32BE();
	    _input.readUint32BE();
	}

    for (i = 0; i < num; i++) {
	comp_input = (byte *) malloc(_compMusicTable[i].size + 1);
	comp_input[_compMusicTable[i].size] = 0;
	comp_output = (byte *) malloc(10000);
	memset(comp_output, 0, 10000);
	input_size = _compMusicTable[i].size;
	codec = _compMusicTable[i].codec;

	_input.seek(_blockTable[index].offset + _compMusicTable[i].offset, SEEK_SET);
	_input.read(comp_input, _compMusicTable[i].size);

	switch (_compMusicTable[i].codec) {
	case 0:
	    memcpy(comp_output, comp_input, input_size);
	    output_size = input_size;
	    break;

	case 1:
	    output_size = compDecode(comp_input, comp_output);
	    break;

	case 2:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];
	    break;

	case 3:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];
	    break;

	case 4:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memset(t_table, 0, output_size);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 0;
	    if (length > 0) {
		c = -12;
		s = 0;
		j = 0;
		do {
		    ptr = src + length + (k >> 1);
		    if (k & 1) {
			r = c >> 3;
			t_table[r + 2] =
			    ((src[j] & 0x0f) << 4) | (ptr[1] >> 4);
			t_table[r + 1] =
			    (src[j] & 0xf0) | (t_table[r + 1]);
		    } else {
			r = s >> 3;
			t_table[r + 0] =
			    ((src[j] & 0x0f) << 4) | (ptr[0] & 0x0f);
			t_table[r + 1] = src[j] >> 4;
		    }
		    s += 12;
		    c += 12;
		    k++;
		    j++;
		}
		while (k < length);
	    }
	    offset1 = ((length - 1) * 3) / 2;
	    t_table[offset1 + 1] =
		(t_table[offset1 + 1]) | (src[length - 1] & 0xf0);
	    memcpy(src, t_table, output_size);
	    free(t_table);
	    break;

	case 5:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memset(t_table, 0, output_size);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 1;
	    c = 0;
	    s = 12;
	    t_table[0] = src[length] / 16;
	    t = length + k;
	    j = 1;
	    if (t > k) {
		do {
		    ptr = src + length + (k >> 1);
		    if (k & 1) {
			r = c >> 3;
			t_table[r + 0] = (src[j - 1] & 0xf0) | t_table[r];
			t_table[r + 1] =
			    ((src[j - 1] & 0x0f) << 4) | (ptr[0] & 0x0f);
		    } else {
			r = s >> 3;
			t_table[r + 0] = src[j - 1] >> 4;
			t_table[r - 1] =
			    ((src[j - 1] & 0x0f) << 4) | (ptr[0] >> 4);
		    }
		    s += 12;
		    c += 12;
		    k++;
		    j++;
		}
		while (k < t);
	    }
	    memcpy(src, t_table, output_size);
	    free(t_table);
	    break;

	case 6:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memset(t_table, 0, output_size);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 0;
	    c = 0;
	    j = 0;
	    s = -12;
	    t_table[0] = src[output_size - 1];
	    t_table[output_size - 1] = src[length - 1];
	    t = length - 1;
	    if (t > 0) {
		do {
		    ptr = src + length + (k >> 1);
		    if (k & 1) {
			r = s >> 3;
			t_table[r + 2] =
			    (src[j] & 0xf0) | *(t_table + r + 2);
			t_table[r + 3] =
			    ((src[j] & 0x0f) << 4) | (ptr[0] >> 4);
		    } else {
			r = c >> 3;
			t_table[r + 2] = src[j] >> 4;
			t_table[r + 1] =
			    ((src[j] & 0x0f) << 4) | (ptr[0] & 0x0f);
		    }
		    s += 12;
		    c += 12;
		    k++;
		    j++;
		}
		while (k < t);
	    }
	    memcpy(src, t_table, output_size);
	    free(t_table);
	    break;

	case 10:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memcpy(t_table, p, output_size);

	    offset1 = output_size / 3;
	    offset2 = offset1 * 2;
	    offset3 = offset2;
	    src = comp_output;
	    do {
		if (offset1 == 0)
		    break;
		offset1--;
		offset2 -= 2;
		offset3--;
		t_table[offset2 + 0] = src[offset1];
		t_table[offset2 + 1] = src[offset3];
	    }
	    while (1);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 0;
	    if (length > 0) {
		c = -12;
		s = 0;
		do {
		    j = length + (k >> 1);
		    if (k & 1) {
			r = c >> 3;
			t_tmp1 = t_table[k];
			t_tmp2 = t_table[j + 1];
			src[r + 2] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 >> 4);
			src[r + 1] = (src[r + 1]) | (t_tmp1 & 0xf0);
		    } else {
			r = s >> 3;
			t_tmp1 = t_table[k];
			t_tmp2 = t_table[j];
			src[r + 0] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 & 0x0f);
			src[r + 1] = t_tmp1 >> 4;
		    }
		    s += 12;
		    c += 12;
		    k++;
		}
		while (k < length);
	    }
	    offset1 = ((length - 1) * 3) / 2;
	    src[offset1 + 1] = (t_table[length] & 0xf0) | src[offset1 + 1];
	    free(t_table);
	    break;

	case 11:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memcpy(t_table, p, output_size);

	    offset1 = output_size / 3;
	    offset2 = offset1 * 2;
	    offset3 = offset2;
	    src = comp_output;
	    do {
		if (offset1 == 0)
		    break;
		offset1--;
		offset2 -= 2;
		offset3--;
		t_table[offset2 + 0] = src[offset1];
		t_table[offset2 + 1] = src[offset3];
	    }
	    while (1);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 1;
	    c = 0;
	    s = 12;
	    t_tmp1 = t_table[length] / 16;
	    src[0] = t_tmp1;
	    t = length + k;
	    if (t > k) {
		do {
		    j = length + (k / 2);
		    if (k & 1) {
			r = c >> 3;
			t_tmp1 = t_table[k - 1];
			t_tmp2 = t_table[j];
			src[r + 0] = (src[r]) | (t_tmp1 & 0xf0);
			src[r + 1] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 & 0x0f);
		    } else {
			r = s >> 3;
			t_tmp1 = t_table[k - 1];
			t_tmp2 = t_table[j];
			src[r + 0] = t_tmp1 >> 4;
			src[r - 1] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 >> 4);
		    }
		    s += 12;
		    c += 12;
		    k++;
		}
		while (k < t);
	    }
	    free(t_table);
	    break;

	case 12:
	    output_size = compDecode(comp_input, comp_output);
	    p = comp_output;
	    for (z = 2; z < output_size; z++)
		p[z] += p[z - 1];
	    for (z = 1; z < output_size; z++)
		p[z] += p[z - 1];

	    t_table = (byte *) malloc(output_size);
	    memcpy(t_table, p, output_size);

	    offset1 = output_size / 3;
	    offset2 = offset1 * 2;
	    offset3 = offset2;
	    src = comp_output;
	    do {
		if (offset1 == 0)
		    break;
		offset1--;
		offset2 -= 2;
		offset3--;
		t_table[offset2 + 0] = src[offset1];
		t_table[offset2 + 1] = src[offset3];
	    }
	    while (1);

	    src = comp_output;
	    length = (output_size * 8) / 12;
	    k = 0;
	    c = 0;
	    s = -12;
	    src[0] = t_table[output_size - 1];
	    src[output_size - 1] = t_table[length - 1];
	    t = length - 1;
	    if (t > 0) {
		do {
		    j = length + (k >> 1);
		    if (k & 1) {
			r = s >> 3;
			t_tmp1 = t_table[k];
			t_tmp2 = t_table[j];
			src[r + 2] = (src[r + 2]) | (t_tmp1 & 0xf0);
			src[r + 3] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 >> 4);
		    } else {
			r = c >> 3;
			t_tmp1 = t_table[k];
			t_tmp2 = t_table[j];
			src[r + 2] = t_tmp1 >> 4;
			src[r + 1] =
			    ((t_tmp1 & 0x0f) << 4) | (t_tmp2 & 0x0f);
		    }
		    s += 12;
		    c += 12;
		    k++;
		}
		while (k < t);
	    }
	    free(t_table);
	    break;

	case 13:
	case 15:
	    if (codec == 13) {
		channels = 1;
	    } else {
		channels = 2;
	    }

	    {
		const int MAX_CHANNELS = 2;
		int32 left,
		    startPos, origLeft, curTableEntry, destPos, esiReg;
		int16 firstWord;
		byte sByte[MAX_CHANNELS] = { 0, 0 };
		int32 sDWord1[MAX_CHANNELS] = { 0, 0 };
		int32 sDWord2[MAX_CHANNELS] = { 0, 0 };
		int32 tableEntrySum,
		    imcTableEntry, curTablePos, outputWord, adder;
		byte decompTable, otherTablePos, bitMask;
		byte *readPos, *dst;
		uint16 readWord;

		assert(0 <= channels && channels <= MAX_CHANNELS);

		src = comp_input;
		dst = comp_output;
		if (channels == 2) {
		    output_size = left = 0x2000;
		} else {
		    left = 0x1000;
		    output_size = 0x2000;
		}
		firstWord = READ_BE_UINT16(src);
		src += 2;
		if (firstWord != 0) {
		    memcpy(dst, src, firstWord);
		    dst += firstWord;
		    src += firstWord;
		    startPos = 0;
		    if (channels == 2) {
			left = 0x2000 - firstWord;
			output_size = left;
		    } else {
			left = 0x1000 - (firstWord >> 1);
			output_size = left << 1;
		    }
		} else {
		    startPos = 1;
		    for (int i = 0; i < channels; i++) {
			sByte[i] = *(src++);
			sDWord1[i] = READ_BE_UINT32(src);
			src += 4;
			sDWord2[i] = READ_BE_UINT32(src);
			src += 4;
		    }
		}

		origLeft = left >> (channels - 1);
		tableEntrySum = 0;
		for (int l = 0; l < channels; l++) {
		    if (startPos != 0) {
			curTablePos = sByte[l];
			imcTableEntry = sDWord1[l];
			outputWord = sDWord2[l];
		    } else {
			curTablePos = 0;
			imcTableEntry = 7;
			outputWord = 0;
		    }

		    left = origLeft;
		    destPos = l * 2;

		    if (channels == 2) {
			if (l == 0)
			    left++;
			left >>= 1;
		    }

		    while (left--) {
			curTableEntry = _destImcTable[curTablePos];
			decompTable = (byte) (curTableEntry - 2);
			bitMask = 2 << decompTable;
			readPos = src + (tableEntrySum >> 3);

			if (readPos + 1 >= comp_input + input_size) {
			    if (readPos + 1 > comp_input + input_size ||
				curTableEntry + (tableEntrySum & 7) > 8) {
				printf
				    ("decompressCodec: input buffer overflow: %d bytes over (we need %d bits of data)\n",
				     (int) ((readPos + 1) -
					    (comp_input + input_size)) + 1,
				     curTableEntry + (tableEntrySum & 7));
			    }
			}
			readWord =
			    (uint16) (READ_BE_UINT16(readPos) <<
				      (tableEntrySum & 7));
			otherTablePos =
			    (byte) (readWord >> (16 - curTableEntry));
			tableEntrySum += curTableEntry;
			esiReg =
			    ((imxShortTable[curTableEntry] & otherTablePos)
			     << (7 - curTableEntry)) + (curTablePos << 6);
			imcTableEntry >>= (curTableEntry - 1);
			adder = imcTableEntry + _destImcTable2[esiReg];
			if ((otherTablePos & bitMask) != 0) {
			    adder = -adder;
			}
			outputWord += adder;

			if (outputWord > 0x7fff)
			    outputWord = 0x7fff;
			if (outputWord < -0x8000)
			    outputWord = -0x8000;
			dst[destPos] = ((int16) outputWord) >> 8;
			dst[destPos + 1] = (byte) (outputWord);

			assert(decompTable < 6);
			curTablePos += (signed char)
			    imxOtherTable[decompTable][otherTablePos];
			if (curTablePos > 88)
			    curTablePos = 88;
			if (curTablePos < 0)
			    curTablePos = 0;
			imcTableEntry = imcTable[curTablePos];

			destPos += channels << 1;
		    }
		}
	    }
	    break;

	default:
	    printf("Bundle: Unknown codec %d!\n", (int) codec);
	    output_size = 0;
	    break;
	}

	memcpy(&CompFinal[finalSize], comp_output, output_size);
	finalSize += output_size;

	free(comp_input);
	comp_input = NULL;
	free(comp_output);
	comp_output = NULL;

    }

    ptr = CompFinal;
    tag = READ_BE_UINT32(ptr);
    ptr += 4;
    if (tag != MKID_BE('iMUS')) {
	printf("Decompression of bundle sound failed\n");
	free(CompFinal);
	return 0;
    }

    ptr += 12;			/* Skip header */
    while (tag != MKID_BE('DATA')) {
	tag = READ_BE_UINT32(ptr);
	ptr += 4;
	switch (tag) {
	case MKID_BE('FRMT'):
	    size = READ_BE_UINT32(ptr);
	    ptr += 12;
	    bits = READ_BE_UINT32(ptr);
	    ptr += 4;
	    rate = READ_BE_UINT32(ptr);
	    ptr += 4;
	    chan = READ_BE_UINT32(ptr);
	    ptr += 4;
	    break;
	case MKID_BE('TEXT'):
	case MKID_BE('REGN'):
	case MKID_BE('STOP'):
	case MKID_BE('JUMP'):
	case MKID_BE('SYNC'):
	    size = READ_BE_UINT32(ptr);
	    ptr += size + 4;
	    break;

	case MKID_BE('DATA'):
	    size = READ_BE_UINT32(ptr);
	    ptr += 4;
	    break;

	default:
	    printf("Unknown bundle header %c%c%c%c\n", tag >> 24,
		   tag >> 16, tag >> 8, tag);
	}
    }

    if (bits == 12) {
	s_size = (size * 4) / 3 + 3;
	buffer = (byte *) malloc(s_size+44);
	uint32 l = 0, r = 44, tmp;
	for (; l < size; l += 3) {
	    tmp = (ptr[l + 1] & 0x0f) << 8;
	    tmp = (tmp | ptr[l + 0]) << 4;
	    tmp -= 0x8000;
	    buffer[r++] = (uint8) (tmp & 0xff);
	    buffer[r++] = (uint8) ((tmp >> 8) & 0xff);

	    tmp = (ptr[l + 1] & 0xf0) << 4;
	    tmp = (tmp | ptr[l + 2]) << 4;
	    tmp -= 0x8000;
	    buffer[r++] = (uint8) (tmp & 0xff);
	    buffer[r++] = (uint8) ((tmp >> 8) & 0xff);
	}
	bits = 16;
    } else {
	size &= ~1;
	printf("bits: %d channels: %d rate: %d\n", bits, chan, rate);
	voice = 1;
	s_size = size;
	buffer = (byte *) malloc(s_size+44);
    }

	byte wav[44];
	memset (wav, 0, 44);
	wav[0] = 'R';
	wav[1] = 'I';
	wav[2] = 'F';
	wav[3] = 'F';
	wav[4] = (s_size + 36) & 0xff;
	wav[5] = ((s_size + 36) >> 8) & 0xff;
	wav[6] = ((s_size + 36) >> 16) & 0xff;
	wav[7] = ((s_size + 36) >> 24) & 0xff;
	wav[8] = 'W';
	wav[9] = 'A';
	wav[10] = 'V';
	wav[11] = 'E';
	wav[12] = 'f';
	wav[13] = 'm';
	wav[14] = 't';
	wav[15] = ' ';
	wav[16] = 16;
	wav[20] = 1;
	wav[22] = chan;
	wav[24] = rate & 0xff;
	wav[25] = (rate >> 8) & 0xff;
	wav[26] = (rate >> 16) & 0xff;
	wav[27] = (rate >> 24) & 0xff;
	wav[28] = (rate * chan * (bits / 8)) & 0xff;
	wav[29] = ((rate * chan * (bits / 8))>> 8) & 0xff;
	wav[30] = ((rate * chan * (bits / 8)) >> 16) & 0xff;
	wav[31] = ((rate * chan * (bits / 8)) >> 24) & 0xff;
	wav[32] = (chan * (bits / 8)) & 0xff;
	wav[33] = ((chan * (bits / 8)) >> 8) & 0xff;
	wav[34] = bits;
	wav[36] = 'd';
	wav[37] = 'a';
	wav[38] = 't';
	wav[39] = 'a';
	wav[40] = s_size & 0xff;
	wav[41] = (s_size >> 8) & 0xff;
	wav[42] = (s_size >> 16) & 0xff;
	wav[43] = (s_size >> 24) & 0xff;

	memcpy(buffer, wav, 44);

	if (voice)
		memcpy(&buffer[44], ptr, s_size);
	
	free(CompFinal);
	CompFinal = NULL;

	if (save == 1) {
		_output.write(buffer, s_size+44);
		_output.close();
	} else {
		rw = SDL_RWFromMem(buffer, s_size+44);
		_mixer->playSound(rw, 1);
	}
	free(buffer);

    return 0;
}
