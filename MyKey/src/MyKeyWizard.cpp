/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyWizard.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     24/05/2010 00:37:08
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

#include "MyKeyWizard.h"

////@begin XPM images
#include "../resource/images/wizard.xpm"
////@end XPM images


/*
 * MyKeyWizard type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MyKeyWizard, wxWizard )


/*
 * MyKeyWizard event table definition
 */

BEGIN_EVENT_TABLE( MyKeyWizard, wxWizard )

////@begin MyKeyWizard event table entries
////@end MyKeyWizard event table entries

END_EVENT_TABLE()


/*
 * MyKeyWizard constructors
 */

MyKeyWizard::MyKeyWizard()
{
    Init();
}

MyKeyWizard::MyKeyWizard( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
    Init();
    Create(parent, id, pos);
}


/*
 * MyKeyWizard creator
 */

bool MyKeyWizard::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin MyKeyWizard creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxBitmap wizardBitmap(GetBitmapResource(wxT("resource/images/wizard.xpm")));
    wxWizard::Create( parent, id, _("My Key"), wizardBitmap, pos, wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    CreateControls();
////@end MyKeyWizard creation
    return true;
}


/*
 * MyKeyWizard destructor
 */

MyKeyWizard::~MyKeyWizard()
{
////@begin MyKeyWizard destruction
////@end MyKeyWizard destruction
}


/*
 * Member initialisation
 */

void MyKeyWizard::Init()
{
////@begin MyKeyWizard member initialisation
////@end MyKeyWizard member initialisation
}


/*
 * Control creation for MyKeyWizard
 */

void MyKeyWizard::CreateControls()
{    
////@begin MyKeyWizard content construction
    MyKeyWizard* itemWizard1 = this;

    WizardPageWelcome* itemWizardPageSimple2 = new WizardPageWelcome( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple2);

    WizardPagePasswd* itemWizardPageSimple4 = new WizardPagePasswd( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple4);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple4);
    lastPage = itemWizardPageSimple4;
////@end MyKeyWizard content construction
}


/*
 * Runs the wizard.
 */

bool MyKeyWizard::Run()
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node)
    {
        wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
        if (startPage) return RunWizard(startPage);
        node = node->GetNext();
    }
    return false;
}


/*
 * Should we show tooltips?
 */

bool MyKeyWizard::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MyKeyWizard::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MyKeyWizard bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("resource/images/wizard.xpm"))
    {
        wxBitmap bitmap(wizard_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MyKeyWizard bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MyKeyWizard::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MyKeyWizard icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MyKeyWizard icon retrieval
}


/*
 * WizardPageWelcome type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPageWelcome, wxWizardPageSimple )


/*
 * WizardPageWelcome event table definition
 */

BEGIN_EVENT_TABLE( WizardPageWelcome, wxWizardPageSimple )

////@begin WizardPageWelcome event table entries
////@end WizardPageWelcome event table entries

END_EVENT_TABLE()


/*
 * WizardPageWelcome constructors
 */

WizardPageWelcome::WizardPageWelcome()
{
    Init();
}

WizardPageWelcome::WizardPageWelcome( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*
 * WizardPageWelcome creator
 */

bool WizardPageWelcome::Create( wxWizard* parent )
{
////@begin WizardPageWelcome creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
////@end WizardPageWelcome creation
    return true;
}


/*
 * WizardPageWelcome destructor
 */

WizardPageWelcome::~WizardPageWelcome()
{
////@begin WizardPageWelcome destruction
////@end WizardPageWelcome destruction
}


/*
 * Member initialisation
 */

void WizardPageWelcome::Init()
{
////@begin WizardPageWelcome member initialisation
////@end WizardPageWelcome member initialisation
}


/*
 * Control creation for WizardPageWelcome
 */

void WizardPageWelcome::CreateControls()
{    
////@begin WizardPageWelcome content construction
    WizardPageWelcome* itemWizardPageSimple2 = this;

    wxStaticText* itemStaticText3 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("Welcome to My Key.\nThe purpose of this application is to generate some secret code for the Rockey4 USB dongle.\nPlease insert the USB dongle that you want to configure, and notice that only one dongle can be configured at the same time. If you have multiple dongle inserted in this computer, please temporary remove those you don't want to configure."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->Wrap(300);

////@end WizardPageWelcome content construction
}


/*
 * Should we show tooltips?
 */

bool WizardPageWelcome::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPageWelcome::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPageWelcome bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPageWelcome bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPageWelcome::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPageWelcome icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPageWelcome icon retrieval
}


/*
 * WizardPagePasswd type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPagePasswd, wxWizardPageSimple )


/*
 * WizardPagePasswd event table definition
 */

BEGIN_EVENT_TABLE( WizardPagePasswd, wxWizardPageSimple )

////@begin WizardPagePasswd event table entries
////@end WizardPagePasswd event table entries

END_EVENT_TABLE()


/*
 * WizardPagePasswd constructors
 */

WizardPagePasswd::WizardPagePasswd()
{
    Init();
}

WizardPagePasswd::WizardPagePasswd( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*
 * WizardPagePasswd creator
 */

bool WizardPagePasswd::Create( wxWizard* parent )
{
////@begin WizardPagePasswd creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPagePasswd creation
    return true;
}


/*
 * WizardPagePasswd destructor
 */

WizardPagePasswd::~WizardPagePasswd()
{
////@begin WizardPagePasswd destruction
////@end WizardPagePasswd destruction
}


/*
 * Member initialisation
 */

void WizardPagePasswd::Init()
{
////@begin WizardPagePasswd member initialisation
////@end WizardPagePasswd member initialisation
}


/*
 * Control creation for WizardPagePasswd
 */

void WizardPagePasswd::CreateControls()
{    
////@begin WizardPagePasswd content construction
    WizardPagePasswd* itemWizardPageSimple4 = this;

    wxGridBagSizer* itemGridBagSizer5 = new wxGridBagSizer(0, 0);
    itemGridBagSizer5->SetEmptyCellSize(wxSize(80, 80));
    itemWizardPageSimple4->SetSizer(itemGridBagSizer5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Please input password:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText6, wxGBPosition(0, 0), wxGBSpan(1, 3), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level1 PW1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText7, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV1PW1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl8->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl8, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level1 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText9, wxGBPosition(2, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV1PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl10->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl10, wxGBPosition(2, 4), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level2 PW1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText11, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV2PW1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl12->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl12, wxGBPosition(3, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level2 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText13, wxGBPosition(3, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl14 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV2PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl14->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl14, wxGBPosition(3, 4), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemWizardPageSimple4, ID_BUTTON_DEMO, _("Use Demo Dongle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemButton15, wxGBPosition(4, 0), wxGBSpan(1, 2), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPagePasswd content construction
}


/*
 * Should we show tooltips?
 */

bool WizardPagePasswd::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPagePasswd::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPagePasswd bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPagePasswd bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPagePasswd::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPagePasswd icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPagePasswd icon retrieval
}
