/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyWizard.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     23/05/2010 20:28:00
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

#include "../include/MyKeyWizard.h"

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
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS|wxWIZARD_EX_HELPBUTTON);
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

    WizardPage* itemWizardPageSimple2 = new WizardPage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple2);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
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
 * WizardPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage, wxWizardPageSimple )


/*
 * WizardPage event table definition
 */

BEGIN_EVENT_TABLE( WizardPage, wxWizardPageSimple )

////@begin WizardPage event table entries
////@end WizardPage event table entries

END_EVENT_TABLE()


/*
 * WizardPage constructors
 */

WizardPage::WizardPage()
{
    Init();
}

WizardPage::WizardPage( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*
 * WizardPage creator
 */

bool WizardPage::Create( wxWizard* parent )
{
////@begin WizardPage creation
    wxBitmap wizardBitmap(GetBitmapResource(wxT("resource/images/wizard.xpm")));
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
////@end WizardPage creation
    return true;
}


/*
 * WizardPage destructor
 */

WizardPage::~WizardPage()
{
////@begin WizardPage destruction
////@end WizardPage destruction
}


/*
 * Member initialisation
 */

void WizardPage::Init()
{
////@begin WizardPage member initialisation
////@end WizardPage member initialisation
}


/*
 * Control creation for WizardPage
 */

void WizardPage::CreateControls()
{    
////@begin WizardPage content construction
    WizardPage* itemWizardPageSimple2 = this;

    wxStaticText* itemStaticText3 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("Welcome to My Key.\nThe purpose of this application is to generate some secret code for the Rockey4 USB dongle.\nPlease insert the USB dongle that you want to configure, and notice that only one dongle can be configured at the same time. If you have multiple dongle inserted in this computer, please temporary remove those you don't want to configure."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->Wrap(300);

////@end WizardPage content construction
}


/*
 * Should we show tooltips?
 */

bool WizardPage::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("resource/images/wizard.xpm"))
    {
        wxBitmap bitmap(wizard_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end WizardPage bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage icon retrieval
}
