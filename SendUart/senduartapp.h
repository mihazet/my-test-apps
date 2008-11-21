/////////////////////////////////////////////////////////////////////////////
// Name:        senduartapp.h
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/21/2008 2:10:13 PM
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SENDUARTAPP_H_
#define _SENDUARTAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "senduart.h"
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
 * SendUartApp class declaration
 */

class SendUartApp: public wxApp
{    
    DECLARE_CLASS( SendUartApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    SendUartApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin SendUartApp event handler declarations

////@end SendUartApp event handler declarations

////@begin SendUartApp member function declarations

////@end SendUartApp member function declarations

////@begin SendUartApp member variables
////@end SendUartApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(SendUartApp)
////@end declare app

#endif
    // _SENDUARTAPP_H_
