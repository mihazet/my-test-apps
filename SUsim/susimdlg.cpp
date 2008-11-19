/////////////////////////////////////////////////////////////////////////////
// Name:        susimdlg.cpp
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/19/2008 2:46:43 PM
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
////@end includes

#include "susimdlg.h"

////@begin XPM images
////@end XPM images


/*!
 * SUSimDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SUSimDlg, wxDialog )


/*!
 * SUSimDlg event table definition
 */

BEGIN_EVENT_TABLE( SUSimDlg, wxDialog )

////@begin SUSimDlg event table entries
////@end SUSimDlg event table entries

END_EVENT_TABLE()


/*!
 * SUSimDlg constructors
 */

SUSimDlg::SUSimDlg()
{
    Init();
}

SUSimDlg::SUSimDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * SUSimDlg creator
 */

bool SUSimDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SUSimDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SUSimDlg creation
    return true;
}


/*!
 * SUSimDlg destructor
 */

SUSimDlg::~SUSimDlg()
{
////@begin SUSimDlg destruction
////@end SUSimDlg destruction
}


/*!
 * Member initialisation
 */

void SUSimDlg::Init()
{
////@begin SUSimDlg member initialisation
////@end SUSimDlg member initialisation
}


/*!
 * Control creation for SUSimDlg
 */

void SUSimDlg::CreateControls()
{    
////@begin SUSimDlg content construction
    SUSimDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Options Setup"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Assign response board name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemTextCtrl7->SetMaxLength(15);
    itemBoxSizer5->Add(itemTextCtrl7, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select one response action"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer4->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALL, 5);

    wxArrayString itemListBox9Strings;
    itemListBox9Strings.Add(_("No Error"));
    itemListBox9Strings.Add(_("Syntax Error"));
    itemListBox9Strings.Add(_("Ram Checksum Error"));
    itemListBox9Strings.Add(_("Brec Checksum Error"));
    itemListBox9Strings.Add(_("Unknown Load Mode"));
    itemListBox9Strings.Add(_("Version Error"));
    itemListBox9Strings.Add(_("Memory Allocation Error"));
    itemListBox9Strings.Add(_("Erase Flash Error"));
    itemListBox9Strings.Add(_("Read Flash Error"));
    itemListBox9Strings.Add(_("Write Flash Error"));
    itemListBox9Strings.Add(_("Firmware Size Error"));
    itemListBox9Strings.Add(_("Bootloader Size Error"));
    wxListBox* itemListBox9 = new wxListBox( itemDialog1, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, itemListBox9Strings, wxLB_SINGLE );
    itemListBox9->SetStringSelection(_("No Error"));
    itemStaticBoxSizer4->Add(itemListBox9, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

    wxToggleButton* itemToggleButton11 = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    itemToggleButton11->SetValue(false);
    itemBoxSizer10->Add(itemToggleButton11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Transaction Log"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer12, 1, wxGROW|wxALL, 5);

    wxTextCtrl* itemTextCtrl13 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(800, 300), wxTE_MULTILINE );
    itemStaticBoxSizer12->Add(itemTextCtrl13, 1, wxGROW|wxALL, 5);

////@end SUSimDlg content construction
}


/*!
 * Should we show tooltips?
 */

bool SUSimDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SUSimDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SUSimDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SUSimDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SUSimDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SUSimDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SUSimDlg icon retrieval
}
