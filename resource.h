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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/resource.h,v 1.14 2003/09/29 09:43:18 khalek Exp $
 *
 */

#ifndef resource_h
#define resource_h

#include "scummsys.h"
#include "wxwindows.h"

struct BlockTable {
	char blockName[29];
	char blockType[13];
	char blockDescription[256];
	char name[256];
	int32 blockTypeID;
	uint32 offset;
	int numFiles;
	uint32 blockSize;
	uint32 variables;
	uint32 bitVariables;
	uint32 localObjects;
	uint32 arrays;
	uint32 characters;
	uint32 invObjects;
	int32 width;
	int trans;
	int32 height;
	ImageWindow *image;
};

struct RoomTable {
	uint32 number;
	uint32 name;
	uint32 offset;
};

struct blockInfo {
	int id;
	char name[5];
	char description[256];
	int html;
	char htmlfile[256];
	int iconid;
};

const struct blockInfo blocksInfo[] = {
	{0, "RNAM", "Room Names", 1, "help/specRNAM.html", 47},
	{1, "MAXS", "Maximum Values", 0, "", 2},
	{2, "LB83", "Lucasarts Bundle 8.3", 1, "help/specLB83.html", 6},
	{3, "LABN", "Lucasarts Bundle New", 1, "help/specLABN.html", 6},
	{4, "LECF", "LucasArts Entertainment Company Files", 1, "help/specLECF.html", 6},
	{5, "LOFF", "Room Offset Table", 1, "help/specLOFF.html", 44},
	{6, "LFLF", "LucasArts File Format", 1, "help/specLFLF.html", 58},
	{7, "ROOM", "Room Container", 1, "help/specROOM.html", 38},
	{8, "RMHD", "Room Header", 0, "", 35},
	{9, "CYCL", "Color Cycle", 0, "", 12},
	{10, "TRNS", "Transparency", 1, "help/specTRNS.html", 46},
	{11, "EPAL", "Enhanced Palette", 0, "", 34},
	{12, "BOXD", "Box Description", 0, "", 4},
	{13, "BOXM", "Box Matrix", 0, "", 5},
	{14, "OBCD", "Object Code", 1, "help/specOBCD.html", 57},
	{15, "OBNA", "Object Name", 1, "help/specOBNA.html", 45},
	{16, "CDHD", "Code Header", 0, "", 7},
	{17, "VERB", "Verbs", 0, "", 50},
	{18, "DROO", "Directory of Rooms", 0, "", 18},
	{19, "DSCR", "Directory of Scripts", 0, "", 19},
	{20, "DSOU", "Directory of Sounds", 0, "", 20},
	{21, "DCOS", "Directory of Costumes", 0, "", 14},
	{22, "DCHR", "Directory of Charsets", 0, "", 13},
	{23, "DOBJ", "Directory of Objects", 0, "", 17},
	{24, "cus1", "bundled file"},
	{25, "cus2", "iMUSE Cue", 0, "", 59},
	{26, "COMP", "Compression Table", 0, "", 9},
	{27, "CLUT", "Color Lookup Table", 1, "help/specCLUT.html", 34},
	{28, "PALS", "Wrapper for Palettes", 1, "help/specPALS.html", 56},
	{29, "WRAP", "Wrapper for Palettes/Images", 1, "help/specWRAP.html", 60},
	{30, "OFFS", "Offsets", 1, "help/specOFFS.html", 44},
	{31, "APAL", "Palette", 1, "help/specAPAL.html", 34},
	{32, "RMIM", "Room Image", 0, "", 37},
	{33, "RMIH", "Room Image Header", 0, "", 36},
	{34, "IM00", "Image", 0, "", 25},
	{35, "SMAP", "Pixelmap", 0, "", 42},
	{36, "SCAL", "Scaling", 0, "", 40},
	{37, "AARY", "Array?", 0, "", 47},
	{38, "ANAM", "", 0, "", 47},
	{39, "SOUN", "Sound", 0, "", 59},
	{40, "SCRP", "Script", 0, "", 41},
	{41, "EXCD", "Exit Code", 0, "", 22},
	{42, "ENCD", "Entry Code", 0, "", 21},
	{43, "NLSC", "Non Local Script", 0, "", 41},
	{44, "OBIM", "Object Image", 0, "", 33},
	{45, "IMHD", "Image Header", 0, "", 26},
	{46, "SOU", "Sound", 0, "", 59},
	{47, "VCTL", "", 0, "", 15},
	{48, "Crea", "", 0, "", 16},
	{49, "VTLK", ""},
	{50, "MCMP", "", 1, "help/specMCMP.html", 9},
	{51, "iMUS", "iMUSE Digital Sound", 1, "help/speciMUS.html", 59},
	{52, "MAP ", "iMUSE MAP", 1, "help/specMAP.html", 29},
	{53, "FRMT", "iMUSE Sound Format", 1, "help/specFRMT.html", 51},
	{54, "TEXT", "", 0, "", 45},
	{55, "REGN", "", 0, "", 27},
	{56, "JUMP", "", 1, "help/specJUMP.html", 27},
	{57, "DATA", "", 1, "help/specDATA.html", 16},
	{58, "STOP", "", 1, "help/specSTOP.html", 27},
	{59, "SYNC", "", 0, "", 43},
	{60, "IM01", "Image", 0, "", 25},
	{61, "IM02", "Image", 0, "", 25},
	{62, "IM03", "Image", 0, "", 25},
	{63, "IM04", "Image", 0, "", 25},
	{64, "IM05", "Image", 0, "", 25},
	{65, "IM06", "Image", 0, "", 25},
	{66, "IM07", "Image", 0, "", 25},
	{67, "IM08", "Image", 0, "", 25},
	{68, "IM09", "Image", 0, "", 25},
	{69, "IMC", "", 1, "help/specIMC.html", 59},
	{70, "ZP01", "", 0, "", 52},
	{71, "LSCR", "", 0, "", 41},
	{72, "COST", "", 0, "", 11},
	{73, "ZP02", "", 0, "", 52},
	{74, "CHAR", "", 0, "", 8},
	{75, "ZP03", "", 0, "", 52},
	{76, "IM0A", "Image", 0, "", 25},
	{77, "IM0B", "Image", 0, "", 25},
	{78, "IM0C", "Image", 0, "", 25},
	{79, "IM0D", "Image", 0, "", 25},
	{80, "IM0E", "Image", 0, "", 25},
	{81, "IM0F", "Image", 0, "", 25},
	{82, "IM10", "Image", 0, "", 25},
	{83, "BOMP", ""},
	{84, "IMAG", "", 0, "", 25},
	{85, "AKOS", "", 0, "", 47},
	{86, "ZP04", "", 0, "", 52},
	{87, "RMSC", "", 1, "help/specRMSC.html", 57},
	{88, "BSTR", "", 0, "", 42},
	{89, "ZPLN", "", 0, "", 52},
	{90, "AKHD", "", 0, "", 47},
	{91, "AKPL", "", 0, "", 47},
	{92, "RGBS", "", 0, "", 47},
	{93, "AKOF", "", 0, "", 47},
	{94, "AKCI", "", 0, "", 47},
	{95, "AKCD", "", 0, "", 47},
	{96, "AKSQ", "", 0, "", 47},
	{97, "AKCH", "", 0, "", 47},
	{98, "ANIM", "SMUSH Animation", 0, "", 55},
	{99, "AHDR", "SMUSH Animation Header", 0, "", 1},
	{100, "FRME", "SMUSH Frame", 0, "", 55},
	{101, "FOBJ", "SMUSH Frame Object", 0, "", 32},
	{102, "IACT", "iMUSE Action", 0, "", 24},
	{103, "NPAL", "New Palette", 0, "", 34},
	{104, "TRES", "Text Resource", 0, "", 47},
	{105, "PSAD", "", 0, "", 24},
	{106, "SAUD", "", 0, "", 24},
	{107, "STRK", "", 0, "", 24},
	{108, "SDAT", "", 0, "", 24},
	{109, "XPAL", "", 0, "", 34},
	{110, "ADL ", "", 1, "help/specADL.html", 31},
	{111, "SPK ", "", 1, "help/specSPK.html", 31},
	{112, "ROL ", "", 1, "help/specROL.html", 31},
	{113, "FTCH", "", 0, "", 47},
	{114, "STOR", "", 0, "", 47},
	{115, "MIDI", "", 0, "", 31},
	{116, "GMD ", "", 0, "", 31},
	{117, "SBL ", "", 0, "", 31},
	{118, "ZSTR", "", 0, "", 47},
	{119, "AUhd", "", 0, "", 31},
	{120, "WVhd", "", 0, "", 31},
	{121, "AUdt", "", 0, "", 31},
	{122, "WVdt", "", 0, "", 31},
	{123, "TLKB", "", 0, "", 47},
	{124, "TALK", "", 0, "", 47},
	{125, "HSHD", "", 0, "", 47},
	{126, "DIRI", "", 0, "", 47},
	{127, "DIRR", "", 0, "", 47},
	{128, "DIRN", "", 0, "", 47},
	{129, "DIRC", "", 0, "", 47},
	{130, "DIRM", "", 0, "", 47},
	{131, "DLFL", "", 0, "", 47},
	{132, "DIRS", "", 0, "", 47}
};

