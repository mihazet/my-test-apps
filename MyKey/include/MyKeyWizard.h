/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyWizard.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     23/05/2010 20:28:00
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYKEYWIZARD_H_
#define _MYKEYWIZARD_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class WizardPage;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MYKEYWIZARD 10000
#define ID_WIZARDPAGE_WELCOME 10001
#define SYMBOL_MYKEYWIZARD_IDNAME ID_MYKEYWIZARD
////@end control identifiers


/*!
 * MyKeyWizard class declaration
 */

class MyKeyWizard: public wxWizard
{    
    DECLARE_DYNAMIC_CLASS( MyKeyWizard )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MyKeyWizard();
    MyKeyWizard( wxWindow* parent, wxWindowID id = SYMBOL_MYKEYWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYKEYWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Destructor
    ~MyKeyWizard();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MyKeyWizard event handler declarations

////@end MyKeyWizard event handler declarations

////@begin MyKeyWizard member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MyKeyWizard member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MyKeyWizard member variables
////@end MyKeyWizard member variables
};

/*!
 * WizardPage class declaration
 */

class WizardPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage();

    WizardPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage event handler declarations

////@end WizardPage event handler declarations

////@begin WizardPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage member variables
////@end WizardPage member variables
};

#endif
    // _MYKEYWIZARD_H_
