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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/wxwindows.cpp,v 1.36 2003/10/01 14:12:52 yoshizf Exp $
 *
 */

#include "wxwindows.h"
#include "file.h"
#include "scummex.h"
#include "icons.h"

ScummEX *g_scummex = 0;
wxTextCtrl *hexdata = 0;
wxToolBar *ToolBar = 0;
GUI_wxWindows *_gui = 0;

IMPLEMENT_APP(GUI_wxWindows)

GUI_wxWindows::GUI_wxWindows()
	: _mainWindow(0) {
	g_scummex = new ScummEX();
	_gui = this; // FIXME - ugly quick workaround for previously broken code
	config = new wxConfig("ScummEX");
}

GUI_wxWindows::~GUI_wxWindows() {
	delete g_scummex;
	delete config;
}

bool GUI_wxWindows::OnInit()
{
	_mainWindow = new MainWindow("ScummEX", wxPoint(-1,-1),
                wxSize(640,480));

	_mainWindow->Show(TRUE);
	SetTopWindow(_mainWindow);
	return TRUE;
}

void GUI_wxWindows::SetTitle(char *title) {
	_mainWindow->SetTitle(title);
}

int GUI_wxWindows::getScummVersionDialog() {
	int choice;
	wxString games[22] = {
	"Maniac Mansion (C64)",
	"Maniac Mansion (PC)",
	"Zak McKracken",
	"Zak McKracken (FM Towns)",
	"Indiana Jones & The Last Crusade",
	"Sampler (Loom, Indy3, SOMI)",
	"The Secret of Monkey Island (Demo)",
	"The Secret of Monkey Island (EGA)",
	"The Secret of Monkey Island",
	"Loom",
	"Loom (CD)",
	"Monkey Island 2: LeChuck's Revenge",
	"Indiana Jones & The Fate of Atlantis",
	"Day of the Tentacle",
	"Sam & Max Hit The Road (Demo)",
	"Sam & Max Hit The Road (Mac demo)",
	"Sam & Max Hit The Road (Disk)",
	"Sam & Max Hit The Road (CD)",
	"Full Throttle",
	"The Dig",
	"The Dig (Win95)",
	"The Curse of Monkey Island"
	};
	
	wxSingleChoiceDialog *dialog = new wxSingleChoiceDialog(_mainWindow, "Please select the correct game", "Scumm version selection", 22, games, NULL, wxOK|wxCANCEL|wxCENTRE, wxDefaultPosition);
	if (dialog->ShowModal() == wxID_OK) {
		choice = dialog->GetSelection();
		switch (choice) {
			case 0:
				return 1;
			case 1:
			case 2:
				return 2;
			case 3:
			case 4:
			case 9:
				return 3;
			case 5:
			case 6:
			case 7:
				return 4;
			case 8:
			case 10:
			case 11:
			case 12:
				return 5;
			case 13:
			case 14:
			case 16:
				return 6;
			case 17:
			case 18:
			case 19:
				return 7;
			case 20:
			case 21:
				return 8;
		}
	}
	return 0;
}

bool GUI_wxWindows::readConfigValue(const char* key, int* string) {
	bool result;
	result = config->Read(key, string);
	return result;
}

bool GUI_wxWindows::readConfigValue(const char* key, wxString* string) {
	bool result;
	result = config->Read(key, string);
	return result;
}

bool GUI_wxWindows::writeConfigValue(const char* key, int string) {
	bool result;
	result = config->Write(key, string);
	config->Flush();
	return result;
}

bool GUI_wxWindows::writeConfigValue(const char* key, wxString& string) {
	bool result;
	result = config->Write(key, string);
	config->Flush();
	return result;
}

BEGIN_EVENT_TABLE(ImageScrolledWindow, wxScrolledWindow)
	EVT_PAINT(ImageScrolledWindow::OnPaint)
END_EVENT_TABLE()

ImageScrolledWindow::ImageScrolledWindow(ImageWindow *imageWindow) : wxScrolledWindow (imageWindow, -1) {
	_imageWindow = imageWindow;
}

void ImageScrolledWindow::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.DrawBitmap(wxBitmap(_imageWindow->_image), 0, 0);
}

BEGIN_EVENT_TABLE(ImageWindow, wxFrame)
	EVT_CLOSE(ImageWindow::OnQuit)
	EVT_MENU(Viewer_Quit, ImageWindow::OnQuit)
	EVT_MENU(ID_BMP, ImageWindow::SaveImage)
	EVT_MENU(ID_Boxes, ImageWindow::boxesDrawOverlay)
	EVT_MENU(ID_Scale1x, ImageWindow::Scale1x)
	EVT_MENU(ID_Scale2x, ImageWindow::Scale2x)
	EVT_MENU(ID_Scale3x, ImageWindow::Scale3x)
