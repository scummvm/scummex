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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/wxwindows.cpp,v 1.25 2003/09/27 14:56:11 yoshizf Exp $
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

BEGIN_EVENT_TABLE(ImageWindow, wxFrame)
	EVT_CLOSE(ImageWindow::OnQuit)
	EVT_MENU(Viewer_Quit, ImageWindow::OnQuit)
	EVT_MENU(ID_BMP, ImageWindow::SaveImage)
	EVT_MENU(ID_Boxes, ImageWindow::boxesDrawOverlay)
END_EVENT_TABLE()

ImageWindow::ImageWindow(const wxString& title, const wxSize& size, int blockId, byte flags)
	: wxFrame(_gui->_mainWindow, -1, title, wxPoint(-1,-1), size, wxDEFAULT_FRAME_STYLE & (wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION))
{
	wxMenuBar *menuBar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;
	_blockId = blockId;
	_boxesDisplayed = 0;
	
	wxMenuItem *BMPItem = new wxMenuItem(menuFile, ID_BMP, "Save to BMP...", "Save image to .BMP", wxITEM_NORMAL, NULL );
	wxBitmap SaveIcon = wxBitmap(save_icon);
	BMPItem->SetBitmap(SaveIcon);
	menuFile->Append(BMPItem);

	menuFile->Append(Viewer_Quit,"Close");
	
	menuBar->Append(menuFile,"&File");

	if (flags & IMAGE_BOXES) {
		wxMenu *menuView = new wxMenu;
		wxMenuItem *BoxesItem = new wxMenuItem(menuView, ID_Boxes, "Display Boxes", "Display Boxes", wxITEM_CHECK, NULL );
		menuView->Append(BoxesItem);
		menuBar->Append(menuView,"View");
	}
	
	SetMenuBar(menuBar);
	
	_scaleFactor = 1;
	_gui->readConfigValue("Scaler", &_scaleFactor);
	SetClientSize(size.GetWidth() * _scaleFactor, size.GetHeight() * _scaleFactor);
	_image = new wxImage(size.GetWidth() * _scaleFactor, size.GetHeight() * _scaleFactor);
	_sbmp = NULL;
}

