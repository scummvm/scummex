#include <stdio.h>
#include "file.h"
#include "resource.h"
#include "scummex.h"

int v8 = 0;

/*void parseV5Index(BlockTable *_blockTable, RoomTable *_roomTable, File& _input, int fileType) {
	int i = 0, index = 0, rooms = 0;

	// MAXS block
	if (rooms < 1)
		fileType = 2;

	if (fileType == 1) {
		_input.read(_blockTable[index].blockName, 4);
		_blockTable[index].blockName[4] = '\0';
		strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
		_blockTable[index].blockTypeID = getBlockType(_blockTable[index].blockName);
		strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);

		_blockTable[index].blockSize = _input.readUint32BE();
		_blockTable[index].offset = _input.pos() - 8;
		_blockTable[index].variables = _input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].bitVariables = _input.readUint16LE();
		_blockTable[index].localObjects = _input.readUint16LE();
		_blockTable[index].arrays = _input.readUint16LE();
		_blockTable[index].characters = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].invObjects = _input.readUint16LE();
		index++;
	}

	if (fileType == 2) {
		_input.read(_blockTable[index].blockName, 4);
		_blockTable[index].blockName[4] = '\0';
		strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
		_blockTable[index].blockTypeID = getBlockType(_blockTable[index].blockName);
		strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);
		_blockTable[index].blockSize = _input.readUint32BE();
		_blockTable[index].offset = _input.pos() - 8;
		_blockTable[index].variables = _input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].bitVariables = _input.readUint16LE();
		_blockTable[index].localObjects = _input.readUint16LE();
		_blockTable[index].arrays = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].invObjects = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].characters = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		index++;
	}

	if (fileType == 3) {
		_input.read(_blockTable[index].blockName, 4);
		_blockTable[index].blockName[4] = '\0';
		strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
		_blockTable[index].blockTypeID = getBlockType(_blockTable[index].blockName);
		strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);
		_blockTable[index].blockSize = _input.readUint32BE();
		_blockTable[index].offset = _input.pos() - 8;
		_input.seek(50 + 50, SEEK_CUR);
		_blockTable[index].variables = _input.readUint16LE();
		_blockTable[index].bitVariables = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].localObjects = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].invObjects = _input.readUint16LE();
		_blockTable[index].arrays = _input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_input.readUint16LE();
		_blockTable[index].characters = _input.readUint16LE();
		_input.readUint16LE();
		index++;
	}

}
*/

Resource::Resource() {
	stopflag = 0;
}

int Resource::searchOldBlocks(BlockTable *_blockTable, File& _input, int index, int level, int size) {
	int32 curpos = 0;
	if (size == 0)
		size = _input.size();

	while (curpos < size) {
			curpos = _input.pos();
			if (curpos >= size)
				break;

			_blockTable[index].blockSize = _input.readUint32LE();
			_input.read(_blockTable[index].blockName, 2);
			_blockTable[index].blockName[2] = '\0';
			strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
			_blockTable[index].offset = _input.pos() - 6;
			_blockTable[index].blockTypeID = getOldBlockType(_blockTable[index].blockName);
			if (_blockTable[index].blockTypeID != -1) {
				index = parseOldBlocks(_blockTable[index].blockName, _blockTable, _input, index, level);
				curpos = _input.pos();
				if (index == 0)
					break;

				if (stopflag == 1) {
					stopflag = 0;
					_input.seek(size, SEEK_SET);
					return index;
				}


			}
			curpos = _input.pos();
	}

	return index;
}


int Resource::searchBlocks(BlockTable *_blockTable, File& _input, int index, int level, int size) {
	int32 curpos = 0;
	int called = 0;
	if (size != 0)
		called = 1;
		
	if (size == 0)
		size = _input.size();

	while (curpos < size) {
			curpos = _input.pos();
			if (curpos >= size)
				break;

			_input.read(_blockTable[index].blockName, 4);
			_blockTable[index].blockName[4] = '\0';
			strcpy(_blockTable[index].blockType, _blockTable[index].blockName);
			_blockTable[index].offset = _input.pos() - 4;
			//printf("block: %s offset: %d\n", _blockTable[index].blockName, _blockTable[index].offset);
			_blockTable[index].blockTypeID = getBlockType(_blockTable[index].blockName);
			if (_blockTable[index].blockTypeID != -1) {
				index = parseBlocks(_blockTable[index].blockName, _blockTable, _input, index, level);
				curpos = _input.pos();
				if (index == 0)
					break;

				if (stopflag == 1) {
					stopflag = 0;
					_input.seek(size, SEEK_SET);
					return index;
				}


			} else if (called) {
					_input.seek(size, SEEK_SET);
					return index;
			}

			curpos = _input.pos();
	}

	return index;
}

