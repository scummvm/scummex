#ifndef image_h
#define image_h

#include "scummsys.h"

struct rgbtable {
	int red;
	int green;
	int blue;
};

class Image {
private:
	struct rgbtable _rgbTable[256];
	uint32 palette[256];
	int transp;
	uint32 *offsets;
	int width;
	int height;
	Resource *_resource;
	GUI_wxWindows *_gui;

public:
	Image();
	~Image();
	int drawPalette(BlockTable *_blockTable, int id, File& _input);
	int drawBG(File& _input, BlockTable *_blockTable, int id, char* filename);
	int drawObject(File& _input, BlockTable *_blockTable, int id, File& _output, int save = 0);
	void decode_uncompressed(uint16 height, File& _input);
	void decode_horiz(uint16 height, uint8 compr, File& _input);
	void decode_vert(uint16 height, uint8 compr, File& _input);
	void decode_horiz_transp(uint16 height, uint8 compr, File& _input);
	void decode_vert_transp(uint16 height, uint8 compr, File& _input);
	void decode2(uint16 height, uint8 compr, File& _input);
	void decode2transp(uint16 height, uint8 compr, File& _inpuit);
	void GetStrip( uint8 pos, File& _input);
};

#endif
