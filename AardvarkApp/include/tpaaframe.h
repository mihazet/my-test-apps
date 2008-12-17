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
#include "wx/grid.h"
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
#define SYMBOL_TPAAFRAME_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TPAAFRAME_TITLE _("Total Phase Aardvark I2C/SPI Control Center fork")
#define SYMBOL_TPAAFRAME_IDNAME ID_TPAAFRAME
#define SYMBOL_TPAAFRAME_SIZE wxSize(860, 640)
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
    /// Control identifiers
    enum {
        ID_TPAAFRAME = 10004,
        ID_MENUITEM_FILE_CLOSE = 10006,
        ID_MENUITEM_HELP_ABOUT = 10008,
        ID_STATUSBAR = 10007,
        ID_PANEL_I2C = 10016,
        ID_BUTTON = 10017,
        ID_CHOICE = 10018,
        ID_TEXTCTRL = 10019,
        ID_BUTTON1 = 10020,
        ID_CHECKBOX = 10021,
        ID_CHECKBOX1 = 10022,
        ID_GRID_RWDATA = 10023,
        ID_SPLITTERWINDOW = 10009,
        ID_PANEL_CONFIG = 10010,
        ID_BUTTON_CONFIG_AA_DLG = 10012,
        ID_PANEL_LOG = 10011,
        ID_LOGWIN = 10013,
        ID_BUTTON_LOG_CLEAR = 10014,
        ID_BUTTON_LOG_SAVE = 10015
    };
////@end TPAAFrame member variables
};

#endif
    // _TPAAFRAME_H_
