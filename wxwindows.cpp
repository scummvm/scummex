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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/wxwindows.cpp,v 1.4 2003/09/19 01:36:40 fingolfin Exp $
 *
 */

#include "wxwindows.h"
#include "file.h"
#include "scummex.h"
#include "icons.h"

wxTreeItemId iter[9], itemid;
wxTreeCtrl *tree;
wxStaticText *TypeLabel, *OffsetLabel, *SizeLabel, *DescriptionLabel;
wxStaticText *SpecLabel[6];
wxButton *SpecButton1, *SpecButton2;
MainWindow *frame;
ImageWindow *imageFrame;
ScummEX *_scummex;
wxImage *image;
const char *file, *filename;
wxTextCtrl *hexdata;
wxToolBar *ToolBar;

IMPLEMENT_APP(GUI_wxWindows)

GUI_wxWindows::GUI_wxWindows() {
	_scummex = new ScummEX();
}

bool GUI_wxWindows::OnInit()
{
	frame = new MainWindow("ScummEX", wxPoint(-1,-1),
                wxSize(640,480));

	frame->Connect( ID_Quit, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &MainWindow::OnQuit );
	frame->Connect( ID_About, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &MainWindow::OnAbout );
	frame->Connect( ID_Open, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &MainWindow::OnOpen );
	frame->Connect( ID_Browse, wxEVT_COMMAND_BUTTON_CLICKED,
		(wxObjectEventFunction) &MainWindow::OnOpen );
	frame->Connect( ID_View, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &ScummEX::fileView );
	frame->Connect( ID_Dump, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &GUI_wxWindows::BlockDump );
	frame->Connect( Tree, wxEVT_COMMAND_TREE_SEL_CHANGING,
		(wxObjectEventFunction) &MainWindow::OnSelChanged );
	frame->Connect( ID_Help, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &MainWindow::OnHelp );
	frame->Connect( ID_FileInfo, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &ScummEX::FileInfo );
		
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}

void GUI_wxWindows::SetTitle(char *title) {
	frame->SetTitle(title);
}

void MainWindow::OnHelp(wxCommandEvent& WXUNUSED(event)) {
	HtmlHelp->DisplayContents();
}

void GUI_wxWindows::FileInfoDialog(int size, int encbyte) {
	char buf[256];
	char msg[512];
	const char *fname;
	const char *fpath;
	wxFileName *filename = new wxFileName(file);
	
	fname = filename->GetFullName();
	fpath = filename->GetFullPath();
	sprintf(msg, "Filename: \t\t %s\n", fname);
	sprintf(buf, "Full Path: \t\t %s\n", fpath);
	strcat(msg, buf);
	sprintf(buf, "Size: \t\t %d\n", size);
	strcat(msg, buf);
	sprintf(buf, "XOR byte: \t %d (0x%02X)\n", encbyte, encbyte);
	strcat(msg, buf);
	wxMessageBox(msg, "File Info", wxOK, frame);
}

void GUI_wxWindows::BlockDump() {
    wxFileDialog *dialog = new wxFileDialog(frame, "Please select an output file.", "", "",
        "*",
        wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		filename = (const char*)dialog->GetPath();
		_scummex->FileDump(filename);
	}

}

void GUI_wxWindows::SaveSOU() {
    wxFileDialog *dialog = new wxFileDialog(frame, "Please select an output file.", "", "",
        "*",
        wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		filename = (const char*)dialog->GetPath();
		_scummex->iMUSEDump(filename);
	}

}

void GUI_wxWindows::SaveiMUSE() {
    wxFileDialog *dialog = new wxFileDialog(frame, "Please select an output file.", "", "",
        "*",
        wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		filename = (const char*)dialog->GetPath();
		_scummex->iMUSEDump(filename);
	}

}

void GUI_wxWindows::PutPixel(int x, int y, int red, int green, int blue) {
	image->SetRGB(x, y, (unsigned char) red, (unsigned char) green, (unsigned char) blue);
}

void GUI_wxWindows::SaveImage() {
    wxFileDialog *dialog = new wxFileDialog(imageFrame, "Please select an output file.", "", "",
        "*",
        wxSAVE);
	if (dialog->ShowModal() == wxID_OK) {
		filename = (const char*)dialog->GetPath();
		image->SaveFile(filename, wxBITMAP_TYPE_BMP);
	}
}

