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
#include "wx/tokenzr.h"
#endif

////@begin includes
////@end includes

#include "MyKeyApp.h"
#include "MyKeyWizard.h"
#include "Rockey4_ND_32.h"
#include "wxMD5.h"

////@begin XPM images
#include "../resource/images/wizard.xpm"
#include "../resource/images/MyKey_32.xpm"
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
    SetIcon(GetIconResource(wxT("resource/images/MyKey_32.xpm")));
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

    WizardPagePasswd* itemWizardPageSimple6 = new WizardPagePasswd( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple6);

    WizardPageKeyInfo* itemWizardPageSimple18 = new WizardPageKeyInfo( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple18);

    WizardPageDone* itemWizardPageSimple33 = new WizardPageDone( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple33);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple6);
    lastPage = itemWizardPageSimple6;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple18);
    lastPage = itemWizardPageSimple18;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple33);
    lastPage = itemWizardPageSimple33;
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
    if (name == _T("resource/images/MyKey_32.xpm"))
    {
        wxIcon icon(MyKey_32_xpm);
        return icon;
    }
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
    if (GetSizer())
        GetSizer()->Fit(this);
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

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple2->SetSizer(itemBoxSizer3);

    wxStaticText* itemStaticText4 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("Welcome to My Key"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("The purpose of this application is to generate some secret code for the Rockey4 USB dongle.\nPlease insert the USB dongle that you want to configure, and notice that only one dongle can be configured at the same time. If you have multiple dongle inserted in this computer, please temporary remove those you don't want to configure."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText5->Wrap(300);
    itemBoxSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

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
    EVT_WIZARD_PAGE_CHANGING( -1, WizardPagePasswd::OnPasswdPageChanging )

    EVT_BUTTON( ID_BUTTON_DEMO, WizardPagePasswd::OnButtonDemoClick )

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
    WizardPagePasswd* itemWizardPageSimple6 = this;

    wxGridBagSizer* itemGridBagSizer7 = new wxGridBagSizer(0, 0);
    itemGridBagSizer7->SetEmptyCellSize(wxSize(80, 80));
    itemWizardPageSimple6->SetSizer(itemGridBagSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Please input password:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemStaticText8, wxGBPosition(0, 0), wxGBSpan(1, 3), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Level1 PW1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemStaticText9, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemWizardPageSimple6, ID_TEXTCTRL_LV1PW1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl10->SetMaxLength(4);
    if (WizardPagePasswd::ShowToolTips())
        itemTextCtrl10->SetToolTip(_("Please provide the basic password #1 of the dongle here.\nThe password is a 4-digit hexdecimal number, ex: 01AB"));
    itemGridBagSizer7->Add(itemTextCtrl10, wxGBPosition(2, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Level1 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemStaticText11, wxGBPosition(2, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemWizardPageSimple6, ID_TEXTCTRL_LV1PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl12->SetMaxLength(4);
    if (WizardPagePasswd::ShowToolTips())
        itemTextCtrl12->SetToolTip(_("Please provide the basic password #2 of the dongle here.\nThe password is a 4-digit hexdecimal number, ex: 01AB"));
    itemGridBagSizer7->Add(itemTextCtrl12, wxGBPosition(2, 4), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Level2 PW1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemStaticText13, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl14 = new wxTextCtrl( itemWizardPageSimple6, ID_TEXTCTRL_LV2PW1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl14->SetMaxLength(4);
    if (WizardPagePasswd::ShowToolTips())
        itemTextCtrl14->SetToolTip(_("Please provide the advanced password #1 of the dongle here.\nThe password is a 4-digit hexdecimal number, ex: 01AB"));
    itemGridBagSizer7->Add(itemTextCtrl14, wxGBPosition(3, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Level2 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemStaticText15, wxGBPosition(3, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl16 = new wxTextCtrl( itemWizardPageSimple6, ID_TEXTCTRL_LV2PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl16->SetMaxLength(4);
    if (WizardPagePasswd::ShowToolTips())
        itemTextCtrl16->SetToolTip(_("Please provide the advanced password #2 of the dongle here.\nThe password is a 4-digit hexdecimal number, ex: 01AB"));
    itemGridBagSizer7->Add(itemTextCtrl16, wxGBPosition(3, 4), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton17 = new wxButton( itemWizardPageSimple6, ID_BUTTON_DEMO, _("Use Demo Dongle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer7->Add(itemButton17, wxGBPosition(4, 0), wxGBSpan(1, 2), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

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


/*
 * WizardPageKeyInfo type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPageKeyInfo, wxWizardPageSimple )


/*
 * WizardPageKeyInfo event table definition
 */

BEGIN_EVENT_TABLE( WizardPageKeyInfo, wxWizardPageSimple )

////@begin WizardPageKeyInfo event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPageKeyInfo::OnKeyInfoPageChanged )
    EVT_WIZARD_PAGE_CHANGING( -1, WizardPageKeyInfo::OnKeyInfoPageChanging )

////@end WizardPageKeyInfo event table entries

END_EVENT_TABLE()


/*
 * WizardPageKeyInfo constructors
 */

WizardPageKeyInfo::WizardPageKeyInfo()
{
    Init();
}

WizardPageKeyInfo::WizardPageKeyInfo( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*
 * WizardPage creator
 */

bool WizardPageKeyInfo::Create( wxWizard* parent )
{
////@begin WizardPageKeyInfo creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPageKeyInfo creation
    return true;
}


/*
 * WizardPageKeyInfo destructor
 */

WizardPageKeyInfo::~WizardPageKeyInfo()
{
////@begin WizardPageKeyInfo destruction
////@end WizardPageKeyInfo destruction
}


/*
 * Member initialisation
 */

void WizardPageKeyInfo::Init()
{
////@begin WizardPageKeyInfo member initialisation
////@end WizardPageKeyInfo member initialisation
}


/*
 * Control creation for WizardPage
 */

void WizardPageKeyInfo::CreateControls()
{    
////@begin WizardPageKeyInfo content construction
    WizardPageKeyInfo* itemWizardPageSimple18 = this;

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple18->SetSizer(itemBoxSizer19);

    wxStaticBox* itemStaticBoxSizer20Static = new wxStaticBox(itemWizardPageSimple18, wxID_ANY, _("Dongle Info"));
    wxStaticBoxSizer* itemStaticBoxSizer20 = new wxStaticBoxSizer(itemStaticBoxSizer20Static, wxVERTICAL);
    itemBoxSizer19->Add(itemStaticBoxSizer20, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer21 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer21->AddGrowableCol(1);
    itemStaticBoxSizer20->Add(itemFlexGridSizer21, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemWizardPageSimple18, wxID_STATIC, _("Hardware ID (hex)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer21->Add(itemStaticText22, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl23 = new wxTextCtrl( itemWizardPageSimple18, ID_TEXTCTRL_HWID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WizardPageKeyInfo::ShowToolTips())
        itemTextCtrl23->SetToolTip(_("This field cannot be edited. Each dongle in the world has an unique hardware ID. The number is hexdecimal."));
    itemTextCtrl23->Enable(false);
    itemFlexGridSizer21->Add(itemTextCtrl23, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText24 = new wxStaticText( itemWizardPageSimple18, wxID_STATIC, _("Software ID (hex)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer21->Add(itemStaticText24, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl25 = new wxTextCtrl( itemWizardPageSimple18, ID_TEXTCTRL_SWID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemTextCtrl25->SetMaxLength(8);
    if (WizardPageKeyInfo::ShowToolTips())
        itemTextCtrl25->SetToolTip(_("You can assign a software ID for this dongle. The software  ID is 32-bit length number, and it is a hexdecimal number."));
    itemFlexGridSizer21->Add(itemTextCtrl25, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(itemWizardPageSimple18, wxID_ANY, _("User Info"));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxVERTICAL);
    itemBoxSizer19->Add(itemStaticBoxSizer26, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer27 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer27->AddGrowableCol(1);
    itemStaticBoxSizer26->Add(itemFlexGridSizer27, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText28 = new wxStaticText( itemWizardPageSimple18, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText28, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl29 = new wxTextCtrl( itemWizardPageSimple18, ID_TEXTCTRL_USER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WizardPageKeyInfo::ShowToolTips())
        itemTextCtrl29->SetToolTip(_("Please assign the owner's name of this dongle. Space is allowed in the name string."));
    itemFlexGridSizer27->Add(itemTextCtrl29, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemWizardPageSimple18, wxID_STATIC, _("Contact"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl31 = new wxTextCtrl( itemWizardPageSimple18, ID_TEXTCTRL_CONTACT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WizardPageKeyInfo::ShowToolTips())
        itemTextCtrl31->SetToolTip(_("Please assign the owner's contact info in this field. For example, the email address or phone number. Space is not allowed in this information."));
    itemFlexGridSizer27->Add(itemTextCtrl31, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemRadioBox32Strings;
    itemRadioBox32Strings.Add(_("&MD5"));
    itemRadioBox32Strings.Add(_("&SHA1"));
    wxRadioBox* itemRadioBox32 = new wxRadioBox( itemWizardPageSimple18, ID_RADIOBOX_CODEGEN, _("Security Code Generation"), wxDefaultPosition, wxDefaultSize, itemRadioBox32Strings, 1, wxRA_SPECIFY_ROWS );
    itemRadioBox32->SetSelection(0);
    itemBoxSizer19->Add(itemRadioBox32, 0, wxGROW|wxALL, 5);

////@end WizardPageKeyInfo content construction
}


/*
 * Should we show tooltips?
 */

bool WizardPageKeyInfo::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPageKeyInfo::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPageKeyInfo bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPageKeyInfo bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPageKeyInfo::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPageKeyInfo icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPageKeyInfo icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEMO
 */

void WizardPagePasswd::OnButtonDemoClick( wxCommandEvent& WXUNUSED(event) )
{
    wxTextCtrl *lv1pw1 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV1PW1), wxTextCtrl);
    wxTextCtrl *lv1pw2 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV1PW2), wxTextCtrl);
    wxTextCtrl *lv2pw1 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV2PW1), wxTextCtrl);
    wxTextCtrl *lv2pw2 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV2PW2), wxTextCtrl);
    
    if (lv1pw1 && lv1pw2 && lv2pw1 && lv2pw2)
    {
        lv1pw1->ChangeValue(wxT("C44C"));
        lv1pw2->ChangeValue(wxT("C8F8"));
        lv2pw1->ChangeValue(wxT("0799"));
        lv2pw2->ChangeValue(wxT("C43B"));
    }
}




/*
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE_PASSWD
 */

void WizardPagePasswd::OnPasswdPageChanging( wxWizardEvent& event )
{
    wxTextCtrl *lv1pw1, *lv1pw2, *lv2pw1, *lv2pw2;
    unsigned short &basicPW1 = wxGetApp().basicPW1;
    unsigned short &basicPW2 = wxGetApp().basicPW2;
    unsigned short &advPW1 = wxGetApp().advPW1;
    unsigned short &advPW2 = wxGetApp().advPW2;
    unsigned short result, dontCareShort;
    unsigned long &hwID = wxGetApp().hwID;
    unsigned long longTemp, dontCareLong;
    unsigned char buffer[1000];
    bool isOk = false;

    if (event.GetDirection())
    {
        lv1pw1 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV1PW1), wxTextCtrl);
        lv1pw2 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV1PW2), wxTextCtrl);
        lv2pw1 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV2PW1), wxTextCtrl);
        lv2pw2 = wxDynamicCast(FindWindow(ID_TEXTCTRL_LV2PW2), wxTextCtrl);
        
        if (!lv1pw1 || !lv1pw2 || !lv2pw1 || !lv2pw2)
        {
            event.Veto();
            return;
        }
        
        lv1pw1->GetValue().ToLong((long *)&longTemp, 16);
        basicPW1 = (unsigned short)longTemp;
        lv1pw2->GetValue().ToLong((long *)&longTemp, 16);
        basicPW2 = (unsigned short)longTemp;
        lv2pw1->GetValue().ToLong((long *)&longTemp, 16);
        advPW1 = (unsigned short)longTemp;
        lv2pw2->GetValue().ToLong((long *)&longTemp, 16);
        advPW2 = (unsigned short)longTemp;
        
        result = Rockey(RY_FIND, &dontCareShort, &hwID, &dontCareLong, &basicPW1, &basicPW2, &advPW1, &advPW2, buffer);
        
        switch (result)
        {
        case ERR_SUCCESS:
            isOk = true;
            break;
        case ERR_NO_ROCKEY:
            wxLogMessage(wxT("No Rockey dongle is found!"));
            break;
        case ERR_INVALID_PASSWORD:
            wxLogMessage(wxT("Invalid password!"));
            break;
        default:
            break;
        }
        
        if (!isOk)
        {
            event.Veto();
            return;
        }
    }
}



/*
 * WizardPageDone type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPageDone, wxWizardPageSimple )


/*
 * WizardPageDone event table definition
 */

BEGIN_EVENT_TABLE( WizardPageDone, wxWizardPageSimple )

////@begin WizardPageDone event table entries
////@end WizardPageDone event table entries

END_EVENT_TABLE()


/*
 * WizardPageDone constructors
 */

WizardPageDone::WizardPageDone()
{
    Init();
}

WizardPageDone::WizardPageDone( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*
 * WizardPageDone creator
 */

bool WizardPageDone::Create( wxWizard* parent )
{
////@begin WizardPageDone creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPageDone creation
    return true;
}


/*
 * WizardPageDone destructor
 */

WizardPageDone::~WizardPageDone()
{
////@begin WizardPageDone destruction
////@end WizardPageDone destruction
}


/*
 * Member initialisation
 */

void WizardPageDone::Init()
{
////@begin WizardPageDone member initialisation
////@end WizardPageDone member initialisation
}


/*
 * Control creation for WizardPageDone
 */

void WizardPageDone::CreateControls()
{    
////@begin WizardPageDone content construction
    WizardPageDone* itemWizardPageSimple33 = this;

    wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple33->SetSizer(itemBoxSizer34);

    wxStaticText* itemStaticText35 = new wxStaticText( itemWizardPageSimple33, wxID_STATIC, _("Congratulations"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText35->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer34->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemWizardPageSimple33, wxID_STATIC, _("Your key configuration has done."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer34->Add(itemStaticText36, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPageDone content construction
}


/*
 * Should we show tooltips?
 */

bool WizardPageDone::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPageDone::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPageDone bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPageDone bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPageDone::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPageDone icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPageDone icon retrieval
}


/*
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_KEYINFO
 */

void WizardPageKeyInfo::OnKeyInfoPageChanged( wxWizardEvent& event )
{
    wxTextCtrl *hwIDText = NULL, *swIDText = NULL, *nameText = NULL, *contactText = NULL;
    unsigned long &hwID = wxGetApp().hwID;
    unsigned short &rockey = wxGetApp().rockey;
    unsigned short &basicPW1 = wxGetApp().basicPW1;
    unsigned short &basicPW2 = wxGetApp().basicPW2;
    unsigned short &advPW1 = wxGetApp().advPW1;
    unsigned short &advPW2 = wxGetApp().advPW2;
    unsigned long swID, dontCareLong, longTemp = 0;
    unsigned short result, pos, len, dontCareShort;
    unsigned char buffer[1000];
    wxString lenStr, totalData, nameData, contactData;
    size_t delimitPos1, delimitPos2;

    if (event.GetDirection())
    {
        hwIDText = wxDynamicCast(FindWindow(ID_TEXTCTRL_HWID), wxTextCtrl);
        if (hwIDText)
            hwIDText->ChangeValue(wxString::Format(wxT("%08lX"), hwID));

        result = Rockey(RY_OPEN, &rockey, &hwID, &dontCareLong, &basicPW1, &basicPW2, &advPW1, &advPW2, buffer);

        swIDText = wxDynamicCast(FindWindow(ID_TEXTCTRL_SWID), wxTextCtrl);
        if (!result && swIDText)
        {
            result = Rockey(RY_READ_USERID, &rockey, &swID, &dontCareLong, &dontCareShort, &dontCareShort, &dontCareShort, &dontCareShort, buffer);
            if (!result)
                swIDText->ChangeValue(wxString::Format(wxT("%lX"), swID));

            /* read user data length */
            pos = 500;
            len = 3;
            result = Rockey(RY_READ, &rockey, &dontCareLong, &dontCareLong, &pos, &len, &dontCareShort, &dontCareShort, buffer);
            if (!result)
                lenStr = wxString::From8BitData((const char *)&buffer[0], 3);
            else
                goto data_invalid;

            /* read user data */
            lenStr.ToULong(&longTemp);
            if (longTemp)
            {
                pos = 500 + ((longTemp < 10) ? 2 : ((longTemp < 100) ? 3 : 4));
                len = longTemp;
                result = Rockey(RY_READ, &rockey, &dontCareLong, &dontCareLong, &pos, &len, &dontCareShort, &dontCareShort, buffer);
                if (!result)
                {
                    totalData = wxString::From8BitData((const char *)&buffer[0], len);
                    delimitPos2 = totalData.find_last_of(' '); // hash start pos
                    delimitPos1 = totalData.find_last_of(' ', delimitPos2 - 1); // contact start pos
                    nameData = totalData.Mid(0, delimitPos1);
                    contactData = totalData.Mid(delimitPos1 + 1, delimitPos2 - delimitPos1 - 1);
                    nameText = wxDynamicCast(FindWindow(ID_TEXTCTRL_USER), wxTextCtrl);
                    contactText = wxDynamicCast(FindWindow(ID_TEXTCTRL_CONTACT), wxTextCtrl);
                    if (!nameData.empty() && nameText)
                        nameText->ChangeValue(nameData);
                    if (!contactData.empty() && contactText)
                        contactText->ChangeValue(contactData);
                }
            }
        }
        
data_invalid:
        result = Rockey(RY_CLOSE, &rockey, &dontCareLong, &dontCareLong, &dontCareShort, &dontCareShort, &dontCareShort, &dontCareShort, buffer);
    }
}


/*
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE_KEYINFO
 */

void WizardPageKeyInfo::OnKeyInfoPageChanging( wxWizardEvent& event )
{
    wxTextCtrl *swIDText = NULL, *nameText = NULL, *contactText = NULL;
    unsigned long &hwID = wxGetApp().hwID;
    unsigned short &rockey = wxGetApp().rockey;
    unsigned short &basicPW1 = wxGetApp().basicPW1;
    unsigned short &basicPW2 = wxGetApp().basicPW2;
    unsigned short &advPW1 = wxGetApp().advPW1;
    unsigned short &advPW2 = wxGetApp().advPW2;
    unsigned short result, pos, len, dontCareShort;
    unsigned long dontCareLong, longTemp = 0, swID;
    unsigned char buffer[1000];
    wxString userData, totalData, hashData;

    if (event.GetDirection())
    {
        result = Rockey(RY_OPEN, &rockey, &hwID, &dontCareLong, &basicPW1, &basicPW2, &advPW1, &advPW2, buffer);
        
        nameText = wxDynamicCast(FindWindow(ID_TEXTCTRL_USER), wxTextCtrl);
        contactText = wxDynamicCast(FindWindow(ID_TEXTCTRL_CONTACT), wxTextCtrl);
        swIDText = wxDynamicCast(FindWindow(ID_TEXTCTRL_SWID), wxTextCtrl);

        /* validate data */
        if (!nameText || nameText->GetValue().empty())
        {
            wxLogError(wxT("Invalid name field"));
            event.Veto();
            goto data_invalid;
        }
        else if (!contactText || contactText->GetValue().empty())
        {
            wxLogError(wxT("Invalid contact field"));
            event.Veto();
            goto data_invalid;
        }
        else if (!swIDText || !swIDText->GetValue().ToULong(&longTemp, 16))
        {
            wxLogError(wxT("Invalid user id field"));
            event.Veto();
            goto data_invalid;
        }

        /* write user id */
        swID = longTemp;
        result = Rockey(RY_WRITE_USERID, &rockey, &swID, &dontCareLong, &dontCareShort, &dontCareShort, &dontCareShort, &dontCareShort, buffer);

        /* write user data */
        userData = nameText->GetValue() + wxT(" ") + contactText->GetValue();
        hashData = wxMD5::GetDigest(userData);
        userData << wxT(" ") << hashData;
        len = userData.length();
        totalData = wxString::Format(wxT("%d "), len) + userData;
        len = totalData.length();
        pos = 500;
        memcpy(buffer, totalData.To8BitData(), len);
        result = Rockey(RY_WRITE, &rockey, &dontCareLong, &dontCareLong, &pos, &len, &dontCareShort, &dontCareShort, buffer);
        
data_invalid:
        result = Rockey(RY_CLOSE, &rockey, &dontCareLong, &dontCareLong, &dontCareShort, &dontCareShort, &dontCareShort, &dontCareShort, buffer);
    }
}

