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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/wxwindows.h,v 1.18 2004/02/14 20:51:11 sev Exp $
 *
 */

#ifndef wxwindows_h
#define wxwindows_h

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/html/helpctrl.h"
#include "wx/grid.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/listctrl.h"
#include "wx/statline.h"
#include "blocks.h"
#include "scummsys.h"

enum {
	FLAG_NONE = 1 << 0,
	IMAGE_BOXES = 1 << 1,
	IMAGE_SCALE = 1 << 2
};

class ImageScrolledWindow;

class MainWindow : public wxFrame {
private:
	wxHtmlHelpController *HtmlHelp;
	wxHtmlWindow *HtmlWindow;
	wxNotebook *notebook;
	wxBitmap bigIconBitmap;
	wxStaticBitmap *BigIcon;
	wxPanel *panel, *infospanel, *htmlpanel, *searchpanel;
	int htmlflag;
	wxStaticText *_GenLabel[5][2];
	wxStaticText *_SpecLabel[6][2];
	wxButton *SpecButton1, *SpecButton2;
	wxTreeItemId iter[11];
	wxBitmap _bigIcon[61];
	wxTreeCtrl *tree;
	wxFileName _file;

	const char *_filename;
	wxString _filepath;

	uint32 _blockId;

public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

	void SetButton(int blocktype);
	void add_tree_elements(char *itemName, int blockid, int level, int type);

protected:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnSelChanged(wxTreeEvent& event);

	void FileView(wxEvent& event);
	void FileInfo(wxEvent& event);
	void BlockDump(wxEvent& event);
	void SaveSOU(wxEvent& event);
	void SaveiMUSE(wxEvent& event);

	void Descumm(wxEvent& event);
	void iMUSEPlay(wxEvent& event);
	void SOUPlay(wxEvent& event);
	void paletteDraw(wxEvent& event);
	void bgDraw(wxEvent& event);
	void SmushFrameDraw(wxEvent& event);
	void objectDraw(wxEvent& event);
	void boxesDraw(wxEvent& event);

	void updateLabels(int blockid);
	void updateLabel(wxStaticText *label[2], const char *title, uint32 text);

	void FileInfoDialog(int size, int encbyte);

	DECLARE_EVENT_TABLE()
};

class ImageWindow : public wxFrame {
public:
	int _scaleFactor;
	wxSize _clientSize;
	wxImage *_image;
	wxMenu *menuZoom;
	
	ImageWindow(const wxString& title, const wxSize& size, int blockId, byte flags = FLAG_NONE, int scaleFactor = 0);
	void DrawImage();
	void PutPixel(int x, int y, int red, int green, int blue);

protected:
	ImageScrolledWindow *_imageScrolledWindow;
	
	int _blockId;
	int _boxesDisplayed;

	void OnQuit(wxCommandEvent& event);
	void SaveImage(wxEvent& event);
	void boxesDrawOverlay(wxEvent& event);
	void Scale1x(wxEvent& event);
	void Scale2x(wxEvent& event);
	void Scale3x(wxEvent& event);
	void Scale();

	DECLARE_EVENT_TABLE()
};

class ImageScrolledWindow : public wxScrolledWindow {
public:
	ImageScrolledWindow(ImageWindow *imageWindow);

protected:
	ImageWindow *_imageWindow;
	void OnPaint(wxPaintEvent &event);

	DECLARE_EVENT_TABLE()
};

class ViewerWindow : public wxFrame {
public:
	ViewerWindow(MainWindow *parent, const wxString& title, const wxString& text, const wxPoint& pos, const wxSize& size);

protected:
	void OnQuit(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

class GUI_wxWindows : public wxApp {
private:
	wxConfig *config;

public:
	GUI_wxWindows();
	~GUI_wxWindows();
	
	MainWindow *_mainWindow;
	void EnableToolbarTool(int tool);
	void DisableToolbarTool(int tool);
	void AppendText(char *text);
	void DisplayHelp();
	void SetTitle(char *title);
	void DisplayViewer(char *title, int width, int height, char *text);
	void DisplayDialog(char *message, char *title);
	virtual bool OnInit();
	void add_tree_elements(char *itemName, int blockid, int level, int type);
	int getScummVersionDialog();
	bool readConfigValue(const char* key, int* string);
	bool readConfigValue(const char* key, wxString* string);
	bool writeConfigValue(const char* key, int string);
	bool writeConfigValue(const char* key, wxString& string);
};

class TreeItemData : public wxTreeItemData {
	public:
		TreeItemData(int blockid, int blocktype) : _blockId(blockid), _blockType(blocktype) { }
		int _blockId;
		int _blockType;
};
		
enum
{
	ID_SoundStop = 1,
	ID_Dump,
	ID_View,
	ID_SpecButton1,
	ID_SpecButton2,
	Viewer_Quit,
	Button_Open,
	Button_Close,
	Button_Save,
	Button_Options,
	ID_Help,
	ID_FileInfo,
	ID_Hex,
	ID_ImageWindow,
	ID_ViewerWindow,
	ID_BMP,
	ID_Boxes,
	ID_Tree,
	ID_Scale1x,
	ID_Scale2x,
	ID_Scale3x,
	ID_Zoom
};

extern GUI_wxWindows *_gui;

#endif
