#ifndef scummex_h
#define scummex_h

#include "resource.h"
#include "image.h"
#include "sound.h"
#include "wxwindows.h"

class ScummEX {
private:
	byte encbyte;
	
	Image *_image;
	Sound *_sound;
	Resource *_resource;
	GUI_wxWindows *_gui;

public:

	ScummEX();
	void fileView();
	void Descumm();
	void FileInfo();
	void UpdateInfosFromTree(int blockid); 
	void getFileType(const char *filename);
	void updateLabel(char *label, char *title, uint32 text);
	void add_tree_elements(char *itemName, int blockid, int level);
	void warning(const char *s, ...);
	void debug(int level, const char *s, ...);
	void error(const char *s, ...);
	void FileDump(const char *filename);
	void iMUSEPlay();
	void iMUSEDump(const char *filename);
	void SOUPlay();
	void paletteDraw();
	void bgDraw();
	void objectDraw();
};

#endif
