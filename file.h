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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/file.h,v 1.1 2003/09/18 16:26:53 fingolfin Exp $
 *
 */

#ifndef COMMON_FILE_H
#define COMMON_FILE_H

#include "stdafx.h"
#include "scummsys.h"

class File {
private:

	FILE * _handle;
	bool _ioFailed;
	byte _encbyte;
	char *_name;	// For debugging

	FILE *fopenNoCase(const char *filename, const char *directory, const char *mode);

public:
	enum {
		kFileReadMode = 1,
		kFileWriteMode = 2
	};

	File();
	virtual ~File();
	bool open(const char *filename, int mode = kFileReadMode, byte encbyte = 0);
	void close();
	void setencbyte (byte encbyte);
	bool isOpen();
	bool ioFailed();
	uint32 pos();
	uint32 size();
	const char *name() const { return _name; }
	void seek(int32 offs, int whence = SEEK_SET);
	uint32 read(void *ptr, uint32 size);
	byte readByte();
	uint16 readUint16LE();
	uint32 readUint32LE();
	uint16 readUint16BE();
	uint32 readUint32BE();
	uint32 write(const void *ptr, uint32 size);
	void writeByte(byte value);
	void writeUint16LE(uint16 value);
	void writeUint32LE(uint32 value);
	void writeUint16BE(uint16 value);
	void writeUint32BE(uint32 value);
	byte File::getbit(byte s);
};

#endif
