/////////////////////////////////////////////////////////////////////////////
// Name:        i2cctrlpanel.cpp
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/18/2008 2:11:35 PM
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

#include "../include/i2cctrlpanel.h"

////@begin XPM images
////@end XPM images


/*!
 * i2cCtrlPanel type definition
 */

IMPLEMENT_DYNAMIC_CLASS( i2cCtrlPanel, wxPanel )


/*!
 * i2cCtrlPanel event table definition
 */

BEGIN_EVENT_TABLE( i2cCtrlPanel, wxPanel )

////@begin i2cCtrlPanel event table entries
////@end i2cCtrlPanel event table entries

END_EVENT_TABLE()


/*!
 * i2cCtrlPanel constructors
 */

i2cCtrlPanel::i2cCtrlPanel()
{
    Init();
}

i2cCtrlPanel::i2cCtrlPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * i2cCtrlPanel creator
 */

bool i2cCtrlPanel::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin i2cCtrlPanel creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end i2cCtrlPanel creation
    return true;
}


/*!
 * i2cCtrlPanel destructor
 */

i2cCtrlPanel::~i2cCtrlPanel()
{
////@begin i2cCtrlPanel destruction
////@end i2cCtrlPanel destruction
}


/*!
 * Member initialisation
 */

void i2cCtrlPanel::Init()
{
////@begin i2cCtrlPanel member initialisation
////@end i2cCtrlPanel member initialisation
}


/*!
 * Control creation for i2cCtrlPanel
 */

void i2cCtrlPanel::CreateControls()
{    
////@begin i2cCtrlPanel content construction
    i2cCtrlPanel* itemPanel1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemPanel1, wxID_STATIC, _("I2C Control"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemPanel1, wxID_STATIC, _("Bitrate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemPanel1, ID_BUTTON, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer3->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice8Strings;
    itemChoice8Strings.Add(_("400"));
    itemChoice8Strings.Add(_("200"));
    itemChoice8Strings.Add(_("100"));
    wxChoice* itemChoice8 = new wxChoice( itemPanel1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, itemChoice8Strings, 0 );
    itemBoxSizer3->Add(itemChoice8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel1, wxID_STATIC, _("kHz"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel1, wxID_STATIC, _("Device Addr:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemPanel1, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemTextCtrl12, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel1, wxID_STATIC, _("(For Hex, enter 0x...)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxButton* itemButton15 = new wxButton( itemPanel1, ID_BUTTON1, _("Free Bus"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel1, wxID_STATIC, _("Features:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxCheckBox* itemCheckBox18 = new wxCheckBox( itemPanel1, ID_CHECKBOX, _("10-Bit Addr"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox18->SetValue(false);
    itemBoxSizer16->Add(itemCheckBox18, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxCheckBox* itemCheckBox19 = new wxCheckBox( itemPanel1, ID_CHECKBOX1, _("No Stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox19->SetValue(false);
    itemBoxSizer16->Add(itemCheckBox19, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel1, wxID_STATIC, _("Master transmit/receive data"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText21->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);

    wxGrid* itemGrid23 = new wxGrid( itemPanel1, ID_GRID_RWDATA, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid23->SetDefaultColSize(24);
    itemGrid23->SetDefaultRowSize(18);
    itemGrid23->SetColLabelSize(18);
    itemGrid23->SetRowLabelSize(24);
    itemGrid23->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer22->Add(itemGrid23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end i2cCtrlPanel content construction
}


/*!
 * Should we show tooltips?
 */

bool i2cCtrlPanel::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap i2cCtrlPanel::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin i2cCtrlPanel bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end i2cCtrlPanel bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon i2cCtrlPanel::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin i2cCtrlPanel icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end i2cCtrlPanel icon retrieval
}
