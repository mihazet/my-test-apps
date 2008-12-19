/////////////////////////////////////////////////////////////////////////////
// Name:        i2cctrlpanel.h
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/18/2008 2:11:35 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _I2CCTRLPANEL_H_
#define _I2CCTRLPANEL_H_


/*!
 * Includes
 */

////@begin includes
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
#define ID_I2CCTRLPANEL 10005
#define ID_BUTTON 10017
#define ID_CHOICE 10018
#define ID_TEXTCTRL 10019
#define ID_BUTTON1 10020
#define ID_CHECKBOX 10021
#define ID_CHECKBOX1 10022
#define ID_GRID_RWDATA 10023
#define SYMBOL_I2CCTRLPANEL_STYLE wxSIMPLE_BORDER|wxTAB_TRAVERSAL
#define SYMBOL_I2CCTRLPANEL_TITLE _("I2C Control Panel")
#define SYMBOL_I2CCTRLPANEL_IDNAME ID_I2CCTRLPANEL
#define SYMBOL_I2CCTRLPANEL_SIZE wxDefaultSize
#define SYMBOL_I2CCTRLPANEL_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * i2cCtrlPanel class declaration
 */

class i2cCtrlPanel: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( i2cCtrlPanel )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    i2cCtrlPanel();
    i2cCtrlPanel( wxWindow* parent, wxWindowID id = SYMBOL_I2CCTRLPANEL_IDNAME, const wxPoint& pos = SYMBOL_I2CCTRLPANEL_POSITION, const wxSize& size = SYMBOL_I2CCTRLPANEL_SIZE, long style = SYMBOL_I2CCTRLPANEL_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_I2CCTRLPANEL_IDNAME, const wxPoint& pos = SYMBOL_I2CCTRLPANEL_POSITION, const wxSize& size = SYMBOL_I2CCTRLPANEL_SIZE, long style = SYMBOL_I2CCTRLPANEL_STYLE );

    /// Destructor
    ~i2cCtrlPanel();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin i2cCtrlPanel event handler declarations

////@end i2cCtrlPanel event handler declarations

////@begin i2cCtrlPanel member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end i2cCtrlPanel member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin i2cCtrlPanel member variables
////@end i2cCtrlPanel member variables
};

#endif
    // _I2CCTRLPANEL_H_