void ImageWindow::DrawImage() {
	wxBitmap bitmap = wxBitmap(_image);
	
	if (_sbmp == NULL) {
		wxBoxSizer *vertSizer = new wxBoxSizer( wxVERTICAL );

		_sbmp = new wxStaticBitmap(this, -1, bitmap);
		vertSizer->Add(_sbmp, 0, wxALL, 0 );
		Show(TRUE);
	} else {
		_sbmp->SetBitmap(bitmap);
		Refresh();
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

void ImageWindow::UpdateImage() {
	wxBitmap bitmap = wxBitmap(_image);
	_sbmp->SetBitmap(bitmap);
	Refresh();
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
	wxMenuBar *menuBar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;

	menuFile->Append(Viewer_Quit,"Close");
	
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
			//SpecButton1->Enable(FALSE); // FIXME Mixer not working anymore?
			SpecButton2->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::iMUSEPlay );
			Connect( ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::SaveiMUSE );
			break;

		case OBIM:
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
			SpecButton1->SetLabel("View Object");
			SpecButton1->Show(TRUE);
			Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &MainWindow::objectDraw );
			break;

		case Crea:
			SpecButton1->SetLabel("Play");
			SpecButton2->SetLabel("Dump to WAV...");
			SpecButton1->Show(TRUE);
			//SpecButton1->Enable(FALSE); // FIXME Mixer not working anymore?
			SpecButton2->Show(TRUE);
			//SpecButton2->Enable(FALSE);
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

void MainWindow::updateLabel(wxStaticText *label, const char *title, uint32 text) {
	char buf[256];
	sprintf(buf, "%s:    \t\t %d", title, text);
	label->SetLabel(buf);
}

void MainWindow::updateLabels(int blockid) {
	const BlockTable &block = g_scummex->getBlockTable(blockid);
	char buf[256];

	// Clear out all labels
	for (int i = 0; i < 6; i++)
		SpecLabel[i]->SetLabel("");

	sprintf(buf, "Type: \t\t %s", block.blockType);
	TypeLabel->SetLabel(buf);

	sprintf(buf, "Offset: \t\t %d (0x%08X)", block.offset, block.offset);
	OffsetLabel->SetLabel(buf);

	sprintf(buf, "Size: \t\t %d (0x%08X)", block.blockSize, block.blockSize);
	SizeLabel->SetLabel(buf);

	sprintf(buf, "Description: \t %s", block.blockDescription);
	DescriptionLabel->SetLabel(buf);

	switch(block.blockTypeID) {
		case MAXS:
			updateLabel(SpecLabel[0], "Number of variables", block.variables);
			updateLabel(SpecLabel[1], "Number of bit variables", block.bitVariables);
			updateLabel(SpecLabel[2], "Number of local objects", block.localObjects);
			updateLabel(SpecLabel[3], "Number of arrays", block.arrays);
			updateLabel(SpecLabel[4], "Number of character sets", block.characters);
			updateLabel(SpecLabel[5], "Number of inventory objects", block.invObjects);
			break;

		case LB83:
		case LABN:
			updateLabel(SpecLabel[0], "Number of files", block.numFiles);
			break;

		case RMHD:
		case HD:
			updateLabel(SpecLabel[0], "Room Width", block.width);
			updateLabel(SpecLabel[1], "Room Height", block.height);
			updateLabel(SpecLabel[2], "Number of Objects", block.localObjects);
			break;

		case TRNS:
			updateLabel(SpecLabel[0], "Transparent Color", block.trans);
			break;
			
		case DROO:
		case DSCR:
		case DSOU:
		case DCOS:
		case DCHR:
		case DOBJ:
			updateLabel(SpecLabel[0], "Number of items", block.numFiles);
			break;

		case cus2:
			SetButton(block.blockTypeID);
			break;
			
		case COMP:
			updateLabel(SpecLabel[0], "Number of samples", block.numFiles);
			break;

		case CLUT:
		case APAL:
		case PA:
		case NPAL:
		case AHDR:
		case RGBS:
			SetButton(block.blockTypeID);
			break;

		case RMIM:
		case BM:
		case IMAG:
			SetButton(block.blockTypeID);
			break;

		case OBIM:
			if (g_scummex->getBlockTable(blockid+3).blockTypeID == 35) {
				SetButton(block.blockTypeID);
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
			SetButton(block.blockTypeID);
			break;

		case IMHD:
			updateLabel(SpecLabel[0], "Room Width", block.width);
			updateLabel(SpecLabel[1], "Room Height", block.height);
			updateLabel(SpecLabel[2], "Number of Images", block.numFiles);
			break;
			
		case Crea:
			updateLabel(SpecLabel[0], "Sample Rate", block.variables);
			SetButton(block.blockTypeID);
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
			SetButton(block.blockTypeID);
			break;

		case FOBJ:
			updateLabel(SpecLabel[0], "Frame Width", block.width);
			updateLabel(SpecLabel[1], "Frame Height", block.height);
			updateLabel(SpecLabel[2], "Codec", block.variables);
			SetButton(block.blockTypeID);
			break;

		case BOXD:
		case BX:
			updateLabel(SpecLabel[0], "No. of Boxes", block.numFiles);
			SetButton(block.blockTypeID);
			break;
	}
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
	
	wxMenuBar *menuBar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;
	
	wxMenuItem *OpenItem = new wxMenuItem(menuFile, wxID_OPEN, "Open...", "Open resource file", wxITEM_NORMAL, NULL );
	OpenItem->SetBitmap(OpenIcon);
	menuFile->Append(OpenItem);
	
	menuFile->AppendSeparator();
	
	wxMenuItem *QuitItem = new wxMenuItem(menuFile, wxID_EXIT, "Exit", "Exit ScummEX", wxITEM_NORMAL, NULL );
	QuitItem->SetBitmap(ExitIcon);
	menuFile->Append(QuitItem);

	menuBar->Append(menuFile,"&File");

	wxMenu *menuTools = new wxMenu;
	wxMenuItem *DumpItem = new wxMenuItem(menuTools, ID_Dump, "File Dump...", "Dump block to disk", wxITEM_NORMAL, NULL );
	DumpItem->SetBitmap(SaveIcon);
	menuTools->Append(DumpItem);
	menuTools->Append(ID_SoundStop, "Stop Sounds");
	menuBar->Append(menuTools, "&Tools");

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_HELP, "Contents");
	menuHelp->Append(wxID_ABOUT,"About...");
	menuBar->Append(menuHelp, "&Help");
	
	SetMenuBar(menuBar);

	SetThemeEnabled(true);
	
	ToolBar = CreateToolBar(wxTB_FLAT, -1, "toolBar");
	
	ToolBar->SetToolBitmapSize(wxSize(16, 16));
	
	ToolBar->AddTool(wxID_OPEN, "Open...", OpenIcon, wxNullBitmap, wxITEM_NORMAL, "Open...", "Open resource file", NULL);
	ToolBar->AddTool(wxID_CLOSE, "Close", CloseIcon, wxNullBitmap, wxITEM_NORMAL, "Close", "Close resource file", NULL);
	ToolBar->EnableTool(wxID_CLOSE, FALSE);
	ToolBar->AddTool(ID_FileInfo, "File Info", FileIcon, wxNullBitmap, wxITEM_NORMAL, "File Info", "Show File Info", NULL);
	ToolBar->EnableTool(ID_FileInfo, FALSE);
	ToolBar->AddTool(wxID_EXIT, "Exit", ExitIcon, wxNullBitmap, wxITEM_NORMAL, "Exit", "Exit ScummEX", NULL);
	ToolBar->AddSeparator();
	ToolBar->AddTool(ID_Dump, "File Dump", SaveIcon, wxNullBitmap, wxITEM_NORMAL, "File Dump", "Dump block to disk", NULL);
	ToolBar->EnableTool(ID_Dump, FALSE);
	ToolBar->AddTool(ID_View, "Hex Viewer...", HexIcon, wxNullBitmap, wxITEM_NORMAL, "Hex Viewer...", "View block as hex", NULL);
	ToolBar->EnableTool(ID_View, FALSE);
	ToolBar->AddTool(Button_Options, "Options...", OptionsIcon, wxNullBitmap, wxITEM_NORMAL, "Options...", "Options", NULL);
	ToolBar->EnableTool(Button_Options, FALSE);
	ToolBar->AddSeparator();
	ToolBar->AddTool(wxID_HELP, "Help", HelpIcon, wxNullBitmap, wxITEM_NORMAL, "Help", "View help", NULL);
	ToolBar->Realize();
	
	panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "panel");
	
	wxBoxSizer *RootSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer *BlocksSizer = new wxBoxSizer( wxHORIZONTAL );

	RootSizer->Add(BlocksSizer,
			1,
			wxALL|wxEXPAND,
			0
			);

	tree = new wxTreeCtrl(panel, ID_Tree, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT, wxDefaultValidator, "listCtrl");
	
	wxImageList *ImageList = new wxImageList(16, 16, true, 61);

	wxIcon icon0 = wxIcon(xpm_0); ImageList->Add(icon0);
	wxIcon icon1 = wxIcon(xpm_1); ImageList->Add(icon1);
	wxIcon icon2 = wxIcon(xpm_2); ImageList->Add(icon2);
	wxIcon icon3 = wxIcon(xpm_3); ImageList->Add(icon3);
	wxIcon icon4 = wxIcon(xpm_4); ImageList->Add(icon4);
	wxIcon icon5 = wxIcon(xpm_5); ImageList->Add(icon5);
	wxIcon icon6 = wxIcon(xpm_6); ImageList->Add(icon6);
	wxIcon icon7 = wxIcon(xpm_7); ImageList->Add(icon7);
	wxIcon icon8 = wxIcon(xpm_8); ImageList->Add(icon8);
	wxIcon icon9 = wxIcon(xpm_9); ImageList->Add(icon9);
	wxIcon icon10 = wxIcon(xpm_10); ImageList->Add(icon10);
	wxIcon icon11 = wxIcon(xpm_11); ImageList->Add(icon11);
	wxIcon icon12 = wxIcon(xpm_12); ImageList->Add(icon12);
	wxIcon icon13 = wxIcon(xpm_13); ImageList->Add(icon13);
	wxIcon icon14 = wxIcon(xpm_14); ImageList->Add(icon14);
	wxIcon icon15 = wxIcon(xpm_15); ImageList->Add(icon15);
	wxIcon icon16 = wxIcon(xpm_16); ImageList->Add(icon16);
	wxIcon icon17 = wxIcon(xpm_17); ImageList->Add(icon17);
	wxIcon icon18 = wxIcon(xpm_18); ImageList->Add(icon18);
	wxIcon icon19 = wxIcon(xpm_19); ImageList->Add(icon19);
	wxIcon icon20 = wxIcon(xpm_20); ImageList->Add(icon20);
	wxIcon icon21 = wxIcon(xpm_21); ImageList->Add(icon21);
	wxIcon icon22 = wxIcon(xpm_22); ImageList->Add(icon22);
	wxIcon icon23 = wxIcon(xpm_23); ImageList->Add(icon23);
	wxIcon icon24 = wxIcon(xpm_24); ImageList->Add(icon24);
	wxIcon icon25 = wxIcon(xpm_25); ImageList->Add(icon25);
	wxIcon icon26 = wxIcon(xpm_26); ImageList->Add(icon26);
	wxIcon icon27 = wxIcon(xpm_27); ImageList->Add(icon27);
	wxIcon icon28 = wxIcon(xpm_28); ImageList->Add(icon28);
	wxIcon icon29 = wxIcon(xpm_29); ImageList->Add(icon29);
	wxIcon icon30 = wxIcon(xpm_30); ImageList->Add(icon30);
	wxIcon icon31 = wxIcon(xpm_31); ImageList->Add(icon31);
	wxIcon icon32 = wxIcon(xpm_32); ImageList->Add(icon32);
	wxIcon icon33 = wxIcon(xpm_33); ImageList->Add(icon33);
	wxIcon icon34 = wxIcon(xpm_34); ImageList->Add(icon34);
	wxIcon icon35 = wxIcon(xpm_35); ImageList->Add(icon35);
	wxIcon icon36 = wxIcon(xpm_36); ImageList->Add(icon36);
	wxIcon icon37 = wxIcon(xpm_37); ImageList->Add(icon37);
	wxIcon icon38 = wxIcon(xpm_38); ImageList->Add(icon38);
	wxIcon icon39 = wxIcon(xpm_39); ImageList->Add(icon39);
	wxIcon icon40 = wxIcon(xpm_40); ImageList->Add(icon40);
	wxIcon icon41 = wxIcon(xpm_41); ImageList->Add(icon41);
	wxIcon icon42 = wxIcon(xpm_42); ImageList->Add(icon42);
	wxIcon icon43 = wxIcon(xpm_43); ImageList->Add(icon43);
	wxIcon icon44 = wxIcon(xpm_44); ImageList->Add(icon44);
	wxIcon icon45 = wxIcon(xpm_45); ImageList->Add(icon45);
	wxIcon icon46 = wxIcon(xpm_46); ImageList->Add(icon46);
	wxIcon icon47 = wxIcon(xpm_47); ImageList->Add(icon47);
	wxIcon icon48 = wxIcon(xpm_48); ImageList->Add(icon48);
	wxIcon icon49 = wxIcon(xpm_49); ImageList->Add(icon49);
	wxIcon icon50 = wxIcon(xpm_50); ImageList->Add(icon50);
	wxIcon icon51 = wxIcon(xpm_51); ImageList->Add(icon51);
	wxIcon icon52 = wxIcon(xpm_52); ImageList->Add(icon52);
	wxIcon icon53 = wxIcon(xpm_53); ImageList->Add(icon53);
	wxIcon icon54 = wxIcon(xpm_54); ImageList->Add(icon54);
	wxIcon icon55 = wxIcon(xpm_55); ImageList->Add(icon55);
	wxIcon icon56 = wxIcon(xpm_56); ImageList->Add(icon56);
	wxIcon icon57 = wxIcon(xpm_57); ImageList->Add(icon57);
	wxIcon icon58 = wxIcon(xpm_58); ImageList->Add(icon58);
	wxIcon icon59 = wxIcon(xpm_59); ImageList->Add(icon59);
	wxIcon icon60 = wxIcon(xpm_60); ImageList->Add(icon60);

	tree->AssignImageList(ImageList);
	
		
	BlocksSizer->Add(tree,
			1,
			wxALL|wxEXPAND,
			4
			);

	iter[0] = tree->AddRoot("", -1, -1, NULL);
	
	wxBoxSizer *InfosSizer = new wxBoxSizer( wxVERTICAL );

	BlocksSizer->Add(InfosSizer,
			1,
			wxALL|wxEXPAND,
			5
			);

	notebook = new wxNotebook(panel, -1, wxPoint(-1, -1), wxSize(-1, -1), 0, "notebook");
	wxNotebookSizer *nbs = new wxNotebookSizer( notebook );
	
	InfosSizer->Add(nbs,
			1,
			wxALL|wxEXPAND,
			0
			);

	infospanel = new wxPanel(notebook, -1, wxPoint(-1, -1), wxSize(-1, -1), 0, "infopanel");
	
	notebook->AddPage(infospanel, "Explorer", true, -1);
	
	wxBoxSizer *panelsizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBox *GenInfosBox = new wxStaticBox(infospanel, -1, "General Block Infos", wxDefaultPosition, wxDefaultSize, 0, "staticBox");
	
	wxStaticBoxSizer *GenInfosHSizer = new wxStaticBoxSizer( GenInfosBox, wxHORIZONTAL );

	panelsizer->Add(GenInfosHSizer,
			1,
			wxALL|wxEXPAND,
			0
			);
				
	wxBoxSizer *GenInfosSizer = new wxBoxSizer( wxVERTICAL );
	
	GenInfosHSizer->Add(GenInfosSizer,
			1,
			wxALL|wxEXPAND,
			0
			);

	TypeLabel = new wxStaticText(infospanel, -1, "Type:  \t\t n/a", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "TypeLabel");
	OffsetLabel = new wxStaticText(infospanel, -1, "Offset:  \t\t n/a", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "TypeLabel");
	SizeLabel = new wxStaticText(infospanel, -1, "Size:  \t\t n/a", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "TypeLabel");
	DescriptionLabel = new wxStaticText(infospanel, -1, "Description:  \t n/a", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "TypeLabel");
	
	GenInfosSizer->Add(TypeLabel,
			0,
			wxALL|wxEXPAND,
			1
			);
					
	GenInfosSizer->Add(OffsetLabel,
			0,
			wxALL|wxEXPAND,
			1
			);
			
	GenInfosSizer->Add(SizeLabel,
			0,
			wxALL|wxEXPAND,
			1
			);
			
	GenInfosSizer->Add(DescriptionLabel,
			0,
			wxALL|wxEXPAND,
			1
			);

	wxBoxSizer *IconSizer = new wxBoxSizer( wxVERTICAL );
	
	GenInfosHSizer->Add(IconSizer,
			0,
			wxALL,
			0
			);

	bigIconBitmap = wxBitmap(xpm_0_big);
	
	BigIcon = new wxStaticBitmap(infospanel, -1, bigIconBitmap, wxPoint(-1, -1), wxSize(-1, -1), wxALIGN_RIGHT);

	IconSizer->Add(BigIcon,
			0,
			wxALL,
			0
			);
							
	wxStaticBox *SpecInfosBox = new wxStaticBox(infospanel, -1, "Specific Block Infos", wxDefaultPosition, wxDefaultSize, 0, "staticBox");
	
	wxStaticBoxSizer *SpecInfosSizer = new wxStaticBoxSizer( SpecInfosBox, wxVERTICAL );

	panelsizer->Add(SpecInfosSizer,
			1,
			wxALL|wxEXPAND,
			0
			);
	
	SpecLabel[0] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	SpecLabel[1] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	SpecLabel[2] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	SpecLabel[3] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	SpecLabel[4] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	SpecLabel[5] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	
	for (int i = 0; i < 6; i++) {
		SpecInfosSizer->Add(SpecLabel[i],
				0,
				wxALL|wxEXPAND,
				1
				);
	}

	HtmlHelp = new wxHtmlHelpController(wxHF_DEFAULT_STYLE & wxHF_CONTENTS);

	HtmlHelp->AddBook(wxFileName("help/help.hhp"));

	wxBoxSizer *SpecButtonsSizer = new wxBoxSizer ( wxHORIZONTAL );

	SpecInfosSizer->Add(SpecButtonsSizer,
			0,
			wxALL,
			0
			);
	
	SpecButton1 = new wxButton( infospanel, ID_SpecButton1, "", wxPoint(-1, -1), wxSize(110, -1));
	SpecButton2 = new wxButton( infospanel, ID_SpecButton2, "", wxPoint(-1, -1), wxSize(110, -1));

	SpecButton1->Show(FALSE);
	SpecButton2->Show(FALSE);

	SpecButtonsSizer->Add( SpecButton1,
			1,
			wxALL,
			3
			);

	SpecButtonsSizer->Add( SpecButton2,
			1,
			wxALL,
			3
			);

	infospanel->SetAutoLayout( true );
	infospanel->SetSizer( panelsizer );

	htmlpanel = new wxPanel(notebook, -1, wxPoint(-1, -1), wxSize(-1, -1), 0, "htmlpanel");
	
	wxBoxSizer *htmlpanelsizer = new wxBoxSizer( wxVERTICAL );

	HtmlWindow = new wxHtmlWindow(htmlpanel, -1, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO, "htmlWindow");
	
	htmlpanelsizer->Add(HtmlWindow,
			1,
			wxALL|wxEXPAND,
			0
			);
			
	htmlpanel->SetAutoLayout( true );
	htmlpanel->SetSizer( htmlpanelsizer );	

	searchpanel = new wxPanel(notebook, -1, wxPoint(-1, -1), wxSize(-1, -1), 0, "searchpanel");
	notebook->AddPage(searchpanel, "Search", false, -1);
	
	wxBoxSizer *SearchPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer *SearchControlsSizer = new wxBoxSizer( wxHORIZONTAL );

	SearchPanelSizer->Add(SearchControlsSizer,
			0,
			wxALL|wxEXPAND,
			5
			);
						
	wxStaticText *FindLabel = new wxStaticText(searchpanel, -1, "Text:", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "FindLabel");
	wxTextCtrl *FindTextCtrl = new wxTextCtrl(searchpanel, -1, "", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxTextCtrlNameStr);
	wxButton *FindButton = new wxButton(searchpanel, -1, "Search", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "button");
	
	SearchControlsSizer->Add(FindLabel,
			0,
			wxALL,
			2
			);

	SearchControlsSizer->Add(FindTextCtrl,
			1,
			wxALL,
			2
			);
			
	SearchControlsSizer->Add(FindButton,
			0,
			wxALL,
			2
			);
			
	wxListCtrl *SearchResultsList = new wxListCtrl(searchpanel, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, "listCtrl");
	SearchResultsList->InsertColumn(0, "Block");
	SearchResultsList->InsertColumn(1, "Offset");
	SearchResultsList->InsertColumn(2, "Found");

	SearchPanelSizer->Add(SearchResultsList,
			1,
			wxALL|wxEXPAND,
			0
			);
			
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
	switch (itemtype) {
		case LECF:
		case LABN:
		case LB83:
		case LE:
			bigIconBitmap = wxBitmap(xpm_6_big);
			break;
			
		case LOFF:
		case OFFS:
		case FO:
			bigIconBitmap = wxBitmap(xpm_44_big);
			break;
			
		case LFLF:
		case LF:
			bigIconBitmap = wxBitmap(xpm_58_big);
			break;
			
		case ROOM:
		case RO:
			bigIconBitmap = wxBitmap(xpm_38_big);
			break;
			
		case RMHD:
		case HD:
			bigIconBitmap = wxBitmap(xpm_35_big);
			break;
			
		case CYCL:
		case CC:
			bigIconBitmap = wxBitmap(xpm_12_big);
			break;
			
		case TRNS:
			bigIconBitmap = wxBitmap(xpm_46_big);
			break;	
			
		case EPAL:
		case CLUT:
		case APAL:
		case NPAL:
		case XPAL:
		case PA:
			bigIconBitmap = wxBitmap(xpm_34_big);
			break;
			
		case BOXD:
		case BX:
			bigIconBitmap = wxBitmap(xpm_4_big);
			break;
			
		case BOXM:
			bigIconBitmap = wxBitmap(xpm_5_big);
			break;
			
		case SCAL:
		case SA:
			bigIconBitmap = wxBitmap(xpm_40_big);
			break;
			
		case RMIM:
		case BM:
			bigIconBitmap = wxBitmap(xpm_37_big);
			break;
			
		case EXCD:
		case EX:
			bigIconBitmap = wxBitmap(xpm_22_big);
			break;

		case ENCD:
		case EN:
			bigIconBitmap = wxBitmap(xpm_21_big);
			break;
			
		case NLSC:
		case SCRP:
		case LSCR:
		case OC:
		case NL:
		case LS:
		case SC:
			bigIconBitmap = wxBitmap(xpm_41_big);
			break;
			
		case SOUN:
		case SOU:
		case iMUS:
		case cus2:
		case IMC:
		case SO:
			bigIconBitmap = wxBitmap(xpm_59_big);
			break;
			
		case RMIH:
			bigIconBitmap = wxBitmap(xpm_36_big);
			break;

		case IM00:
		case IM01:
		case IM02:
		case IM03:
		case IM04:
		case IM05:
		case IM06:
		case IM07:
		case IM08:
		case IM09:
		case IMAG:
			bigIconBitmap = wxBitmap(xpm_25_big);
			break;
			
		case SMAP:
		case TBSTR:
			bigIconBitmap = wxBitmap(xpm_42_big);
			break;
			
		case OBIM:
		case OI:
			bigIconBitmap = wxBitmap(xpm_33_big);
			break;
			
		case IMHD:
			bigIconBitmap = wxBitmap(xpm_26_big);
			break;
			
		case OBCD:
		case RMSC:
			bigIconBitmap = wxBitmap(xpm_57_big);
			break;
			
		case CDHD:
			bigIconBitmap = wxBitmap(xpm_7_big);
			break;
			
		case VERB:
			bigIconBitmap = wxBitmap(xpm_50_big);
			break;
			
		case OBNA:
		case TEXT:
			bigIconBitmap = wxBitmap(xpm_45_big);
			break;
						
		case VCTL:
			bigIconBitmap = wxBitmap(xpm_15_big);
			break;
			
		case Crea:
			bigIconBitmap = wxBitmap(xpm_16_big);
			break;
			
		case MAXS:
			bigIconBitmap = wxBitmap(xpm_2_big);
			break;
			
		case DROO:
			bigIconBitmap = wxBitmap(xpm_18_big);
			break;
			
		case DSCR:
			bigIconBitmap = wxBitmap(xpm_19_big);
			break;
			
		case DSOU:
			bigIconBitmap = wxBitmap(xpm_20_big);
			break;
			
		case DCOS:
			bigIconBitmap = wxBitmap(xpm_14_big);
			break;
			
		case DCHR:
			bigIconBitmap = wxBitmap(xpm_13_big);
			break;
			
		case DOBJ:
			bigIconBitmap = wxBitmap(xpm_17_big);
			break;
			
		case COMP:
		case MCMP:
			bigIconBitmap = wxBitmap(xpm_9_big);
			break;
			
		case MAP:
			bigIconBitmap = wxBitmap(xpm_29_big);
			break;
			
		case FRMT:
			bigIconBitmap = wxBitmap(xpm_51_big);
			break;
			
		case SYNC:
			bigIconBitmap = wxBitmap(xpm_43_big);
			break;
			
		case REGN:
		case STOP:
		case JUMP:
		
			bigIconBitmap = wxBitmap(xpm_27_big);
			break;
			
		case DATA:
			bigIconBitmap = wxBitmap(xpm_16_big);
			break;
			
		case WRAP:
			bigIconBitmap = wxBitmap(xpm_60_big);
			break;
			
		case ZP01:
		case ZP02:
		case ZP03:
		case ZP04:
		case ZPLN:
			bigIconBitmap = wxBitmap(xpm_52_big);
			break;
			
		case COST:
		case CO:
			bigIconBitmap = wxBitmap(xpm_11_big);
			break;
		
		case PALS:
			bigIconBitmap = wxBitmap(xpm_56_big);
			break;

		case ANIM:
		case FRME:
			bigIconBitmap = wxBitmap(xpm_55_big);
			break;

		case FOBJ:
			bigIconBitmap = wxBitmap(xpm_32_big);
			break;

		case AHDR:
			bigIconBitmap = wxBitmap(xpm_1_big);
			break;

		case PSAD:
		case SAUD:
		case STRK:
		case SDAT:
		case IACT:
			bigIconBitmap = wxBitmap(xpm_24_big);
			break;

		case ADL:
		case SPK:
		case ROL:
		case MIDI:
			bigIconBitmap = wxBitmap(xpm_31_big);
			break;

		default:
			bigIconBitmap = wxBitmap(xpm_47_big);
			break;
	}

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
		
	BigIcon->SetBitmap(bigIconBitmap);
	
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