void GUI_wxWindows::DisplayImage(char* title, int width, int height) {
	imageFrame = new ImageWindow(title, wxPoint(-1,-1), wxSize(width, height));

	imageFrame->Connect( Viewer_Quit, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &ImageWindow::OnQuit );
	imageFrame->Connect( ID_BMP, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &GUI_wxWindows::SaveImage );
	imageFrame->Connect( ID_ImageWindow, wxEVT_CLOSE_WINDOW,
		(wxObjectEventFunction) &ImageWindow::OnQuit );
}

void GUI_wxWindows::DrawImage() {
	imageFrame->DrawImage();
}

ImageWindow::ImageWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(frame,ID_ImageWindow,title,pos,size, wxDEFAULT_FRAME_STYLE & (wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION))
{
	wxMenuBar *menuBar = new wxMenuBar;
	wxMenu *menuFile = new wxMenu;
	
	wxMenuItem *BMPItem = new wxMenuItem(menuFile, ID_BMP, "Save to BMP...", "Save image to .BMP", wxITEM_NORMAL, NULL );
	wxBitmap SaveIcon = wxBitmap(save_icon);
	BMPItem->SetBitmap(SaveIcon);
	menuFile->Append(BMPItem);

	menuFile->Append(Viewer_Quit,"Close");
	
	menuBar->Append(menuFile,"&File");
	SetMenuBar(menuBar);
	SetClientSize(size.GetWidth(), size.GetHeight());
	image = new wxImage(size.GetWidth(), size.GetHeight());
}

void ImageWindow::DrawImage() {
	wxBitmap bitmap = wxBitmap(image);
		
	wxBoxSizer *vertSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBitmap *sbmp = new wxStaticBitmap(this, -1, bitmap);
	
	vertSizer->Add(sbmp, 0, wxALL, 0 );
	
	Show(TRUE);
}

void ImageWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	delete image;
	Destroy();
}

void GUI_wxWindows::DisplayViewer(char *title, int width, int height, char *text) {

	ViewerWindow *viewerFrame = new ViewerWindow(title, text, wxPoint(50,50), wxSize(width, height));

	viewerFrame->Connect( Viewer_Quit, wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction) &ViewerWindow::OnQuit );
	viewerFrame->Connect( ID_ViewerWindow, wxEVT_CLOSE_WINDOW,
		(wxObjectEventFunction) &ViewerWindow::OnQuit );
}

void GUI_wxWindows::AppendText(char *text) {
	hexdata->AppendText(text);
}

ViewerWindow::ViewerWindow(const wxString& title, const wxString& text, const wxPoint& pos, const wxSize& size)
	: wxFrame(frame, ID_ViewerWindow, title, pos, size)
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
	wxMessageBox(message, title, wxOK, frame);
}

void GUI_wxWindows::EnableToolbarTool(int tool) {
	ToolBar->EnableTool(tool, TRUE);
}

void GUI_wxWindows::DisableToolbarTool(int tool) {
	ToolBar->EnableTool(tool, FALSE);
}

void GUI_wxWindows::SetButton(int blocktype) {
	switch(blocktype) {
		case 25: // iMUSE
			SpecButton1->SetLabel("Play");
			SpecButton2->SetLabel("Decompress to WAV...");
			SpecButton1->Show(TRUE);
			SpecButton1->Enable(FALSE); // FIXME Mixer not working anymore?
			SpecButton2->Show(TRUE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::iMUSEPlay );
			SpecButton2->Connect( ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &GUI_wxWindows::SaveiMUSE );
			break;

		case OBIM:
			SpecButton1->SetLabel("View Object");
			SpecButton1->Show(TRUE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::objectDraw );
			break;

		case 48: // SOU
			SpecButton1->SetLabel("Play");
			SpecButton2->SetLabel("Decompress to WAV...");
			SpecButton1->Show(TRUE);
			SpecButton1->Enable(FALSE); // FIXME Mixer not working anymore?
			SpecButton2->Show(TRUE);
			SpecButton2->Enable(FALSE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::SOUPlay );
			SpecButton2->Connect( ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &GUI_wxWindows::SaveSOU );
			break;
			

		case CLUT:
		case APAL:
		case 208: // Palettes
		case NPAL:
			SpecButton1->SetLabel("View Palette");
			SpecButton1->Show(TRUE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::paletteDraw );
			break;
		
		case 209:
		case RMIM:
			SpecButton1->SetLabel("View Image");
			SpecButton1->Show(TRUE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::bgDraw );
			break;

		case LSCR:
		case SCRP:
		case ENCD:
		case EXCD:
		case VERB:
			SpecButton1->SetLabel("Decompile Script");
			SpecButton1->Show(TRUE);
			SpecButton1->Connect( ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED,
				(wxObjectEventFunction) &ScummEX::Descumm );
			break;

	}

}

