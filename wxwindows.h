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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/wxwindows.h,v 1.3 2003/09/19 19:57:07 yoshizf Exp $
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

class MainWindow : public wxFrame {
private:
	wxHtmlHelpController *HtmlHelp;
	wxHtmlWindow *HtmlWindow;
	wxNotebook *notebook;
	wxBitmap bigIconBitmap;
	wxStaticBitmap *BigIcon;
	wxPanel *panel, *infospanel, *htmlpanel, *searchpanel;
	int htmlflag;

public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnSelChanged(wxTreeEvent& event);
};

class ImageWindow : public wxFrame {
public:
	void DrawImage();
	void OnQuit(wxCommandEvent& event);
	ImageWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
};

class ViewerWindow : public wxFrame {
public:
	void OnQuit(wxCommandEvent& event);
	ViewerWindow(const wxString& title, const wxString& text, const wxPoint& pos, const wxSize& size);
};

class GUI_wxWindows : public wxApp {
private:

public:
	GUI_wxWindows();
	void BlockDump();
	void SaveSOU();
	void SaveiMUSE();
	void EnableToolbarTool(int tool);
	void DisableToolbarTool(int tool);
	void AppendText(char *text);
	void FileInfoDialog(int size, int encbyte);
	void DrawImage();
	void DisplayHelp();
	void SetTitle(char *title);
	void SaveImage();
	void DisplayViewer(char *title, int width, int height, char *text);
	void PutPixel(int x, int y, int red, int green, int blue);
	void DisplayImage(char* title, int width, int height);
	void SetButton(int blocktype);
	void DisplayDialog(char *message, char *title);
	void updateLabel(char *label, char *title, uint32 text);
	virtual bool OnInit();
	void add_tree_elements(char *itemName, int blockid, int level, int type);
	int getScummVersionDialog();
};

class TreeItemData : public wxTreeItemData {
	public:
		TreeItemData(int blockid, int blocktype) : _blockId(blockid), _blockType(blocktype) { }
		int _blockId;
		int _blockType;
};
		
enum
{
	ID_Quit=1,
	ID_About,
	ID_Open,
	ID_Browse,
	ID_SoundStop,
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
	ID_Close,
	ID_FileInfo,
	ID_Hex,
	ID_ImageWindow,
	ID_ViewerWindow,
	ID_BMP,
	Tree
};

#endif