int Resource::parseBlocks(char *blockName, BlockTable *_blockTable, File& _input, int index, int level) {
	
	int z = 0;
	byte c;
	char name[256];
	int numFiles, offset, number;
	int bufindex;
	uint blockOffset, blockSize;

	strcpy(_blockTable[index].blockDescription, blocksInfo[_blockTable[index].blockTypeID].description);

	switch(_blockTable[index].blockTypeID) {
	
		case RNAM:
			_blockTable[index].blockSize = _input.readUint32BE();
			while (1) {
				number = _input.readByte();
				if (number == 0) 
					break;

				_input.seek(9, SEEK_CUR);
			}
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case LECF:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;

		case LOFF:
			_blockTable[index].blockSize = _input.readUint32BE();
			numFiles = _input.readByte();
			_blockTable[index].numFiles = numFiles;
			// Getting LFLFs (rooms) offsets
			for (int i=0; i<numFiles; i++) {
				number = _input.readByte();
				offset = _input.readUint32LE();
			}
	
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case LFLF:
			blockSize = _input.readUint32BE();
			_blockTable[index].blockSize = blockSize;
			blockOffset = _blockTable[index].offset;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, blockSize + blockOffset);
			_input.seek(blockSize + blockOffset, SEEK_SET);
			break;

		case ROOM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;
			
		case RMHD:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].width = _input.readUint32LE();

			switch(_blockTable[index].width) {
				case 730:
					_blockTable[index].width = _input.readUint16LE();
					_blockTable[index].height = _input.readUint16LE();
					_blockTable[index].localObjects = _input.readUint16LE();					
					break;

				case 800:
					_blockTable[index].width = _input.readUint32LE();
					_blockTable[index].height = _input.readUint32LE();
					_blockTable[index].localObjects = _input.readUint32LE();
					break;
					
				default:
					_input.seek(-4, SEEK_CUR);
					_blockTable[index].width = _input.readUint16LE();
					_blockTable[index].height = _input.readUint16LE();
					_blockTable[index].localObjects = _input.readUint16LE();
					break;

			}
			
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case CYCL:
		case EPAL:
		case BOXD:
		case BOXM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case OBCD:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;

		case OBNA:
			_blockTable[index].blockSize = _input.readUint32BE();
			while (1) {
				c = _input.readByte();
				name[z++] = c;
				if (c == 0)
					break;
			}
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case CDHD:
		case VERB:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;			

		case DROO:
		case DSCR:
		case DSOU:
		case DCOS:
		case DCHR:
		case DOBJ:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].numFiles = _input.readUint16LE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case COMP:
			numFiles = _input.readUint32BE();
			_blockTable[index].numFiles = numFiles;
			_input.readUint32BE();
			_input.readUint32BE();

			for (int j=0; j<numFiles; j++) {
				_input.readUint32BE(); // dwOffset
				_input.readUint32BE(); // dwSize
				_input.readUint32BE(); // dwCodec
				_input.readUint32BE(); // dwUnknown
			}
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			stopflag = 1;
			break;
			
		case CLUT:
		case APAL:
		case NPAL:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(256*3, SEEK_CUR);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case PALS:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;
			
		case WRAP:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;
		
		case OFFS:
			_blockTable[index].blockSize = _input.readUint32BE();
			for (uint32 j=0; j < ((_blockTable[index].blockSize-8) / 4); j++) {
				offset = _input.readUint32LE();
			}
			//_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case RMIM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case RMIH:
			_blockTable[index].blockSize = _input.readUint32BE();
			int numZBuffer;
			numZBuffer = _input.readUint16LE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case 34: // IM00
		case 60:
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case SMAP:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			if (v8) {
				level++;
				index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			}
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case SCAL:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case AARY:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].numFiles = _input.readUint16LE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
			
		case ANAM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
			
		case SOUN:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case EXCD:
		case ENCD:
		case NLSC:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case SCRP:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case OBIM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case IMHD:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].width = _input.readUint32LE();

			if (_blockTable[index].width == 730) {
				_input.readUint16LE();
				_blockTable[index].numFiles = _input.readUint16LE(); // Number of images
				_input.readUint16LE(); // X Position
				_input.readUint16LE(); // Y Position
				_blockTable[index].width = _input.readUint16LE();
				_blockTable[index].height = _input.readUint16LE();
			} else {
				_input.seek(_blockTable[index].offset + 48, SEEK_SET);
				_blockTable[index].width = _input.readUint32LE();
				if (_blockTable[index].width == 801) {
					_blockTable[index].numFiles = _input.readUint32LE(); // Number of images
					_input.readUint32LE(); // X Position
					_input.readUint32LE(); // Y Position
					_blockTable[index].width = _input.readUint32LE();
					_blockTable[index].height = _input.readUint32LE();
				} else {
					_input.seek(_blockTable[index].offset + 10, SEEK_SET);
					_blockTable[index].numFiles = _input.readUint16LE();
					_input.readUint32BE(); // Number of Z-Buffers per Image
					_input.readUint16LE(); // X Position
					_input.readUint16LE(); // Y Position
					_blockTable[index].width = _input.readUint16LE();
					_blockTable[index].height = _input.readUint16LE();
				}
			}

			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			index++;
			break;
	
		case MCMP:
			numFiles = _input.readUint16BE();
			_blockTable[index].numFiles = numFiles;
			
			for (int j=0; j<numFiles; j++) {
				_input.readByte(); // nCodec
				_input.readUint32BE(); // dwDecompSize
				_input.readUint32BE(); // dwCompSize
			}
			number = _input.readUint16BE(); // nCodecs
			number /= 5;

			_input.seek(5*number, SEEK_CUR);
			_blockTable[index].blockSize = _input.pos() - _blockTable[index].offset;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case iMUS:
			number = _input.readUint32BE(); // dwDecompSize
			_blockTable[index].blockSize = 9;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;

		case MAP:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;

		case FRMT:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.readUint32BE(); // dwPosition
			_input.readUint32BE(); // dwUnknown
			_input.readUint32BE(); // dwBitsPerSample
			number = _input.readUint32BE(); // dwSampleRate
			_input.readUint32BE(); // nChannels
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
			
		case TEXT:
			_blockTable[index].blockSize = _input.readUint32BE() + 8; // Name and size not included
			_input.readUint32BE(); // Unknown
			/*while (number < _blockTable[index].blockSize + _blockTable[index].offset) {
				c = _input.readByte();
				name[z++] = c;
				number = _input.pos();
				if (c == 0)
					break;
			}
			printf("%s\n", name);*/
			_input.seek(_blockTable[index].blockSize + _blockTable[index].offset, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
	
		case REGN:
			_blockTable[index].blockSize = _input.readUint32BE() + 8; // Name and size not included
			_input.seek(_blockTable[index].blockSize + _blockTable[index].offset, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case JUMP:
			_blockTable[index].blockSize = _input.readUint32BE() + 8; // Name and size not included
			_input.readUint32BE(); // Hook Position
			_input.readUint32BE(); // Jump Destination
			_input.readUint32BE(); // Hook ID
			_input.readUint32BE(); // Unknown
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case DATA:
			_input.readUint32BE(); // Decompressed Size
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			stopflag = 1;
			break;

		case STOP:
			_blockTable[index].blockSize = _input.readUint32BE() + 8; // Name and size not included
			_input.readUint32BE(); // Hook Position
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case SYNC:
			_blockTable[index].blockSize = _input.readUint32BE() + 8; // Name and size not included
			_input.readUint32BE(); // Hook Position
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
		
		case TRNS:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].trans = _input.readUint16LE(); 
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			_input.seek(_blockTable[index].blockSize + _blockTable[index].offset, SEEK_SET);
			index++;
			break;
			
		case MAXS:
			_blockTable[index].blockSize = _input.readUint32BE();
			_blockTable[index].variables = _input.readUint16LE();
			_input.readUint16LE();
			_blockTable[index].bitVariables = _input.readUint16LE();
			_blockTable[index].localObjects = _input.readUint16LE();
			_blockTable[index].arrays = _input.readUint16LE();
			_blockTable[index].characters = _input.readUint16LE();
			_input.readUint16LE();
			_input.readUint16LE();
			_blockTable[index].invObjects = _input.readUint16LE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case LB83:
			offset = _input.readUint32BE();
			numFiles = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			_input.seek(offset, SEEK_SET);
			index++;
			level++;

			// Files blocks
			for (int i=index; i<numFiles+index; i++) {
				char name[13];
				char *tempname;
				byte c;
				int z = 0;
				int z2;
				for (z2 = 0; z2 < 8; z2++)
				    if ((c = _input.readByte()) != 0)
					name[z++] = c;
				name[z++] = '.';
				for (z2 = 0; z2 < 4; z2++)
				    if ((c = _input.readByte()) != 0)
					name[z++] = c;
				name[z] = '\0';
				strcpy(_blockTable[i].blockName, name);
		
				_blockTable[i].offset = _input.readUint32BE();
				_blockTable[i].blockSize = _input.readUint32BE();
				_blockTable[i].blockTypeID = 25;
				sprintf(_blockTable[i].blockDescription, "iMUSE Cue");
		
				tempname = strtok(name, ".");
				if (tempname != NULL) {
					tempname = strtok(NULL, ".");
						if (tempname != NULL) {
							sprintf(_blockTable[i].blockType, ".%s", tempname);
						}
				} else {
					sprintf(_blockTable[i].blockType, "Unknown");
				}

			}

			index += numFiles;
			level++;
			
			for (int i=1; i<numFiles+1; i++) {
				_gui->add_tree_elements(_blockTable[i].blockName, i, level-1, _blockTable[i].blockTypeID);
				_input.seek(_blockTable[i].offset, SEEK_SET);
				index = searchBlocks(_blockTable, _input, index, level, _blockTable[i].blockSize + _blockTable[i].offset);
				_input.seek(_blockTable[i].blockSize + _blockTable[i].offset, SEEK_SET);
			}
			
			break;
			
			
		case LABN:
			_input.readUint32LE();
			numFiles = _input.readUint32LE();
			_blockTable[index].numFiles = numFiles;
			_input.readUint32LE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;

			// Files blocks
			for (int i=index; i<numFiles+index; i++) {
				_input.readUint32LE();
				_blockTable[i].blockTypeID = 24;
				sprintf(_blockTable[i].blockDescription, " ");
				_blockTable[i].offset = _input.readUint32LE();
				_blockTable[i].blockSize = _input.readUint32LE();
				_input.readUint32LE();
			}
			
			for (int i=index; i<numFiles+index; i++) {
				char name[29];
				char *tempname;
				byte c;
				int z = 0;

				while(1) {
					c = _input.readByte();
					name[z++] = c;
					if (c == 0) {
						break;
					}
				}
				strcpy(_blockTable[i].blockName, name);
				tempname = strtok(name, ".");
				while (tempname != NULL) {
					tempname = strtok(NULL, ".");
					if (tempname != NULL) {
						sprintf(_blockTable[i].blockType, ".%s", tempname);
						_blockTable[i].blockTypeID = getBlockType(tempname);
					}
				}
				if (_blockTable[i].blockType == "") {
					sprintf(_blockTable[i].blockType, "Unknown");
				}
			}

			index += numFiles;
			level++;
			
			for (int i=1; i<numFiles+1; i++) {
				_gui->add_tree_elements(_blockTable[i].blockName, i, level-1, _blockTable[i].blockTypeID);
				_input.seek(_blockTable[i].offset, SEEK_SET);
				index = searchBlocks(_blockTable, _input, index, level, _blockTable[i].blockSize + _blockTable[i].offset);
				_input.seek(_blockTable[i].blockSize + _blockTable[i].offset, SEEK_SET);
			}
			
			break;
		
		case IMAG:
		case BSTR:
			v8 = 1;
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case RMSC:
		case AKOS:
		case ANIM:
			_blockTable[index].blockSize = _input.readUint32BE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case IACT:
			_blockTable[index].blockSize = _input.readUint32BE() + 9;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;
			
		case FRME:
		case SAUD:
			_blockTable[index].blockSize = _input.readUint32BE() + 8;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;

		case PSAD:
			_blockTable[index].blockSize = _input.readUint32BE() + 8;
			_input.readUint16LE(); // Track id
			_input.readUint16LE(); // Index
			_input.readUint16LE(); // Max Frames
			_input.readUint16LE(); // Flags
			_input.readByte(); // Volume
			_input.readByte(); // bal
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			bufindex = index;
			index++;
			level++;
			index = searchBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			_input.seek(_blockTable[bufindex].offset + _blockTable[bufindex].blockSize, SEEK_SET);
			break;
			
		case AHDR:
		case TRES:
		case STRK:
		case XPAL:
			_blockTable[index].blockSize = _input.readUint32BE() + 8;
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case FOBJ:
			_blockTable[index].blockSize = _input.readUint32BE() + 8;
			
			_input.readUint16LE(); // Codec
			_input.readUint16BE(); // Left
			_input.readUint16BE(); // Top
			_blockTable[index].width = _input.readUint16LE(); // Width
			_blockTable[index].height = _input.readUint16LE(); // Height
			_input.readUint16BE();
			_input.readUint16BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
			
		case SDAT:
			_blockTable[index].blockSize = _input.readUint32BE();
			stopflag = 1;
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		default:
			_blockTable[index].blockSize = _input.readUint32BE();
			_input.seek(_blockTable[index].offset + _blockTable[index].blockSize, SEEK_SET);
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;
			
	}
	return index;

}