void GUI_wxWindows::add_tree_elements(char *itemName, int blockid, int level, int type) {

	assert(level <= 8);
	itemid = iter[level] = tree->AppendItem(iter[level-1], itemName, -1, -1, new TreeItemData(blockid, type));

	if (blocksInfo[type].iconid != 0) {
		tree->SetItemImage(itemid, blocksInfo[type].iconid);
		tree->SetItemImage(itemid, blocksInfo[type].iconid, wxTreeItemIcon_Selected);
	} else {
		tree->SetItemImage(itemid, 47);
		tree->SetItemImage(itemid, 47, wxTreeItemIcon_Selected);
	}
	
	if (blockid == 1) {
		tree->EnsureVisible(itemid);
	}
}

void GUI_wxWindows::updateLabel(char *label, char *title, uint32 text) {
	char buf[256];
	if (strstr(label, "TypeLabel")) {
		sprintf(buf, "%s: \t\t %s", title, (char*)text);
		TypeLabel->SetLabel(buf);
	}
	if (strstr(label, "OffsetLabel")) {
		sprintf(buf, "%s: \t\t %d (0x%08X)", title, text, text);
		OffsetLabel->SetLabel(buf);
	}
	if (strstr(label, "DescriptionLabel")) {
		sprintf(buf, "%s: \t %s", title, (char*)text);
		DescriptionLabel->SetLabel(buf);
	}
	if (strstr(label, "SizeLabel")) {
		sprintf(buf, "%s: \t\t %d (0x%08X)", title, text, text);
		SizeLabel->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel1")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[0]->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel2")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[1]->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel3")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[2]->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel4")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[3]->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel5")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[4]->SetLabel(buf);
	}
	if (strstr(label, "SpecLabel6")) {
		sprintf(buf, "%s:    \t\t %d", title, text);
		SpecLabel[5]->SetLabel(buf);
	}

}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame*)NULL,-1,title,pos,size)
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
	
	wxMenuItem *OpenItem = new wxMenuItem(menuFile, ID_Open, "Open", "Open resource file", wxITEM_NORMAL, NULL );
	OpenItem->SetBitmap(OpenIcon);
	menuFile->Append(OpenItem);
	
	menuFile->AppendSeparator();
	
	wxMenuItem *QuitItem = new wxMenuItem(menuFile, ID_Quit, "Exit", "Exit ScummEX", wxITEM_NORMAL, NULL );
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
	menuHelp->Append(ID_Help, "Contents");
	menuHelp->Append(ID_About,"About...");
	menuBar->Append(menuHelp, "&Help");
	
	SetMenuBar(menuBar);

	SetThemeEnabled(true);
	
	ToolBar = CreateToolBar(wxTB_FLAT, -1, "toolBar");
	
	ToolBar->SetToolBitmapSize(wxSize(16, 16));
	
	ToolBar->AddTool(ID_Open, "Open", OpenIcon, wxNullBitmap, wxITEM_NORMAL, "Open", "Open resource file", NULL);
	ToolBar->AddTool(ID_Close, "Close", CloseIcon, wxNullBitmap, wxITEM_NORMAL, "Close", "Close resource file", NULL);
	ToolBar->EnableTool(ID_Close, FALSE);
	ToolBar->AddTool(ID_FileInfo, "File Info", FileIcon, wxNullBitmap, wxITEM_NORMAL, "File Info", "Show File Info", NULL);
	ToolBar->EnableTool(ID_FileInfo, FALSE);
	ToolBar->AddTool(ID_Quit, "Exit", ExitIcon, wxNullBitmap, wxITEM_NORMAL, "Exit", "Exit ScummEX", NULL);
	ToolBar->AddSeparator();
	ToolBar->AddTool(ID_Dump, "File Dump", SaveIcon, wxNullBitmap, wxITEM_NORMAL, "File Dump", "Dump block to disk", NULL);
	ToolBar->EnableTool(ID_Dump, FALSE);
	ToolBar->AddTool(ID_View, "Hex Viewer", HexIcon, wxNullBitmap, wxITEM_NORMAL, "Hex Viewer", "View block as hex", NULL);
	ToolBar->EnableTool(ID_View, FALSE);
	ToolBar->AddTool(Button_Options, "Options", OptionsIcon, wxNullBitmap, wxITEM_NORMAL, "Options", "Options", NULL);
	ToolBar->EnableTool(Button_Options, FALSE);
	ToolBar->AddSeparator();
	ToolBar->AddTool(ID_Help, "Help", HelpIcon, wxNullBitmap, wxITEM_NORMAL, "Help", "View help", NULL);
	ToolBar->Realize();
	
	panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "panel");
	
	wxBoxSizer *RootSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer *BlocksSizer = new wxBoxSizer( wxHORIZONTAL );

	RootSizer->Add(BlocksSizer,
			1,
			wxALL|wxEXPAND,
			0
			);

	tree = new wxTreeCtrl(panel, Tree, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT, wxDefaultValidator, "listCtrl");
	
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
	
	SpecLabel[0] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel1");
	SpecLabel[1] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel2");
	SpecLabel[2] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel3");
	SpecLabel[3] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel4");
	SpecLabel[4] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel5");
	SpecLabel[5] = new wxStaticText(infospanel, -1, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, "SpecLabel6");
	
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
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageDialog *dialog = new wxMessageDialog(this, "\nScummEX 0.1.0 by Adrien Mercier <yosh@yosh.biz>\n","About", wxOK|wxCENTRE);
	dialog->ShowModal();
}