END_EVENT_TABLE()

ImageWindow::ImageWindow(const wxString& title, const wxSize& size, int blockId, byte flags, int scaleFactor)
	: wxFrame(_gui->_mainWindow, -1, title, wxPoint(-1,-1), size)
{
	wxMenuBar *menuBar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;
	_blockId = blockId;
	_boxesDisplayed = 0;
	_clientSize = size;
	
	wxMenuItem *BMPItem = new wxMenuItem(menuFile, ID_BMP, "Save to BMP...", "Save image to .BMP", wxITEM_NORMAL, NULL );
	wxBitmap SaveIcon = wxBitmap(save_icon);
	BMPItem->SetBitmap(SaveIcon);
	menuFile->Append(BMPItem);

	menuFile->Append(Viewer_Quit,"Close");
	
	menuBar->Append(menuFile,"&File");

	wxMenu *menuView = new wxMenu;

	if (scaleFactor == 0) {
		_scaleFactor = 1;
		_gui->readConfigValue("Scaler", &_scaleFactor);
	} else {
		_scaleFactor = scaleFactor;
	}
	
	if (flags & IMAGE_BOXES) {
		menuView->AppendCheckItem(ID_Boxes, "Display Boxes", "Display Boxes");
	}

	if (flags & IMAGE_SCALE) {
		menuZoom = new wxMenu;
		menuZoom->AppendCheckItem(ID_Scale1x, "100%", "100%");
		menuZoom->AppendCheckItem(ID_Scale2x, "200%", "200%");
		menuZoom->AppendCheckItem(ID_Scale3x, "300%", "300%");
		menuView->Append(ID_Zoom, "Zoom", menuZoom, "Zoom");
		switch(_scaleFactor) {
			case 1:
				menuZoom->Check(ID_Scale1x, TRUE);
				break;

			case 2:
				menuZoom->Check(ID_Scale2x, TRUE);
				break;

			case 3:
				menuZoom->Check(ID_Scale3x, TRUE);
				break;
		}
	}
	
	menuBar->Append(menuView,"View");
	SetMenuBar(menuBar);

	SetClientSize(size.GetWidth() * _scaleFactor+20, size.GetHeight() * _scaleFactor+20);
	_image = new wxImage(size.GetWidth() * _scaleFactor, size.GetHeight() * _scaleFactor);
	
	_imageScrolledWindow = new ImageScrolledWindow(this);

	_imageScrolledWindow->SetVirtualSize(size.GetWidth() * _scaleFactor, size.GetHeight() * _scaleFactor);
	_imageScrolledWindow->SetScrollRate(10, 10);
	SetSizeHints(130, 80, wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 25, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 25);
}

void ImageWindow::DrawImage() {
	wxClientDC dc(this);
	dc.DrawBitmap(wxBitmap(_image), 0, 0);
	Show(TRUE);
	Refresh();
}

void ImageWindow::Scale1x(wxEvent& event) {
	_scaleFactor = 1;
	menuZoom->Check(ID_Scale2x, FALSE);
	menuZoom->Check(ID_Scale3x, FALSE);
	Scale();
}

void ImageWindow::Scale2x(wxEvent& event) {
	_scaleFactor = 2;
	menuZoom->Check(ID_Scale1x, FALSE);
	menuZoom->Check(ID_Scale3x, FALSE);
	Scale();
}

void ImageWindow::Scale3x(wxEvent& event) {
	_scaleFactor = 3;
	menuZoom->Check(ID_Scale1x, FALSE);
	menuZoom->Check(ID_Scale2x, FALSE);
	Scale();
}

void ImageWindow::Scale() {
	delete _image;
	_image = new wxImage(_clientSize.GetWidth() * _scaleFactor, _clientSize.GetHeight() * _scaleFactor);

	SetClientSize(_clientSize.GetWidth() * _scaleFactor+20, _clientSize.GetHeight() * _scaleFactor+20);
	_imageScrolledWindow->SetVirtualSize(_clientSize.GetWidth() * _scaleFactor, _clientSize.GetHeight() * _scaleFactor);
	
	switch (g_scummex->getBlockTable(_blockId).blockTypeID) {
		case IM01:
		case IM02:
		case IM03:
		case IM04:
		case IM05:
		case IM06:
		case IM07:
		case IM08:
		case IM09:
		case IM0A:
		case IM0B:
		case IM0C:
		case IM0D:
		case IM0E:
		case IM0F:
		case OI:
		case OBIM:
			g_scummex->objectDraw(_blockId);
			break;

		case CLUT:
		case APAL:
		case PA:
		case NPAL:
		case AHDR:
		case RGBS:
			g_scummex->paletteDraw(_blockId);
			break;

		case BM:
		case RMIM:
		case IMAG:
			g_scummex->bgDraw(_blockId);
			if (_boxesDisplayed)
				g_scummex->boxesDraw(_blockId);
			break;

		case BOXD:
		case BX:
			g_scummex->boxesDraw(_blockId);
			break;

		case FOBJ:
			g_scummex->SmushFrameDraw(_blockId);
			break;
	}
}

void ImageWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	delete _image;
	g_scummex->getBlockTable(_blockId).image = NULL;
	Destroy();
}

void ImageWindow::SaveImage(wxEvent& event) {
	wxFileDialog *dialog = new wxFileDialog(this, "Please select an output file.", "", "",
		"*",
		wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		const char *filename = (const char*)dialog->GetPath();
		_image->SaveFile(filename, wxBITMAP_TYPE_BMP);
	}
}

void ImageWindow::boxesDrawOverlay(wxEvent& event) {
	if (!_boxesDisplayed) {
		_boxesDisplayed = 1;
		g_scummex->boxesDraw(_blockId);
	} else {
		_boxesDisplayed = 0;
		g_scummex->bgDraw(_blockId);
	}
}

void ImageWindow::PutPixel(int x, int y, int red, int green, int blue) {
	_image->SetRGB(x, y, (unsigned char) red, (unsigned char) green, (unsigned char) blue);
}

void GUI_wxWindows::DisplayViewer(char *title, int width, int height, char *text) {
	ViewerWindow *viewerFrame = new ViewerWindow(_mainWindow, title, text, wxPoint(50,50), wxSize(width, height));
}

void GUI_wxWindows::AppendText(char *text) {
	hexdata->AppendText(text);
}

BEGIN_EVENT_TABLE(ViewerWindow, wxFrame)
	EVT_CLOSE(ViewerWindow::OnQuit)
	EVT_MENU(Viewer_Quit, ViewerWindow::OnQuit)
END_EVENT_TABLE()

ViewerWindow::ViewerWindow(MainWindow *parent, const wxString& title, const wxString& text, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, ID_ViewerWindow, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(Viewer_Quit,"Close");
	
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile,"&File");
	
	SetMenuBar(menuBar);

	hexdata = new wxTextCtrl(this, -1, text, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE|wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);

	hexdata->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL));

	Show(TRUE);

}

void ViewerWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Destroy();
}

void GUI_wxWindows::DisplayDialog(char *message, char *title) {
	wxMessageBox(message, title, wxOK, _mainWindow);
}

void GUI_wxWindows::EnableToolbarTool(int tool) {
	ToolBar->EnableTool(tool, TRUE);
}

void GUI_wxWindows::DisableToolbarTool(int tool) {
	ToolBar->EnableTool(tool, FALSE);
}

void GUI_wxWindows::add_tree_elements(char *itemName, int blockid, int level, int type) {
	_mainWindow->add_tree_elements(itemName, blockid, level, type);
}

void MainWindow::SetButton(int blocktype) {
	SpecButton1->Enable(TRUE);
	SpecButton2->Enable(TRUE);
	switch(blocktype) {
		case cus2: // iMUSE
			SpecButton1->SetLabel("Play");
			SpecButton2->SetLabel("Dump to WAV...");
			SpecButton1->Show(TRUE);
			SpecButton2->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::iMUSEPlay );
			Connect( ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::SaveiMUSE );
			break;

		case OBIM:
			if (g_scummex->getBlockTable(_blockId+3).blockTypeID == 35) {
				SpecButton1->SetLabel("View Object");
				SpecButton1->Show(TRUE);
				Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
					(wxObjectEventFunction) &MainWindow::objectDraw );
			}
			break;

		case IM01:
		case IM02:
		case IM03:
		case IM04:
		case IM05:
		case IM06:
		case IM07:
		case IM08:
		case IM09:
		case IM0A:
		case IM0B:
		case IM0C:
		case IM0D:
		case IM0E:
		case IM0F:
		case OI:
		if (g_scummex->getBlockTable(_blockId).blockSize > 8) {
			SpecButton1->SetLabel("View Object");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::objectDraw );
		}
			break;

		case Crea:
		case AUdt:
			SpecButton1->SetLabel("Play");
			SpecButton2->SetLabel("Dump to WAV...");
			SpecButton1->Show(TRUE);
			SpecButton2->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::SOUPlay );
			Connect( ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::SaveSOU );
			break;
			

		case CLUT:
		case APAL:
		case PA:
		case NPAL:
		case AHDR:
		case RGBS:
			SpecButton1->SetLabel("View Palette");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::paletteDraw );
			break;
		
		case BM:
		case RMIM:
		case IMAG:
			SpecButton1->SetLabel("View Image");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::bgDraw );
			break;

		case FOBJ:
			SpecButton1->SetLabel("View Frame");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::SmushFrameDraw );
			break;
			
		case LSCR:
		case SCRP:
		case ENCD:
		case EXCD:
		case VERB:
		case LS:
		case SC:
		case EN:
		case EX:
		case OC:
			SpecButton1->SetLabel("Decompile Script");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::Descumm );
			break;

		case BOXD:
		case BX:
			SpecButton1->SetLabel("View Boxes...");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::boxesDraw );
			break;
	}

}

