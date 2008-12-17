/////////////////////////////////////////////////////////////////////////////
// Name:        tpaaframe.cpp
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/17/2008 2:16:56 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "tpaaframe.h"
#include "configaadlg.h"

////@begin XPM images
////@end XPM images


/*!
 * TPAAFrame type definition
 */

IMPLEMENT_CLASS( TPAAFrame, wxFrame )


/*!
 * TPAAFrame event table definition
 */

BEGIN_EVENT_TABLE( TPAAFrame, wxFrame )

////@begin TPAAFrame event table entries
    EVT_BUTTON( ID_BUTTON_CONFIG_AA_DLG, TPAAFrame::OnButtonConfigAaDlgClick )

////@end TPAAFrame event table entries

END_EVENT_TABLE()


/*!
 * TPAAFrame constructors
 */

TPAAFrame::TPAAFrame()
{
    Init();
}

TPAAFrame::TPAAFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * TPAAFrame creator
 */

bool TPAAFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin TPAAFrame creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("../image/Aardvark GUI.ico")));
    Centre();
////@end TPAAFrame creation
    return true;
}


/*!
 * TPAAFrame destructor
 */

TPAAFrame::~TPAAFrame()
{
////@begin TPAAFrame destruction
////@end TPAAFrame destruction
}


/*!
 * Member initialisation
 */

void TPAAFrame::Init()
{
////@begin TPAAFrame member initialisation
////@end TPAAFrame member initialisation
}


/*!
 * Control creation for TPAAFrame
 */