int Resource::parseOldBlocks(char *blockName, BlockTable *_blockTable, File& _input, int index, int level) {
	
	int numFiles, offset, number;

	strcpy(_blockTable[index].blockDescription, oldBlocksInfo[_blockTable[index].blockTypeID].description);

	switch(_blockTable[index].blockTypeID) {

		case 200: // 0R
		case 201: // OS
		case 202: // ON
		case 203: // OC
			numFiles = _input.readUint16LE();
			_blockTable[index].numFiles = numFiles;
			
			for (int i=0; i<numFiles; i++) {
				number = _input.readByte();
				offset = _input.readUint32LE();
			}
			
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case 204: // 00
			numFiles = _input.readUint16LE();
			_blockTable[index].numFiles = numFiles;
			
			for (int i=0; i<numFiles; i++) {
				_input.readByte(); // Owner of object
			}
			
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case 205: // RO
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			level++;
			index = searchOldBlocks(_blockTable, _input, index, level, _blockTable[index-1].blockSize + _blockTable[index-1].offset);
			break;

			break;
			
		case 206: // HD
			_blockTable[index].width = _input.readUint16LE();
			_blockTable[index].height = _input.readUint16LE();
			_blockTable[index].localObjects = _input.readUint16LE();
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			index++;
			break;

		case 207: // BX
		case 208:
		case 209:
		case 210:
		case 212:
			_gui->add_tree_elements(_blockTable[index].blockName, index, level, _blockTable[index].blockTypeID);
			_input.seek(_blockTable[index].blockSize + _blockTable[index].offset, SEEK_SET);
			index++;
			break;
			
	}
	return index;

}