void MainWindow::add_tree_elements(char *itemName, int blockid, int level, int type) {

	wxTreeItemId itemid;
	assert(level <= 10);
	itemid = iter[level] = tree->AppendItem(iter[level-1], itemName, -1, -1, new TreeItemData(blockid, type));

	if (type < 200) {
		tree->SetItemImage(itemid, blocksInfo[type].iconid);
		tree->SetItemImage(itemid, blocksInfo[type].iconid, wxTreeItemIcon_Selected);
	} else {
		tree->SetItemImage(itemid, oldBlocksInfo[type-200].iconid);
		tree->SetItemImage(itemid, oldBlocksInfo[type-200].iconid, wxTreeItemIcon_Selected);
	}
	
	if (blockid == 1) {
		tree->EnsureVisible(itemid);
	}
}

void MainWindow::updateLabel(wxStaticText *label[2], const char *title, uint32 text) {
	char buf[256];
	sprintf(buf, "%d", text);
	label[0]->SetLabel(title);
	label[1]->SetLabel(buf);
}

void MainWindow::updateLabels(int blockid) {
	const BlockTable &block = g_scummex->getBlockTable(blockid);
	char buf[256];

	// Clear out all labels
	for (int i = 0; i < 6; i++) {
		_SpecLabel[i][0]->SetLabel("");
		_SpecLabel[i][1]->SetLabel("");
	}
	
	_GenLabel[0][1]->SetLabel(block.blockType);

	sprintf(buf, "%d (0x%08X)", block.offset, block.offset);
	_GenLabel[1][1]->SetLabel(buf);

	sprintf(buf, "%d (0x%08X)", block.blockSize, block.blockSize);
	_GenLabel[2][1]->SetLabel(buf);

	_GenLabel[3][1]->SetLabel(block.blockDescription);

	switch(block.blockTypeID) {
		case MAXS:
			updateLabel(_SpecLabel[0], "Num. of variables:", block.variables);
			updateLabel(_SpecLabel[1], "Num. of bit variables:", block.bitVariables);
			updateLabel(_SpecLabel[2], "Num. of local objects:", block.localObjects);
			updateLabel(_SpecLabel[3], "Num. of arrays:", block.arrays);
			updateLabel(_SpecLabel[4], "Num. of character sets:", block.characters);
			updateLabel(_SpecLabel[5], "Num. of inventory objects:", block.invObjects);
			break;

		case LB83:
		case LABN:
			updateLabel(_SpecLabel[0], "Number of files:", block.numFiles);
			break;

		case RMHD:
		case HD:
			updateLabel(_SpecLabel[0], "Room Width:", block.width);
			updateLabel(_SpecLabel[1], "Room Height:", block.height);
			updateLabel(_SpecLabel[2], "Number of Objects:", block.localObjects);
			break;

		case TRNS:
			updateLabel(_SpecLabel[0], "Transparent Color:", block.trans);
			break;
			
		case DROO:
		case DSCR:
		case DSOU:
		case DCOS:
		case DCHR:
		case DOBJ:
			updateLabel(_SpecLabel[0], "Number of items:", block.numFiles);
			break;

		case COMP:
			updateLabel(_SpecLabel[0], "Number of samples:", block.numFiles);
			break;

		case IMHD:
			updateLabel(_SpecLabel[0], "Object Width:", block.width);
			updateLabel(_SpecLabel[1], "Object Height:", block.height);
			updateLabel(_SpecLabel[2], "Number of Images:", block.numFiles);
			break;
			
		case Crea:
		case AUdt:
			updateLabel(_SpecLabel[0], "Sample Rate:", block.variables);
			break;

		case FOBJ:
			updateLabel(_SpecLabel[0], "Frame Width:", block.width);
			updateLabel(_SpecLabel[1], "Frame Height:", block.height);
			updateLabel(_SpecLabel[2], "Codec:", block.variables);
			break;

		case BOXD:
		case BX:
			updateLabel(_SpecLabel[0], "No. of Boxes:", block.numFiles);
			break;

		case OI:
			updateLabel(_SpecLabel[0], "Object ID:", block.numFiles);
			break;

		case OC:
			updateLabel(_SpecLabel[0], "Object ID:", block.numFiles);
			updateLabel(_SpecLabel[1], "Object Width:", block.width);
			updateLabel(_SpecLabel[2], "Object Height:", block.height);
			break;

	}
	SetButton(block.blockTypeID);
}

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
	EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
	EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
	EVT_MENU(ID_View, MainWindow::FileView)
	EVT_MENU(ID_Dump, MainWindow::BlockDump)
	EVT_MENU(wxID_HELP, MainWindow::OnHelp)
	EVT_MENU(ID_FileInfo, MainWindow::FileInfo)
	EVT_CUSTOM(wxEVT_COMMAND_TREE_SEL_CHANGING, ID_Tree, MainWindow::OnSelChanged)
