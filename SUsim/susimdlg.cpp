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
    LoadInterface();
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
    m_boardNameTextCtrl = NULL;
    m_response = NULL;
    m_interface = NULL;
    m_enableBtn = NULL;
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
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    m_boardNameTextCtrl = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_boardNameTextCtrl->SetMaxLength(15);
    m_boardNameTextCtrl->SetHelpText(_("You can assign a board name for this simulated target.\nWhen query request coming from remote, this simulated\ntarget will use the specific board name as its response\nmessage. The max length of board name is 15 bytes."));
    if (SUSimDlg::ShowToolTips())
        m_boardNameTextCtrl->SetToolTip(_("You can assign a board name for this simulated target.\nWhen query request coming from remote, this simulated\ntarget will use the specific board name as its response\nmessage. The max length of board name is 15 bytes."));
    itemBoxSizer5->Add(m_boardNameTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer8->Add(itemBoxSizer9, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select one response action"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALL, 5);

    wxArrayString m_responseStrings;
    m_responseStrings.Add(_("No Error"));
    m_responseStrings.Add(_("Syntax Error"));
    m_responseStrings.Add(_("Ram Checksum Error"));
    m_responseStrings.Add(_("Brec Checksum Error"));
    m_responseStrings.Add(_("Unknown Load Mode"));
    m_responseStrings.Add(_("Version Error"));
    m_responseStrings.Add(_("Memory Allocation Error"));
    m_responseStrings.Add(_("Erase Flash Error"));
    m_responseStrings.Add(_("Read Flash Error"));
    m_responseStrings.Add(_("Write Flash Error"));
    m_responseStrings.Add(_("Firmware Size Error"));
    m_responseStrings.Add(_("Bootloader Size Error"));
    m_response = new wxListBox( itemDialog1, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, m_responseStrings, wxLB_SINGLE );
    m_response->SetStringSelection(_("No Error"));
    m_response->SetHelpText(_("You can choose one result from this list box. The result\nwill be used as requested action response back to remote.\nYou can test the how the official Updater react."));
    if (SUSimDlg::ShowToolTips())
        m_response->SetToolTip(_("You can choose one result from this list box. The result\nwill be used as requested action response back to remote.\nYou can test the how the official Updater react."));
    itemBoxSizer9->Add(m_response, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer8->Add(itemBoxSizer12, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select one interface"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALL, 5);

    wxArrayString m_interfaceStrings;
    m_interface = new wxListBox( itemDialog1, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, m_interfaceStrings, wxLB_SINGLE );
    itemBoxSizer12->Add(m_interface, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);

    m_enableBtn = new wxToggleButton( itemDialog1, ID_TOGGLEBUTTON, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
    m_enableBtn->SetValue(false);
    m_enableBtn->SetHelpText(_("A toggle button to enable/disable the simulated\ntarget functions."));
    if (SUSimDlg::ShowToolTips())
        m_enableBtn->SetToolTip(_("A toggle button to enable/disable the simulated\ntarget functions."));
    itemBoxSizer15->Add(m_enableBtn, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer17Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Transaction Log"));
    wxStaticBoxSizer* itemStaticBoxSizer17 = new wxStaticBoxSizer(itemStaticBoxSizer17Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer17, 1, wxGROW|wxALL, 5);

    wxTextCtrl* itemTextCtrl18 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(800, 300), wxTE_MULTILINE );
    itemStaticBoxSizer17->Add(itemTextCtrl18, 1, wxGROW|wxALL, 5);

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





void SUSimDlg::LoadInterface()
{
    size_t numAdapters = m_adapters.GetAdapterCount();
    
    if (numAdapters > 0) {
        wxString p[numAdapters];
        
        for (size_t id = 0; id < numAdapters; id++)
            p[id] = m_adapters.GetAdapterIpAddress(id);
            
        m_interface->Set(numAdapters, p);
    }
}