void TPAAFrame::CreateControls()
{    
////@begin TPAAFrame content construction
    TPAAFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENUITEM_FILE_CLOSE, _("Close"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu5 = new wxMenu;
    itemMenu5->Append(ID_MENUITEM_HELP_ABOUT, _("&About"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu5, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    wxStatusBar* itemStatusBar7 = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar7->SetFieldsCount(1);
    itemFrame1->SetStatusBar(itemStatusBar7);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer8);

    wxPanel* itemPanel9 = new wxPanel( itemFrame1, ID_PANEL_I2C, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
    itemPanel9->Show(false);
    itemBoxSizer8->Add(itemPanel9, 1, wxGROW, 0);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemPanel9->SetSizer(itemBoxSizer10);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer10->Add(itemBoxSizer11, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel9, wxID_STATIC, _("I2C Control"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText12->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer11->Add(itemStaticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel9, wxID_STATIC, _("Bitrate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemPanel9, ID_BUTTON, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer11->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice16Strings;
    itemChoice16Strings.Add(_("400"));
    itemChoice16Strings.Add(_("200"));
    itemChoice16Strings.Add(_("100"));
    wxChoice* itemChoice16 = new wxChoice( itemPanel9, ID_CHOICE, wxDefaultPosition, wxDefaultSize, itemChoice16Strings, 0 );
    itemBoxSizer11->Add(itemChoice16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel9, wxID_STATIC, _("kHz"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer10->Add(itemBoxSizer18, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel9, wxID_STATIC, _("Device Addr:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxTextCtrl* itemTextCtrl20 = new wxTextCtrl( itemPanel9, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemTextCtrl20, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel9, wxID_STATIC, _("(For Hex, enter 0x...)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxButton* itemButton23 = new wxButton( itemPanel9, ID_BUTTON1, _("Free Bus"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemButton23, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer10->Add(itemBoxSizer24, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemPanel9, wxID_STATIC, _("Features:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxCheckBox* itemCheckBox26 = new wxCheckBox( itemPanel9, ID_CHECKBOX, _("10-Bit Addr"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox26->SetValue(false);
    itemBoxSizer24->Add(itemCheckBox26, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxCheckBox* itemCheckBox27 = new wxCheckBox( itemPanel9, ID_CHECKBOX1, _("No Stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox27->SetValue(false);
    itemBoxSizer24->Add(itemCheckBox27, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer10->Add(itemBoxSizer28, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel9, wxID_STATIC, _("Master transmit/receive data"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText29->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer28->Add(itemStaticText29, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer10->Add(itemBoxSizer30, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    wxGrid* itemGrid31 = new wxGrid( itemPanel9, ID_GRID_RWDATA, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid31->SetDefaultColSize(24);
    itemGrid31->SetDefaultRowSize(18);
    itemGrid31->SetColLabelSize(18);
    itemGrid31->SetRowLabelSize(24);
    itemGrid31->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer30->Add(itemGrid31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSplitterWindow* itemSplitterWindow32 = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow32->SetMinimumPaneSize(200);

    wxPanel* itemPanel33 = new wxPanel( itemSplitterWindow32, ID_PANEL_CONFIG, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel33->SetSizer(itemBoxSizer34);

    itemBoxSizer34->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton36 = new wxButton( itemPanel33, ID_BUTTON_CONFIG_AA_DLG, _("Configure Aardvark Adapter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton36->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Courier New")));
    itemBoxSizer34->Add(itemButton36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer34->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxPanel* itemPanel38 = new wxPanel( itemSplitterWindow32, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxVERTICAL);
    itemPanel38->SetSizer(itemBoxSizer39);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel38, wxID_STATIC, _("Transaction Log:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer39->Add(itemStaticText40, 0, wxALIGN_LEFT|wxALL, 5);

    wxTextCtrl* itemTextCtrl41 = new wxTextCtrl( itemPanel38, ID_LOGWIN, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2|wxDOUBLE_BORDER );
    itemBoxSizer39->Add(itemTextCtrl41, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer39->Add(itemBoxSizer42, 0, wxALIGN_RIGHT|wxLEFT|wxTOP|wxBOTTOM, 5);
    wxButton* itemButton43 = new wxButton( itemPanel38, ID_BUTTON_LOG_CLEAR, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer42->Add(itemButton43, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxButton* itemButton44 = new wxButton( itemPanel38, ID_BUTTON_LOG_SAVE, _("Save to File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer42->Add(itemButton44, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    itemSplitterWindow32->SplitHorizontally(itemPanel33, itemPanel38, -200);
    itemBoxSizer8->Add(itemSplitterWindow32, 1, wxGROW, 0);

////@end TPAAFrame content construction
    ModifyControls();
}


/*!
 * Should we show tooltips?
 */

bool TPAAFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap TPAAFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin TPAAFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end TPAAFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TPAAFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin TPAAFrame icon retrieval
    wxUnusedVar(name);
    if (name == _T("../image/Aardvark GUI.ico"))
    {
        wxIcon icon(_T("../image/Aardvark GUI.ico"), wxBITMAP_TYPE_ICO);
        return icon;
    }
    return wxNullIcon;
////@end TPAAFrame icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONFIG_AA_DLG
 */

void TPAAFrame::OnButtonConfigAaDlgClick( wxCommandEvent& event )
{
    wxSplitterWindow *sw = wxDynamicCast(FindWindow(ID_SPLITTERWINDOW), wxSplitterWindow);
    wxPanel *cfgPanel = wxDynamicCast(FindWindow(ID_PANEL_CONFIG), wxPanel);
    wxPanel *i2cPanel = wxDynamicCast(FindWindow(ID_PANEL_I2C), wxPanel);
    ConfigAADlg dlg(this);
    if (wxID_OK == dlg.ShowModal())
    {
        cfgPanel->Hide();
        i2cPanel->Show();
        sw->ReplaceWindow(cfgPanel, i2cPanel);
    }
}

void TPAAFrame::ModifyControls(void)
{
    /* redirect log message to our textctrl window. */
    delete wxLog::SetActiveTarget(new wxLogTextCtrl(wxDynamicCast(FindWindow(ID_LOGWIN), wxTextCtrl)));
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S] "));
}
