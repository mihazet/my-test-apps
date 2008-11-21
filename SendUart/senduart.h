/////////////////////////////////////////////////////////////////////////////
// Name:        senduart.h
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/21/2008 2:10:40 PM
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SENDUART_H_
#define _SENDUART_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/grid.h"
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
#define SYMBOL_SENDUART_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SENDUART_TITLE _("SendUart")
#define SYMBOL_SENDUART_IDNAME ID_SENDUART
#define SYMBOL_SENDUART_SIZE wxDefaultSize
#define SYMBOL_SENDUART_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SendUart class declaration
 */

class SendUart: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SendUart )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SendUart();
    SendUart( wxWindow* parent, wxWindowID id = SYMBOL_SENDUART_IDNAME, const wxString& caption = SYMBOL_SENDUART_TITLE, const wxPoint& pos = SYMBOL_SENDUART_POSITION, const wxSize& size = SYMBOL_SENDUART_SIZE, long style = SYMBOL_SENDUART_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SENDUART_IDNAME, const wxString& caption = SYMBOL_SENDUART_TITLE, const wxPoint& pos = SYMBOL_SENDUART_POSITION, const wxSize& size = SYMBOL_SENDUART_SIZE, long style = SYMBOL_SENDUART_STYLE );

    /// Destructor
    ~SendUart();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SendUart event handler declarations

////@end SendUart event handler declarations

////@begin SendUart member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SendUart member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SendUart member variables
    /// Control identifiers
    enum {
        ID_SENDUART = 10000,
        ID_NOTEBOOK = 10001,
        ID_PANEL_GENERATION = 10002,
        ID_PANEL_TRANSMISSION = 10003,
        ID_BUTTON_FILE_LOCATION = 10005,
        ID_TEXTCTRL = 10006,
        ID_BUTTON = 10007,
        ID_GRID_BYTE_COUNTER = 10004,
        ID_CHOICE = 10008,
        ID_BUTTON1 = 10013,
        ID_CHOICE1 = 10009,
        ID_CHOICE2 = 10010,
        ID_CHOICE3 = 10011,
        ID_CHOICE4 = 10012,
        ID_BUTTON_TRANSMIT = 10014
    };
////@end SendUart member variables
};

#endif
    // _SENDUART_H_
