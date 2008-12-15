/////////////////////////////////////////////////////////////////////////////
// Name:        configaadlg.cpp
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/15/2008 7:04:16 PM
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

#include "../include/configaadlg.h"

////@begin XPM images
////@end XPM images


/*!
 * ConfigAADlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConfigAADlg, wxDialog )


/*!
 * ConfigAADlg event table definition
 */

BEGIN_EVENT_TABLE( ConfigAADlg, wxDialog )

////@begin ConfigAADlg event table entries
////@end ConfigAADlg event table entries

END_EVENT_TABLE()


/*!
 * ConfigAADlg constructors
 */

ConfigAADlg::ConfigAADlg()
{
    Init();
}

ConfigAADlg::ConfigAADlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ConfigAADlg creator
 */

bool ConfigAADlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ConfigAADlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ConfigAADlg creation
    return true;
}


/*!
 * ConfigAADlg destructor
 */

ConfigAADlg::~ConfigAADlg()
{
////@begin ConfigAADlg destruction
////@end ConfigAADlg destruction
}


/*!
 * Member initialisation
 */

void ConfigAADlg::Init()
{
////@begin ConfigAADlg member initialisation
////@end ConfigAADlg member initialisation
}


/*!
 * Control creation for ConfigAADlg
 */

void ConfigAADlg::CreateControls()
{    
////@begin ConfigAADlg content construction
    ConfigAADlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Configure Aardvark Adapter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer5, 2, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select an Aardvark"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText6->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Verdana")));
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

    wxListCtrl* itemListCtrl7 = new wxListCtrl( itemDialog1, ID_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_USER_TEXT|wxLC_SINGLE_SEL );
    itemBoxSizer5->Add(itemListCtrl7, 3, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer8, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemRadioBox9Strings;
    itemRadioBox9Strings.Add(_("I2C - SPI"));
    itemRadioBox9Strings.Add(_("I2C - GPIO"));
    itemRadioBox9Strings.Add(_("SPI - GPIO"));
    itemRadioBox9Strings.Add(_("GPIO only"));
    itemRadioBox9Strings.Add(_("Batch Mode"));
    itemRadioBox9Strings.Add(_("I2C Monitor"));
    wxRadioBox* itemRadioBox9 = new wxRadioBox( itemDialog1, ID_RADIOBOX, _("Select a Mode"), wxDefaultPosition, wxDefaultSize, itemRadioBox9Strings, 6, wxRA_SPECIFY_ROWS );
    itemRadioBox9->SetSelection(0);
    itemBoxSizer8->Add(itemRadioBox9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end ConfigAADlg content construction
}


/*!
 * Should we show tooltips?
 */

bool ConfigAADlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ConfigAADlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConfigAADlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ConfigAADlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ConfigAADlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ConfigAADlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ConfigAADlg icon retrieval
}
