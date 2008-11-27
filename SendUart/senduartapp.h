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
#include "wx/config.h"

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

    /// generate default app config if no config exists
    void GenDefaultAppConfig(void);

    void InitAppConfigForNumeric(wxString prop, long idx, long code, long max, long min);

    void InitAppConfigForPairNumeric(wxString prop, long idx, long code, long max1, long min1, long max2, long min2);

    void InitAppConfigForNumeric2Byte(wxString prop, long idx, long code, long max, long min);

    void InitAppConfigForNumericHex(wxString prop, long idx, long code, long max, long min);

    void GenIfNotExist(const wxString& key, const wxString& defVal);
    
    void GenIfNotExist(const wxString& key, long defVal);

////@begin SendUartApp event handler declarations

////@end SendUartApp event handler declarations

////@begin SendUartApp member function declarations

////@end SendUartApp member function declarations

////@begin SendUartApp member variables
public:
	wxConfig * m_appConfig;
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
