/////////////////////////////////////////////////////////////////////////////
// Name:        tpaaapp.h
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/15/2008 5:20:47 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TPAAAPP_H_
#define _TPAAAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "../build/tpaaframe.h"
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
 * TPAAApp class declaration
 */

class TPAAApp: public wxApp
{    
    DECLARE_CLASS( TPAAApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    TPAAApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin TPAAApp event handler declarations

////@end TPAAApp event handler declarations

////@begin TPAAApp member function declarations

////@end TPAAApp member function declarations

////@begin TPAAApp member variables
////@end TPAAApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(TPAAApp)
////@end declare app

#endif
    // _TPAAAPP_H_
