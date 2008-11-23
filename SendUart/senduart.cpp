/////////////////////////////////////////////////////////////////////////////
// Name:        senduart.cpp
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     23/11/2008 21:21:49
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
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
#include "wx/bookctrl.h"
////@end includes

#include "senduart.h"

////@begin XPM images
////@end XPM images


/*!
 * SendUart type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SendUart, wxPropertySheetDialog )


/*!
 * SendUart event table definition
 */

BEGIN_EVENT_TABLE( SendUart, wxPropertySheetDialog )

////@begin SendUart event table entries
////@end SendUart event table entries

END_EVENT_TABLE()


/*!
 * SendUart constructors
 */

SendUart::SendUart()
{
    Init();
}

SendUart::SendUart( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * SendUart creator
 */

bool SendUart::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SendUart creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    SetSheetStyle(wxPROPSHEET_DEFAULT);
    wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

    CreateButtons(wxOK|wxCANCEL|wxHELP);
    CreateControls();
    LayoutDialog();
    Centre();
////@end SendUart creation
    return true;
}


/*!
 * SendUart destructor
 */

SendUart::~SendUart()
{
////@begin SendUart destruction
////@end SendUart destruction
}


/*!
 * Member initialisation
 */

void SendUart::Init()
{
////@begin SendUart member initialisation
////@end SendUart member initialisation
}


/*!
 * Control creation for SendUart
 */

void SendUart::CreateControls()
{    
////@begin SendUart content construction
    SendUart* itemPropertySheetDialog1 = this;

    wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PANEL_GENERATION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel3 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemPanel3->SetSizer(itemBoxSizer4);

    wxStaticBox* itemStaticBoxSizer5Static = new wxStaticBox(itemPanel3, wxID_ANY, _("File Settting"));
    wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(itemStaticBoxSizer5Static, wxVERTICAL);
    itemBoxSizer4->Add(itemStaticBoxSizer5, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer5->Add(itemBoxSizer6, 0, wxGROW|wxALL, 0);
    wxButton* itemButton7 = new wxButton( itemPanel3, ID_BUTTON_FILE_LOCATION, _("File Location ..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemPanel3, ID_TEXTCTRL_FILE_PATH, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_LEFT );
    itemBoxSizer6->Add(itemTextCtrl8, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemPanel3, ID_BUTTON_LOAD_FILE, _("Load File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer5->Add(itemBoxSizer10, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid11 = new wxGrid( itemPanel3, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid11->SetDefaultColSize(45);
    itemGrid11->SetDefaultRowSize(18);
    itemGrid11->SetColLabelSize(18);
    itemGrid11->SetRowLabelSize(20);
    itemGrid11->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer10->Add(itemGrid11, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemPanel3, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxVERTICAL);
    itemBoxSizer10->Add(itemStaticBoxSizer12, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer13 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer13->AddGrowableCol(1);
    itemStaticBoxSizer12->Add(itemFlexGridSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel3, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel3, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText15, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel3, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel3, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel3, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText18, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel3, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText19, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel3, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText20, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel3, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText21, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel3, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText22, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText23 = new wxStaticText( itemPanel3, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText23, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer24, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer25Static = new wxStaticBox(itemPanel3, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer25 = new wxStaticBoxSizer(itemStaticBoxSizer25Static, wxHORIZONTAL);
    itemBoxSizer24->Add(itemStaticBoxSizer25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer26 = new wxFlexGridSizer(0, 3, 0, 5);
    itemFlexGridSizer26->AddGrowableCol(1);
    itemStaticBoxSizer25->Add(itemFlexGridSizer26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel3, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText27, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice28Strings;
    wxChoice* itemChoice28 = new wxChoice( itemPanel3, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice28Strings, 0 );
    itemFlexGridSizer26->Add(itemChoice28, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxButton* itemButton29 = new wxButton( itemPanel3, ID_BUTTON_SCAN_PORT, _("Re-scan available port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemButton29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel3, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice31Strings;
    itemChoice31Strings.Add(_("115200"));
    itemChoice31Strings.Add(_("57600"));
    itemChoice31Strings.Add(_("38400"));
    itemChoice31Strings.Add(_("19200"));
    itemChoice31Strings.Add(_("9600"));
    wxChoice* itemChoice31 = new wxChoice( itemPanel3, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice31Strings, 0 );
    itemChoice31->SetStringSelection(_("115200"));
    itemFlexGridSizer26->Add(itemChoice31, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel3, wxID_STATIC, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel3, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice34Strings;
    itemChoice34Strings.Add(_("8"));
    itemChoice34Strings.Add(_("7"));
    itemChoice34Strings.Add(_("6"));
    itemChoice34Strings.Add(_("5"));
    wxChoice* itemChoice34 = new wxChoice( itemPanel3, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice34Strings, 0 );
    itemChoice34->SetStringSelection(_("8"));
    itemFlexGridSizer26->Add(itemChoice34, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel3, wxID_STATIC, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText35, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel3, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText36, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice37Strings;
    itemChoice37Strings.Add(_("None"));
    itemChoice37Strings.Add(_("Even"));
    itemChoice37Strings.Add(_("Odd"));
    itemChoice37Strings.Add(_("Mark"));
    itemChoice37Strings.Add(_("Space"));
    wxChoice* itemChoice37 = new wxChoice( itemPanel3, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice37Strings, 0 );
    itemChoice37->SetStringSelection(_("None"));
    itemFlexGridSizer26->Add(itemChoice37, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel3, wxID_STATIC, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText38, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel3, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText39, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice40Strings;
    itemChoice40Strings.Add(_("1"));
    itemChoice40Strings.Add(_("2"));
    wxChoice* itemChoice40 = new wxChoice( itemPanel3, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice40Strings, 0 );
    itemChoice40->SetStringSelection(_("1"));
    itemFlexGridSizer26->Add(itemChoice40, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel3, wxID_STATIC, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText41, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer42Static = new wxStaticBox(itemPanel3, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer42 = new wxStaticBoxSizer(itemStaticBoxSizer42Static, wxVERTICAL);
    itemBoxSizer24->Add(itemStaticBoxSizer42, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer42->Add(itemBoxSizer43, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel3, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer43->Add(itemStaticText44, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel3, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer43->Add(itemStaticText45, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer42->Add(itemBoxSizer46, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText47 = new wxStaticText( itemPanel3, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer46->Add(itemStaticText47, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText48 = new wxStaticText( itemPanel3, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer46->Add(itemStaticText48, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer42->Add(itemBoxSizer49, 1, wxGROW|wxALL, 5);
    wxButton* itemButton50 = new wxButton( itemPanel3, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer49->Add(itemButton50, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel3, _("Transmission"));

////@end SendUart content construction
}


/*!
 * Should we show tooltips?
 */

bool SendUart::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SendUart::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SendUart bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SendUart bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SendUart::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SendUart icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SendUart icon retrieval
}