int Resource::getBlockType(char *tag) {
	
	for (int i=0; i<110; i++) {
		if(strstr(tag, blocksInfo[i].name)) {
			return blocksInfo[i].id;
		}

	}
	//printf("Unknown block: %s\n", tag);
	//exit(1);
	return -1;
}

int Resource::getOldBlockType(char *tag) {
	
	for (int i=0; i<12; i++) {
		if(strstr(tag, oldBlocksInfo[i].name)) {
			return oldBlocksInfo[i].id;
		}
	}
	
	return -1;
}

int Resource::findBlock(int direction, BlockTable * _blockTable, File& _input, int id, ...)
{
	va_list arg_ptr;
	char *test;
	int tempid;
	tempid = id;

	va_start(arg_ptr, id);

	while (strcmp(test = (va_arg(arg_ptr, char *)) , "-1")) {
		id = tempid;
		if (direction == 0) {
			id -= 1;
			while (id >= 0) {
				if(strstr(test, _blockTable[id].blockName)) {
					return id;
				}
				id--;
			}
		}

		if (direction == 1) {
			id += 1;
			while (1) {
				if(strstr(test, _blockTable[id].blockName)) {
					return id;
				}
				id++;
			}
		}
	}
	
	va_end(arg_ptr);
	return -1;
}

