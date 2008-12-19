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
#include "i2cctrlpanel.h"

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

    wxPanel* itemPanel8 = new wxPanel( itemFrame1, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemPanel8->SetSizer(itemBoxSizer9);

    wxSplitterWindow* itemSplitterWindow10 = new wxSplitterWindow( itemPanel8, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_3DBORDER|wxSP_3DSASH|wxDOUBLE_BORDER );
    itemSplitterWindow10->SetMinimumPaneSize(200);

    wxPanel* itemPanel11 = new wxPanel( itemSplitterWindow10, ID_PANEL_CONFIG, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel11->SetSizer(itemBoxSizer12);

    itemBoxSizer12->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemPanel11, ID_BUTTON_CONFIG_AA_DLG, _("Configure Aardvark Adapter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton14->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Courier New")));
    itemBoxSizer12->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer12->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxPanel* itemPanel16 = new wxPanel( itemSplitterWindow10, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemPanel16->SetSizer(itemBoxSizer17);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel16, wxID_STATIC, _("Transaction Log:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_LEFT|wxALL, 5);

    wxTextCtrl* itemTextCtrl19 = new wxTextCtrl( itemPanel16, ID_LOGWIN, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2|wxDOUBLE_BORDER );
    itemBoxSizer17->Add(itemTextCtrl19, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer20, 0, wxALIGN_RIGHT|wxLEFT|wxTOP|wxBOTTOM, 5);
    wxButton* itemButton21 = new wxButton( itemPanel16, ID_BUTTON_LOG_CLEAR, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxButton* itemButton22 = new wxButton( itemPanel16, ID_BUTTON_LOG_SAVE, _("Save to File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    itemSplitterWindow10->SplitHorizontally(itemPanel11, itemPanel16, -200);
    itemBoxSizer9->Add(itemSplitterWindow10, 1, wxGROW, 0);

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
    
    /* show config aa dialog */
    ConfigAADlg dlg(this);
    if (wxID_OK == dlg.ShowModal())
    {
        sw->ReplaceWindow(cfgPanel, new i2cCtrlPanel(sw));
        cfgPanel->Destroy();
    }
}

void TPAAFrame::ModifyControls(void)
{
    /* redirect log message to our textctrl window. */
    delete wxLog::SetActiveTarget(new wxLogTextCtrl(wxDynamicCast(FindWindow(ID_LOGWIN), wxTextCtrl)));
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S] "));
}
