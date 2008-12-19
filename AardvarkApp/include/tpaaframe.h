/////////////////////////////////////////////////////////////////////////////
// Name:        tpaaframe.h
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/17/2008 2:16:56 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TPAAFRAME_H_
#define _TPAAFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/statusbr.h"
#include "wx/splitter.h"
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
#define ID_TPAAFRAME 10004
#define ID_MENUITEM_FILE_CLOSE 10007
#define ID_MENUITEM_HELP_ABOUT 10008
#define ID_STATUSBAR 10009
#define ID_PANEL 10010
#define ID_SPLITTERWINDOW 10011
#define ID_PANEL_CONFIG 10012
#define ID_BUTTON_CONFIG_AA_DLG 10013
#define ID_PANEL_LOG 10014
#define ID_LOGWIN 10015
#define ID_BUTTON_LOG_CLEAR 10016
#define ID_BUTTON_LOG_SAVE 10024
#define SYMBOL_TPAAFRAME_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TPAAFRAME_TITLE _("Total Phase Aardvark I2C/SPI Control Center fork")
#define SYMBOL_TPAAFRAME_IDNAME ID_TPAAFRAME
#define SYMBOL_TPAAFRAME_SIZE wxSize(960, 740)
#define SYMBOL_TPAAFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * TPAAFrame class declaration
 */

class TPAAFrame: public wxFrame
{    
    DECLARE_CLASS( TPAAFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    TPAAFrame();
    TPAAFrame( wxWindow* parent, wxWindowID id = SYMBOL_TPAAFRAME_IDNAME, const wxString& caption = SYMBOL_TPAAFRAME_TITLE, const wxPoint& pos = SYMBOL_TPAAFRAME_POSITION, const wxSize& size = SYMBOL_TPAAFRAME_SIZE, long style = SYMBOL_TPAAFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TPAAFRAME_IDNAME, const wxString& caption = SYMBOL_TPAAFRAME_TITLE, const wxPoint& pos = SYMBOL_TPAAFRAME_POSITION, const wxSize& size = SYMBOL_TPAAFRAME_SIZE, long style = SYMBOL_TPAAFRAME_STYLE );

    /// Destructor
    ~TPAAFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void ModifyControls(void);

////@begin TPAAFrame event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONFIG_AA_DLG
    void OnButtonConfigAaDlgClick( wxCommandEvent& event );

////@end TPAAFrame event handler declarations

////@begin TPAAFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end TPAAFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin TPAAFrame member variables
////@end TPAAFrame member variables
};

#endif
    // _TPAAFRAME_H_
