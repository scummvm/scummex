#ifndef sound_h
#define sound_h

#include "SDL.h"
#include "scummsys.h"
#include "file.h"
#include "resource.h"
#include "mixer.h"
#include "wxwindows.h"

struct CompTable {
	int32 offset;
	int32 size;
	int32 codec;
};
	
class Sound {
private:
	Mixer *_mixer;
	Resource *_resource;
	GUI_wxWindows *_gui;

	static int32 Sound::compDecode(byte * src, byte * dst);

public:
	Sound();
	~Sound();
	int playiMUSE(File& _input, BlockTable *_blockTable, int index, File& _output, int save = 0);
	int playSOU(BlockTable *_blockTable, File& _input, int index, File& _output, int save = 0);
	void parseSOU(BlockTable *_blockTable, File& _input);

};

#endif
