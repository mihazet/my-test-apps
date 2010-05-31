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

#include "MyKeyApp.h"
#include "MyKeyWizard.h"
#include "Rockey4_ND_32.h"

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

    WizardPageKeyInfo* itemWizardPageSimple16 = new WizardPageKeyInfo( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple16);

    WizardPageDone* itemWizardPageSimple31 = new WizardPageDone( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple31);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple4);
    lastPage = itemWizardPageSimple4;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple16);
    lastPage = itemWizardPageSimple16;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple31);
    lastPage = itemWizardPageSimple31;
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
    itemGridBagSizer5->Add(itemTextCtrl8, wxGBPosition(2, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level1 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText9, wxGBPosition(2, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV1PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl10->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl10, wxGBPosition(2, 4), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level2 PW1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText11, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV2PW1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl12->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl12, wxGBPosition(3, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemWizardPageSimple4, wxID_STATIC, _("Level2 PW2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer5->Add(itemStaticText13, wxGBPosition(3, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl14 = new wxTextCtrl( itemWizardPageSimple4, ID_TEXTCTRL_LV2PW2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxTE_PASSWORD );
    itemTextCtrl14->SetMaxLength(4);
    itemGridBagSizer5->Add(itemTextCtrl14, wxGBPosition(3, 4), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

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
    WizardPageKeyInfo* itemWizardPageSimple16 = this;

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple16->SetSizer(itemBoxSizer17);

    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(itemWizardPageSimple16, wxID_ANY, _("Dongle Info"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
    itemBoxSizer17->Add(itemStaticBoxSizer18, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer19 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer19->AddGrowableCol(1);
    itemStaticBoxSizer18->Add(itemFlexGridSizer19, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemWizardPageSimple16, wxID_STATIC, _("Hardware ID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer19->Add(itemStaticText20, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl21 = new wxTextCtrl( itemWizardPageSimple16, ID_TEXTCTRL_HWID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemTextCtrl21->Enable(false);
    itemFlexGridSizer19->Add(itemTextCtrl21, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemWizardPageSimple16, wxID_STATIC, _("Software ID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer19->Add(itemStaticText22, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl23 = new wxTextCtrl( itemWizardPageSimple16, ID_TEXTCTRL_SWID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer19->Add(itemTextCtrl23, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemWizardPageSimple16, wxID_ANY, _("User Info"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxVERTICAL);
    itemBoxSizer17->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer25 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer25->AddGrowableCol(1);
    itemStaticBoxSizer24->Add(itemFlexGridSizer25, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText26 = new wxStaticText( itemWizardPageSimple16, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer25->Add(itemStaticText26, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl27 = new wxTextCtrl( itemWizardPageSimple16, ID_TEXTCTRL_USER, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer25->Add(itemTextCtrl27, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText28 = new wxStaticText( itemWizardPageSimple16, wxID_STATIC, _("Contact"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer25->Add(itemStaticText28, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl29 = new wxTextCtrl( itemWizardPageSimple16, ID_TEXTCTRL_CONTACT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer25->Add(itemTextCtrl29, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemRadioBox30Strings;
    itemRadioBox30Strings.Add(_("&MD5"));
    itemRadioBox30Strings.Add(_("&SHA1"));
    wxRadioBox* itemRadioBox30 = new wxRadioBox( itemWizardPageSimple16, ID_RADIOBOX_CODEGEN, _("Security Code Generation"), wxDefaultPosition, wxDefaultSize, itemRadioBox30Strings, 1, wxRA_SPECIFY_ROWS );
    itemRadioBox30->SetSelection(0);
    itemBoxSizer17->Add(itemRadioBox30, 0, wxGROW|wxALL, 5);

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
    WizardPageDone* itemWizardPageSimple31 = this;

    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple31->SetSizer(itemBoxSizer32);

    wxStaticText* itemStaticText33 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("Congratulations"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText33->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer32->Add(itemStaticText33, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("Your key configuration has done."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(itemStaticText34, 0, wxALIGN_LEFT|wxALL, 5);

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
    wxTextCtrl *hwIDText;
    unsigned long &hwID = wxGetApp().hwID;

    if (event.GetDirection())
    {
        hwIDText = wxDynamicCast(FindWindow(ID_TEXTCTRL_HWID), wxTextCtrl);
        if (hwIDText)
        {
            hwIDText->ChangeValue(wxString::Format(wxT("0x%08lX"), hwID));
        }
    }
}

