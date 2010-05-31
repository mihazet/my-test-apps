/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyApp.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     23/05/2010 18:35:34
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYKEYAPP_H_
#define _MYKEYAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "MyKeyWizard.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * MyKeyApp class declaration
 */

class MyKeyApp: public wxApp
{    
    DECLARE_CLASS( MyKeyApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    MyKeyApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin MyKeyApp event handler declarations

////@end MyKeyApp event handler declarations

////@begin MyKeyApp member function declarations

////@end MyKeyApp member function declarations

////@begin MyKeyApp member variables
public:
	unsigned short basicPW1;
	unsigned short basicPW2;
	unsigned short advPW1;
	unsigned short advPW2;
	unsigned long hwID;
////@end MyKeyApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(MyKeyApp)
////@end declare app

#endif
    // _MYKEYAPP_H_
