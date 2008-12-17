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

    wxSplitterWindow* itemSplitterWindow9 = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_BORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow9->SetMinimumPaneSize(200);

    wxPanel* itemPanel10 = new wxPanel( itemSplitterWindow9, ID_PANEL_CONFIG, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel10->SetSizer(itemBoxSizer11);

    itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemPanel10, ID_BUTTON_CONFIG_AA_DLG, _("Configure Aardvark Adapter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton13->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Courier New")));
    itemBoxSizer11->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxPanel* itemPanel15 = new wxPanel( itemSplitterWindow9, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
    itemPanel15->SetSizer(itemBoxSizer16);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel15, wxID_STATIC, _("Transaction Log:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_LEFT|wxALL, 5);

    wxTextCtrl* itemTextCtrl18 = new wxTextCtrl( itemPanel15, ID_LOGWIN, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2|wxDOUBLE_BORDER );
    itemBoxSizer16->Add(itemTextCtrl18, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer16->Add(itemBoxSizer19, 0, wxALIGN_RIGHT|wxLEFT|wxTOP|wxBOTTOM, 5);
    wxButton* itemButton20 = new wxButton( itemPanel15, ID_BUTTON_LOG_CLEAR, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton20, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxButton* itemButton21 = new wxButton( itemPanel15, ID_BUTTON_LOG_SAVE, _("Save to File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    itemSplitterWindow9->SplitHorizontally(itemPanel10, itemPanel15, -200);
    itemBoxSizer8->Add(itemSplitterWindow9, 1, wxGROW, 0);

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
    ConfigAADlg dlg(this);
    if (wxID_OK == dlg.ShowModal())
    {
        wxLogMessage(wxT("OK"));
    }
}

void TPAAFrame::ModifyControls(void)
{
    /* redirect log message to our textctrl window. */
    delete wxLog::SetActiveTarget(new wxLogTextCtrl(wxDynamicCast(FindWindow(ID_LOGWIN), wxTextCtrl)));
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S] "));
}