const int numBlocks = 133;

const struct blockInfo oldBlocksInfo[] = {
	{200, "0R", "Directory of Rooms", 0, "", 18},
	{201, "0S", "", 0, "", 19},
	{202, "0N", "", 0, "", 47},
	{203, "0C", "", 0, "", 14},
	{204, "0O", "Directory of Objects", 0, "", 17},
	{205, "RO", "", 0, "", 38},
	{206, "HD", "", 0, "", 35},
	{207, "BX", "", 0, "", 4},
	{208, "PA", "", 0, "", 34},
	{209, "BM", "", 0, "", 37},
	{210, "OI", "", 0, "", 33},
	{211, "OC", "", 0, "", 41},
	{212, "NL", "", 0, "", 41},
	{213, "SL", "", 0, "", 47},
	{214, "EX", "", 0, "", 22},
	{215, "EN", "", 0, "", 21},
	{216, "LC", "", 0, "", 47},
	{217, "LS", "", 0, "", 41},
	{218, "SC", "", 0, "", 41},
	{219, "CO", "", 0, "", 11},
	{220, "LE", "LucasArts Entertainment Company Files", 0, "", 6},
	{221, "FO", "", 0, "", 44},
	{222, "LF", "", 0, "", 58},
	{223, "CC", "", 0, "", 12},
	{224, "SP", "", 0, "", 47},
	{225, "SA", "", 0, "", 40},
	{226, "SO", "", 0, "", 59},
	{227, "RN", "", 0, "", 47}
};

class Resource {
private:
	int stopflag;	
public:
	Resource();
	int getBlockType(char *tag);
	int getOldBlockType(char *tag);
	int searchBlocks(BlockTable *_blockTable, File& _input, int index = 0, int level = 1, int size = 0);
	int searchOldBlocks(BlockTable *_blockTable, File& _input, int index = 0, int level = 1, int size = 0);
	int parseBlocks(char *blockName, BlockTable *_blockTable, File& _input, int index, int level);
	int parseOldBlocks(char *blockName, BlockTable *_blockTable, File& _input, int index, int level);
	int findBlock(int direction, BlockTable * _blockTable, int id, ...);
};

#endif
