/////////////////////////////////////////////////////////////////////////////
// Name:        senduart.h
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     23/11/2008 21:21:49
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
#include "wx/propdlg.h"
#include "wx/filepicker.h"
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
#define SYMBOL_SENDUART_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SENDUART_TITLE _("SendUart")
#define SYMBOL_SENDUART_IDNAME ID_SENDUART
#define SYMBOL_SENDUART_SIZE wxDefaultSize
#define SYMBOL_SENDUART_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SendUart class declaration
 */

class SendUart: public wxPropertySheetDialog
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

    /// Replace byte counter grid row and column label
    void ReplaceRowColLabel(void);

////@begin SendUart event handler declarations

    /// wxEVT_FILEPICKER_CHANGED event handler for ID_FILECTRL_FILE_LOCATION
    void OnFileLocationChanged( wxFileDirPickerEvent& event );

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
        ID_SENDUART = 10022,
        ID_PANEL_GENERATION = 10002,
        ID_PANEL_TRANSMISSION = 10003,
        ID_FILECTRL_FILE_LOCATION = 10000,
        ID_GRID_BYTE_COUNTER = 10004,
        wxID_STATIC_FILE_SIZE = 10015,
        wxID_STATIC_MU_BYTE = 10016,
        wxID_STATIC_MU_CNT = 10017,
        wxID_STATIC_LU_BYTE = 10018,
        wxID_STATIC_LU_CNT = 10019,
        ID_CHOICE_PORT = 10008,
        ID_BUTTON_SCAN_PORT = 10013,
        ID_CHOICE_BAUD = 10009,
        ID_CHOICE_CHAR_SIZE = 10010,
        ID_CHOICE_PARITY = 10011,
        ID_CHOICE_STOP_BITS = 10012,
        wxID_STATIC_THEORY_TRANSMIT_TIME = 10020,
        wxID_STATIC_PRATICE_TRANSMIT_TIME = 10021,
        ID_BUTTON_TRANSMIT = 10014
    };
////@end SendUart member variables
};

#endif
    // _SENDUART_H_
