/////////////////////////////////////////////////////////////////////////////
// Name:        susimapp.h
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/17/2008 6:53:35 PM
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SUSIMAPP_H_
#define _SUSIMAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
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
 * SUsimApp class declaration
 */

class SUsimApp: public wxApp
{    
    DECLARE_CLASS( SUsimApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    SUsimApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin SUsimApp event handler declarations
////@end SUsimApp event handler declarations

////@begin SUsimApp member function declarations
////@end SUsimApp member function declarations

////@begin SUsimApp member variables
////@end SUsimApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(SUsimApp)
////@end declare app

#endif
    // _SUSIMAPP_H_
