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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/file.cpp,v 1.4 2003/10/01 17:28:48 yoshizf Exp $
 *
 */

#include "file.h"

File::File() {
	_handle = NULL;
	_ioFailed = false;
	_encbyte = 0;
	_name = 0;
}

File::~File() {
	delete [] _name;
	close();
}

bool File::open(const char *filename,  int mode, byte encbyte) {
	if (_handle) {
		debug(2, "File %s already opened", filename);
		return false;
	}

	if (filename == NULL || *filename == 0)
		return false;

	if (mode == kFileReadMode) {
		_handle = fopen(filename, "rb");
		if (_handle == NULL) {
			debug(2, "File %s not found", filename);
			return false;
		}
	}
	else if (mode == kFileWriteMode) {
		_handle = fopen(filename, "wb");
		if (_handle == NULL) {
			debug(2, "File %s not opened", filename);
			return false;
		}
	}	else {
		warning("Only read/write mode supported!");
		return false;
	}

	_encbyte = encbyte;

	int len = strlen(filename);
	if (_name != 0)
		delete [] _name;
	_name = new char[len+1];
	memcpy(_name, filename, len+1);

	return true;
}

void File::close() {
	if (_handle) {
		fclose(_handle);
	}
	_handle = NULL;
}

void File::setencbyte (byte encbyte) {
	_encbyte = encbyte;
}

bool File::isOpen() {
	return _handle != NULL;
}

uint32 File::pos() {
	if (_handle == NULL) {
		error("File is not open!");
		return 0;
	}

	return ftell(_handle);
}

uint32 File::size() {
	if (_handle == NULL) {
		error("File is not open!");
		return 0;
	}

	uint32 oldPos = ftell(_handle);
	fseek(_handle, 0, SEEK_END);
	uint32 length = ftell(_handle);
	fseek(_handle, oldPos, SEEK_SET);

	return length;
}

void File::seek(int32 offs, int whence) {
	if (_handle == NULL) {
		error("File is not open!");
		return;
	}

	if (fseek(_handle, offs, whence) != 0)
		clearerr(_handle);
}

uint32 File::read(void *ptr, uint32 len) {
	byte *ptr2 = (byte *)ptr;
	uint32 real_len;

	if (_handle == NULL) {
		error("File is not open!");
		return 0;
	}

	if (len == 0)
		return 0;

	real_len = fread(ptr2, 1, len, _handle);
	if (real_len < len) {
		clearerr(_handle);
		_ioFailed = true;
	}

	if (_encbyte != 0) {
		uint32 t_size = real_len;
		do {
			*ptr2++ ^= _encbyte;
		} while (--t_size);
	}

	return real_len;
}

byte File::readByte() {
	byte b;

	if (_handle == NULL) {
		error("File is not open!");
		return 0;
	}

	if (fread(&b, 1, 1, _handle) != 1) {
		clearerr(_handle);
		_ioFailed = true;
	}
	return b ^ _encbyte;
}

uint16 File::readUint16LE() {
	uint16 a = readByte();
	uint16 b = readByte();
	return a | (b << 8);
}

uint32 File::readUint32LE() {
	uint32 a = readUint16LE();
	uint32 b = readUint16LE();
	return (b << 16) | a;
}

uint16 File::readUint16BE() {
	uint16 b = readByte();
	uint16 a = readByte();
	return a | (b << 8);
}

uint32 File::readUint32BE() {
	uint32 b = readUint16BE();
	uint32 a = readUint16BE();
	return (b << 16) | a;
}

uint32 File::write(const void *ptr, uint32 len) {
	byte *tmp = 0;
	
	if (_handle == NULL) {
		error("File is not open!");
		return 0;
	}

	if (len == 0)
		return 0;

	if (_encbyte != 0) {
		// Maybe FIXME: while it's efficient to do the encoding here,
		// it not really nice for a write function to modify its input.
		// Maybe we should work on a copy here...
		tmp = (byte *)malloc(len);
		for (uint32 i = 0; i < len; i ++) {
			tmp[i] = ((const byte *)ptr)[i] ^ _encbyte;
		}
		ptr = tmp;
	}

	if ((uint32)fwrite(ptr, 1, len, _handle) != len) {
		clearerr(_handle);
		_ioFailed = true;
	}

	if (_encbyte != 0) {
		free(tmp);
	}

	return len;
}

void File::writeByte(byte value) {
	value ^= _encbyte;

	if (_handle == NULL) {
		error("File is not open!");
	}

	if (fwrite(&value, 1, 1, _handle) != 1) {
		clearerr(_handle);
		_ioFailed = true;
	}
}

void File::writeUint16LE(uint16 value) {
	writeByte((byte)(value & 0xff));
	writeByte((byte)(value >> 8));
}

void File::writeUint32LE(uint32 value) {
	writeUint16LE((uint16)(value & 0xffff));
	writeUint16LE((uint16)(value >> 16));
}

void File::writeUint16BE(uint16 value) {
	writeByte((byte)(value >> 8));
	writeByte((byte)(value & 0xff));
}

void File::writeUint32BE(uint32 value) {
	writeUint16BE((uint16)(value >> 16));
	writeUint16BE((uint16)(value & 0xffff));
}