void MainWindow::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog *dialog = new wxFileDialog(frame, "Please select an input file.", "", "",
        "All Supported Files|*|Main Resource Files|*.001;*.la1;*.la2;*.lab;*.lfl;*.lec;*.sm1|Directory Files|*.000;*.la0;*.lfl;*.sm0",
        wxOPEN);
	if (dialog->ShowModal() == wxID_OK) {
		file = (const char*)dialog->GetPath();
		tree->DeleteChildren(iter[0]);
		_scummex->getFileType(file);
	}
}

void MainWindow::OnSelChanged(wxTreeEvent& event) {
	int itemid, itemtype;
	bool val1, val2;
	itemid = event.GetItem();
	TreeItemData *item = (TreeItemData *)tree->GetItemData(itemid);

	for (int i = 0; i < 6; i++)
		SpecLabel[i]->SetLabel("");
	SpecButton1->Show(FALSE);
	SpecButton2->Show(FALSE);
	ToolBar->EnableTool(ID_Dump, TRUE);
	ToolBar->EnableTool(ID_View, TRUE);
	val1 = SpecButton1->Disconnect(ID_SpecButton1, wxEVT_COMMAND_BUTTON_CLICKED);
	val2 = SpecButton2->Disconnect(ID_SpecButton2, wxEVT_COMMAND_BUTTON_CLICKED);

	itemtype = item->block_type;
	switch (itemtype) {
		case LECF:
		case LABN:
		case LB83:
			bigIconBitmap = wxBitmap(xpm_6_big);
			break;
			
		case LOFF:
		case OFFS:
			bigIconBitmap = wxBitmap(xpm_44_big);
			break;
			
		case LFLF:
			bigIconBitmap = wxBitmap(xpm_58_big);
			break;
			
		case ROOM:
			bigIconBitmap = wxBitmap(xpm_38_big);
			break;
			
		case RMHD:
			bigIconBitmap = wxBitmap(xpm_35_big);
			break;
			
		case CYCL:
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
			bigIconBitmap = wxBitmap(xpm_34_big);
			break;
			
		case BOXD:
			bigIconBitmap = wxBitmap(xpm_4_big);
			break;
			
		case BOXM:
			bigIconBitmap = wxBitmap(xpm_5_big);
			break;
			
		case SCAL:
			bigIconBitmap = wxBitmap(xpm_40_big);
			break;
			
		case RMIM:
			bigIconBitmap = wxBitmap(xpm_37_big);
			break;
			
		case EXCD:
			bigIconBitmap = wxBitmap(xpm_22_big);
			break;

		case ENCD:
			bigIconBitmap = wxBitmap(xpm_21_big);
			break;
			
		case NLSC:
		case SCRP:
		case LSCR:
			bigIconBitmap = wxBitmap(xpm_41_big);
			break;
			
		case SOUN:
		case SOU:
		case iMUS:
		case cus2:
		case IMC:
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
		case BSTR:
			bigIconBitmap = wxBitmap(xpm_42_big);
			break;
			
		case OBIM:
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
			bigIconBitmap = wxBitmap(xpm_52_big);
			break;
			
		case COST:
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
			
	_scummex->UpdateInfosFromTree(item->block_id);
	event.Skip();
}
