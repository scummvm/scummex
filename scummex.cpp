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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/scummex.cpp,v 1.23 2003/09/24 11:49:30 yoshizf Exp $
 *
 */

#include <stdio.h>
#include "file.h"
#include "scummex.h"
#include "descumm.h"

ScummEX::ScummEX() {
	_image = new Image();
	_sound = new Sound();
	_resource = new Resource();
}

ScummEX::~ScummEX() {
	delete _resource;
	delete _sound;
	delete _image;
}

void ScummEX::loadFile(const char *filename) {
	uint32 tag;
	_encbyte = 0;
	char buf[256];
	_scummVersion = 0;
	
	if (_input.isOpen()) {
		_input.close();
	}
		
	_input.open(filename, 1, _encbyte);

	sprintf(buf, "ScummEX - %s", filename);
	_gui->SetTitle(buf);

	_gui->EnableToolbarTool(wxID_CLOSE);
	_gui->EnableToolbarTool(ID_FileInfo);

	_input.read(&tag, 4);

	switch (tag) {
		case MKID('LB83'):
		case MKID('LABN'):
		case MKID('RNAM'):
		case MKID('LECF'):
		case MKID('ANIM'):
		case MKID('SOU '):
			_input.seek(0, SEEK_SET);
			_resource->searchBlocks(_blockTable, _input);
			return;

	}

	tag = 0;
	_input.read(&tag, 2);

	switch(tag) {
		case 21040: // OR
		case 20306: // RO
		case 20050: // RN
			_input.seek(0, SEEK_SET);
			_resource->searchOldBlocks(_blockTable, _input);
			return;

	}
	
	_input.close();
	_encbyte = 0x69;
	_input.open(filename, 1, _encbyte);
	_input.read(&tag, 4);

	switch (tag) {
		case MKID('RNAM'):
			_input.seek(0, SEEK_SET);
			_resource->searchBlocks(_blockTable, _input);
			return;

		case MKID('LECF'):
			_input.seek(0, SEEK_SET);
			_resource->searchBlocks(_blockTable, _input);
			return;
	}

	tag = 0;
	_input.read(&tag, 2);

	switch(tag) {
		case 17740: // LE
			_input.seek(0, SEEK_SET);
			_resource->searchOldBlocks(_blockTable, _input);
			return;
	}

	_gui->SetTitle("ScummEX");
	_gui->DisableToolbarTool(wxID_CLOSE);
	_gui->DisableToolbarTool(ID_FileInfo);
	_gui->DisplayDialog("Unknown file type!", "Error");
}

void ScummEX::Descumm(int blockid) {
	
	_input.seek(_blockTable[blockid].offset, SEEK_SET);
	if (_scummVersion == 0)
		_scummVersion = _gui->getScummVersionDialog();
	if (_scummVersion != 0) {
		DeScumm(_input, _blockTable[blockid].blockSize, _scummVersion);
	}
}

void ScummEX::FileDump(int blockid, const char *filename) {

	uint32 size = 0, tot_size = 0;
	char fbuf2[2048];

	_output.open(filename, 2, 0);
	_input.seek(_blockTable[blockid].offset, SEEK_SET);

	while (tot_size < _blockTable[blockid].blockSize) {
		size = _input.read(fbuf2, 1);
		tot_size += size;
		_output.write(fbuf2, 1);
	}
	_output.close();
}

void ScummEX::fileView(int blockid) {
	int j, i;
	char *text;
	char title[256];
	char buf[256];
	byte* data;
	byte* dataorg;
	int len, offset, nlines, bytes_per_line;
	byte c;
	
	len = _blockTable[blockid].blockSize;
	offset = _blockTable[blockid].offset;
	bytes_per_line = 16;
	nlines = len / bytes_per_line;
	
	text = (char *) calloc(((16 + (3*bytes_per_line) + (bytes_per_line/4) + bytes_per_line) * nlines) + 256, 1);

	if (text == NULL)
		return;

	sprintf(title, "%s Block at offset %d", _blockTable[blockid].blockName, _blockTable[blockid].offset);
	
	_input.seek(_blockTable[blockid].offset, SEEK_SET);

	dataorg = data = (byte*) malloc(_blockTable[blockid].blockSize);

	if (data == NULL)
		return;

	_input.read(data, _blockTable[blockid].blockSize);

	while (len >= bytes_per_line) {
		sprintf(buf, "%06X: ", offset);
		strcat(text, buf);
		for (i = 0; i < bytes_per_line; i++) {
			sprintf(buf, "%02X ", data[i]);
			strcat(text, buf);
			if (i % 4 == 3) {
				sprintf(buf, " ");
				strcat(text, buf);
			}
		}
		sprintf(buf, " |");
		strcat(text, buf);
		for (i = 0; i < bytes_per_line; i++) {
			c = data[i];
			if (c < 32 || c >= 127)
				c = '.';
			sprintf(buf, "%c", c);
			strcat(text, buf);
		}
		sprintf(buf, "|\n");
		strcat(text, buf);
		data += bytes_per_line;
		len -= bytes_per_line;
		offset += bytes_per_line;
	}

	if (len <= 0) 
		return;

	sprintf(buf, "%06X: ", offset);
	strcat(text, buf);
	for (i = 0; i < len; i++) {
		sprintf(buf, "%02X ", data[i]);
		strcat(text, buf);
		if (i % 4 == 3) {
			sprintf(buf, " ");
			strcat(text, buf);
		}
	}

	for (; i < bytes_per_line; i++) {
		sprintf(buf, "   ");
		strcat(text, buf);
			if (i % 4 == 3) {
				sprintf(buf, " ");
				strcat(text, buf);
			}
	}
	sprintf(buf, " |");
	strcat(text, buf);
	for (i = 0; i < len; i++) {
		c = data[i];
		if (c < 32 || c >= 127)
			c = '.';
		sprintf(buf, "%c", c);
		strcat(text, buf);
	}
	for (; i < bytes_per_line; i++) {
		sprintf(buf, " ");
		strcat(text, buf);
	}
	sprintf(buf, "|\n");
	strcat(text, buf);

	free(dataorg);
	
	_gui->DisplayViewer(title, 660, 480, text);

	free(text);

}

void ScummEX::iMUSEPlay(int blockid)
{
	_sound->playiMUSE(_input, _blockTable, blockid, _input);
}

void ScummEX::iMUSEDump(int blockid, const char *filename)
{
	_output.open(filename, 2, 0);

	_sound->playiMUSE(_input, _blockTable, blockid, _output, 1);
}

void ScummEX::SOUPlay(int blockid)
{
	_sound->playSOU(_blockTable, _input, blockid, _input);
}

void ScummEX::paletteDraw(int blockid)
{
	_image->drawPalette(_blockTable, blockid, _input);
}

void ScummEX::bgDraw(int blockid)
{
	_image->drawBG(_input, _blockTable, blockid);
}

void ScummEX::bgReDraw(int imageWindowId, int blockid)
{
	_image->drawBG(_input, _blockTable, blockid, 0, imageWindowId);
}

void ScummEX::SmushFrameDraw(int blockid)
{
	_image->drawSmushFrame(_blockTable, blockid, _input);
}

void ScummEX::objectDraw(int blockid)
{
	_image->drawObject(_input, _blockTable, blockid);
}

void ScummEX::boxesDraw(int blockid)
{
	_image->drawBoxes(_blockTable, blockid, _input);
}

void ScummEX::boxesDrawOverlay(int imageWindowId, int blockid)
{
	_image->drawBoxes(_blockTable, blockid, _input, 0, imageWindowId);
}