END_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame*)NULL,-1,title,pos,size),
	_filename(0), _blockId(0)
{
	htmlflag = 0;
	
	wxBitmap OpenIcon = wxBitmap(open_icon);
	wxBitmap ExitIcon = wxBitmap(exit_icon);
	wxBitmap SaveIcon = wxBitmap(save_icon);
	wxBitmap OptionsIcon = wxBitmap(options_icon);
	wxBitmap HelpIcon = wxBitmap(help_icon);
	wxBitmap CloseIcon = wxBitmap(close_icon);
	wxBitmap FileIcon = wxBitmap(file_icon);
	wxBitmap HexIcon = wxBitmap(hex_icon);
	

	//
	// File menu
	//
	wxMenu *fileMenu = new wxMenu();
	
	fileMenu->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open resource file");
	fileMenu->Append(wxID_CLOSE, "Close\tCtrl-W", "Close resource file");
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_FileInfo, "File Info", "Show File Info");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, "Exit\tCtrl-X");
	// FIXME: On Mac you *always* say "Quit", but on Windows/Linux, "Exit" is more common, I think?
	// So maybe we'll have to make this compile conditionally...
	

	//
	// Tools menu
	//
	wxMenu *toolsMenu = new wxMenu();

	toolsMenu->Append(ID_Dump, "File Dump...", "Dump block to disk");
	toolsMenu->Append(ID_View, "Hex Viewer...", "View block as hex");
	toolsMenu->AppendSeparator();
	toolsMenu->Append(ID_SoundStop, "Stop Sounds");
	toolsMenu->AppendSeparator();
	toolsMenu->Append(Button_Options, "Options...", "ScummEX Options");

	//
	// Help menu
	//
	wxMenu *helpMenu = new wxMenu();
	helpMenu->Append(wxID_HELP, "Contents");
	helpMenu->Append(wxID_ABOUT, "About...");

	//
	// Add the menus to a menu bar
	//
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(toolsMenu, "&Tools");
	menuBar->Append(helpMenu, "&Help");
	SetMenuBar(menuBar);

	SetThemeEnabled(true);
	
	ToolBar = CreateToolBar();
	
	ToolBar->SetToolBitmapSize(wxSize(16, 16));
	
	ToolBar->AddTool(wxID_OPEN, OpenIcon, "Open...", "Open resource file");
	ToolBar->AddTool(wxID_CLOSE, CloseIcon, "Close", "Close resource file");
	ToolBar->EnableTool(wxID_CLOSE, FALSE);
	ToolBar->AddTool(ID_FileInfo, FileIcon, "File Info", "Show File Info");
	ToolBar->EnableTool(ID_FileInfo, FALSE);
	ToolBar->AddTool(wxID_EXIT, ExitIcon, "Exit", "Exit ScummEX");
	ToolBar->AddSeparator();
	ToolBar->AddTool(ID_Dump, SaveIcon, "File Dump...", "Dump block to disk");
	ToolBar->EnableTool(ID_Dump, FALSE);
	ToolBar->AddTool(ID_View, HexIcon, "Hex Viewer...", "View block as hex");
	ToolBar->EnableTool(ID_View, FALSE);
	ToolBar->AddTool(Button_Options, OptionsIcon, "Options...", "Options");
	ToolBar->EnableTool(Button_Options, FALSE);
	ToolBar->AddSeparator();
	ToolBar->AddTool(wxID_HELP, HelpIcon, "Help", "View help");
	ToolBar->Realize();
	
	panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "panel");
	
	wxBoxSizer *RootSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer *BlocksSizer = new wxBoxSizer( wxHORIZONTAL );
	RootSizer->Add(BlocksSizer, 1, wxALL|wxEXPAND, 0);

	wxImageList *ImageList = new wxImageList(16, 16, true, 61);
	
	LOADICONS;
	LOADBIGICONS;

	tree = new wxTreeCtrl(panel, ID_Tree, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT);
	tree->AssignImageList(ImageList);
	iter[0] = tree->AddRoot("", -1, -1, NULL);
	BlocksSizer->Add(tree, 1, wxALL|wxEXPAND, 4);
	
	wxBoxSizer *InfosSizer = new wxBoxSizer( wxVERTICAL );
	BlocksSizer->Add(InfosSizer, 1, wxALL|wxEXPAND,	5);

	notebook = new wxNotebook(panel, -1, wxPoint(-1, -1), wxSize(-1, -1), 0, "notebook");
	
	wxNotebookSizer *nbs = new wxNotebookSizer( notebook );
	InfosSizer->Add(nbs, 1,	wxALL|wxEXPAND,	0);
	
	//
	// Explorer panel
	//	
	infospanel = new wxPanel(notebook, -1);
	notebook->AddPage(infospanel, "Explorer", true, -1);
	
	wxBoxSizer *panelsizer = new wxBoxSizer( wxVERTICAL );
	
	//
	// General Block Infos
	//	
	wxStaticBox *GenInfosBox = new wxStaticBox(infospanel, -1, "General Block Infos");
	
	wxStaticBoxSizer *GenInfosHSizer = new wxStaticBoxSizer( GenInfosBox, wxHORIZONTAL );
	panelsizer->Add(GenInfosHSizer,	1, wxALL|wxEXPAND, 0);
				
	wxBoxSizer *GenInfosSizer = new wxBoxSizer( wxVERTICAL );
	GenInfosHSizer->Add(GenInfosSizer, 0, wxALL|wxEXPAND, 0);
	
	wxBoxSizer *GenInfosSizer2 = new wxBoxSizer( wxVERTICAL );
	GenInfosHSizer->Add(GenInfosSizer2, 1, wxLEFT|wxEXPAND,	12);

	_GenLabel[0][0] = new wxStaticText(infospanel, -1, "Type:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	_GenLabel[1][0] = new wxStaticText(infospanel, -1, "Offset:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	_GenLabel[2][0] = new wxStaticText(infospanel, -1, "Size:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	_GenLabel[3][0] = new wxStaticText(infospanel, -1, "Description:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	
	for (int i = 0; i < 4; i++) {
		GenInfosSizer->Add(_GenLabel[i][0], 0, wxALL|wxEXPAND, 1);
	}
		
	for (int i = 0; i < 4; i++) {
		_GenLabel[i][1] = new wxStaticText(infospanel, -1, "n/a", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
		GenInfosSizer2->Add(_GenLabel[i][1], 0, wxALL|wxEXPAND, 1);
	}				

	wxBoxSizer *IconSizer = new wxBoxSizer( wxVERTICAL );
	GenInfosHSizer->Add(IconSizer, 0, wxALL, 0);
	
	bigIconBitmap = _bigIcon[60];
	BigIcon = new wxStaticBitmap(infospanel, -1, bigIconBitmap, wxPoint(-1, -1), wxSize(-1, -1), wxALIGN_RIGHT);
	IconSizer->Add(BigIcon,	0, wxALL, 0);

	//
	// Specific Block Infos
	//
	wxStaticBox *SpecInfosBox = new wxStaticBox(infospanel, -1, "Specific Block Infos");
	
	wxStaticBoxSizer *SpecInfosVSizer = new wxStaticBoxSizer( SpecInfosBox, wxVERTICAL );
	panelsizer->Add(SpecInfosVSizer, 1, wxALL|wxEXPAND, 0);

	wxBoxSizer *SpecSizer = new wxBoxSizer( wxHORIZONTAL );
	SpecInfosVSizer->Add(SpecSizer, 0, wxALL|wxEXPAND, 0);
			
	wxBoxSizer *SpecInfosSizer[2]; 
	SpecInfosSizer[0] = new wxBoxSizer( wxVERTICAL );
	SpecSizer->Add(SpecInfosSizer[0], 1, wxRIGHT|wxEXPAND, 5);
	
	SpecInfosSizer[1] = new wxBoxSizer( wxVERTICAL );
	SpecSizer->Add(SpecInfosSizer[1], 1, wxALL|wxEXPAND, 0);
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			_SpecLabel[j][i] = new wxStaticText(infospanel, -1, "");
			SpecInfosSizer[i]->Add(_SpecLabel[j][i], 1, wxALL|wxEXPAND, 1);
		}
	}
	
	HtmlHelp = new wxHtmlHelpController(wxHF_DEFAULT_STYLE & wxHF_CONTENTS);
	HtmlHelp->AddBook(wxFileName("help/help.hhp"));

	wxBoxSizer *SpecButtonsSizer = new wxBoxSizer ( wxHORIZONTAL );
	SpecInfosVSizer->Add(SpecButtonsSizer, 0, wxALL, 0);
	
	SpecButton1 = new wxButton( infospanel, ID_SpecButton1, "", wxPoint(-1, -1), wxSize(120, -1));
	SpecButtonsSizer->Add( SpecButton1, 1, wxALL, 3);
	SpecButton1->Show(FALSE);
	
	SpecButton2 = new wxButton( infospanel, ID_SpecButton2, "", wxPoint(-1, -1), wxSize(120, -1));
	SpecButtonsSizer->Add( SpecButton2, 1, wxALL, 3);
	SpecButton2->Show(FALSE);

	infospanel->SetAutoLayout( true );
	infospanel->SetSizer( panelsizer );
	
	//
	// Block Specifications panel
	//	
	htmlpanel = new wxPanel(notebook, -1);
	
	wxBoxSizer *htmlpanelsizer = new wxBoxSizer( wxVERTICAL );

	HtmlWindow = new wxHtmlWindow(htmlpanel, -1, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO, "htmlWindow");
	htmlpanelsizer->Add(HtmlWindow,	1, wxALL|wxEXPAND, 0);
			
	htmlpanel->SetAutoLayout( true );
	htmlpanel->SetSizer( htmlpanelsizer );	
	
	//
	// Search panel
	//	
	searchpanel = new wxPanel(notebook, -1);
	notebook->AddPage(searchpanel, "Search", false, -1);
	
	wxBoxSizer *SearchPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer *SearchControlsSizer = new wxBoxSizer( wxHORIZONTAL );
	SearchPanelSizer->Add(SearchControlsSizer, 0, wxALL|wxEXPAND, 5);
						
	wxStaticText *FindLabel = new wxStaticText(searchpanel, -1, "Text:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "FindLabel");
	SearchControlsSizer->Add(FindLabel, 0, wxALL, 2);

	wxTextCtrl *FindTextCtrl = new wxTextCtrl(searchpanel, -1, "", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxTextCtrlNameStr);
	SearchControlsSizer->Add(FindTextCtrl, 1, wxALL, 2);
	
	wxButton *FindButton = new wxButton(searchpanel, -1, "Search", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "button");
	SearchControlsSizer->Add(FindButton, 0,	wxALL, 2);
			
	wxListCtrl *SearchResultsList = new wxListCtrl(searchpanel, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, "listCtrl");
	SearchPanelSizer->Add(SearchResultsList, 1, wxALL|wxEXPAND, 0);
	
	SearchResultsList->InsertColumn(0, "Block");
	SearchResultsList->InsertColumn(1, "Offset");
	SearchResultsList->InsertColumn(2, "Found");
			
	searchpanel->SetAutoLayout( true );
	searchpanel->SetSizer( SearchPanelSizer );	
	
	panel->SetAutoLayout( TRUE );
	panel->SetSizer( RootSizer );

	CreateStatusBar();
	SetStatusText("Ready.");

	SetIcon(wxICON(scummex_icon));
	
	SetSizeHints(640, 480);
	_gui->readConfigValue("DataPath", &_filepath);
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageDialog *dialog = new wxMessageDialog(this, "\nScummEX 0.1.0cvs (C) 2003 The ScummVM project\n","About", wxOK|wxCENTRE);
	dialog->ShowModal();
}

void MainWindow::OnHelp(wxCommandEvent& WXUNUSED(event)) {
	HtmlHelp->DisplayContents();
}

void MainWindow::OnOpen(wxCommandEvent& WXUNUSED(event)) 
{
	wxFileDialog *dialog = new wxFileDialog(this, "Please select an input file.", _filepath, "",
		"All Supported Files|*|"
		"Main Resource Files|*.001;*.la1;*.la2;*.lab;*.lfl;*.lec;*.sm1|"
		"Directory Files|*.000;*.la0;*.lfl;*.sm0",
		wxOPEN);
	if (dialog->ShowModal() == wxID_OK) {
		_filename = (const char*)dialog->GetPath();
		_file = wxFileName(_filename);
	
		_filepath = _file.GetPath();
		_gui->writeConfigValue("DataPath", _filepath);
		tree->DeleteChildren(iter[0]);
		g_scummex->loadFile(_filename);
	}
}

void MainWindow::OnSelChanged(wxTreeEvent& event) {
	wxTreeItemId itemid;
	int itemtype;
	bool val1, val2;
	itemid = event.GetItem();
	TreeItemData *item = (TreeItemData *)tree->GetItemData(itemid);

	SpecButton1->Show(FALSE);
	SpecButton2->Show(FALSE);
	ToolBar->EnableTool(ID_Dump, TRUE);
	ToolBar->EnableTool(ID_View, TRUE);
	val1 = SpecButton1->Disconnect(ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED);
	val2 = SpecButton2->Disconnect(ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED);

	itemtype = item->_blockType;
	if (itemtype < 200)
		bigIconBitmap = _bigIcon[blocksInfo[itemtype].iconid];
	else
		bigIconBitmap = _bigIcon[oldBlocksInfo[itemtype-200].iconid];

	BigIcon->SetBitmap(bigIconBitmap);

	if (blocksInfo[itemtype].html == 1) {
		if (htmlflag == 0) {
			notebook->AddPage(htmlpanel, "Block Specifications", false, -1);
			notebook->Refresh();
		}
		HtmlWindow->LoadPage(blocksInfo[itemtype].htmlfile);
		htmlflag = 1;
	} else if (htmlflag == 1) {
		notebook->SetSelection(0);
		notebook->RemovePage(notebook->GetPageCount() -1);
		notebook->Refresh();
		htmlflag = 0;
	}
		
	_blockId = item->_blockId;
	updateLabels(_blockId);
	event.Skip();
}

void MainWindow::FileView(wxEvent& event) {
	g_scummex->fileView(_blockId);
}

void MainWindow::FileInfo(wxEvent& event) {
	FileInfoDialog(g_scummex->getInputFileSize(), g_scummex->getEncByte());
}

void MainWindow::FileInfoDialog(int size, int encbyte) {
	char buf[256];
	char msg[512];
	const char *fname;
	const char *fpath;
	
	fname = _file.GetFullName();
	fpath = _file.GetFullPath();
	sprintf(msg, "Filename: \t\t %s\n", fname);
	sprintf(buf, "Full Path: \t\t %s\n", fpath);
	strcat(msg, buf);
	sprintf(buf, "Size: \t\t %d\n", size);
	strcat(msg, buf);
	sprintf(buf, "XOR byte: \t %d (0x%02X)\n", encbyte, encbyte);
	strcat(msg, buf);
	wxMessageBox(msg, "File Info", wxOK, this);
}


void MainWindow::BlockDump(wxEvent& event) {
	wxFileDialog *dialog = new wxFileDialog(this, "Please select an output file.", "", "",
		"*",
		wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		const char *filename = (const char*)dialog->GetPath();
		g_scummex->FileDump(_blockId, filename);
	}
	
}
	
void MainWindow::SaveSOU(wxEvent& event) {
	wxFileDialog *dialog = new wxFileDialog(this, "Please select an output file.", "", "",
		"*",
		wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		const char *filename = (const char*)dialog->GetPath();
		g_scummex->iMUSEDump(_blockId, filename);
	}
	
}
	
void MainWindow::SaveiMUSE(wxEvent& event) {
	wxFileDialog *dialog = new wxFileDialog(this, "Please select an output file.", "", "",
		"*",
		wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		const char *filename = (const char*)dialog->GetPath();
		g_scummex->iMUSEDump(_blockId, filename);
	}
	
}

void MainWindow::Descumm(wxEvent& event) {
	g_scummex->Descumm(_blockId);
}
void MainWindow::iMUSEPlay(wxEvent& event) {
	g_scummex->iMUSEPlay(_blockId);
}
void MainWindow::SOUPlay(wxEvent& event) {
	g_scummex->SOUPlay(_blockId);
}
void MainWindow::paletteDraw(wxEvent& event) {
	g_scummex->paletteDraw(_blockId);
}
void MainWindow::bgDraw(wxEvent& event) {
	g_scummex->bgDraw(_blockId);
}
void MainWindow::SmushFrameDraw(wxEvent& event) {
	g_scummex->SmushFrameDraw(_blockId);
}
void MainWindow::objectDraw(wxEvent& event) {
	g_scummex->objectDraw(_blockId);
}
void MainWindow::boxesDraw(wxEvent& event) {
	g_scummex->boxesDraw(_blockId);
}
