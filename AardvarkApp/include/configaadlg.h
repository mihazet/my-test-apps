/////////////////////////////////////////////////////////////////////////////
// Name:        configaadlg.h
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/15/2008 7:04:16 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIGAADLG_H_
#define _CONFIGAADLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
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
#define ID_CONFIGAADLG 10000
#define ID_LISTCTRL_ADAPTER_LIST 10001
#define ID_RADIOBOX 10002
#define ID_BUTTON_REFRESH_ADAPTERLIST 10003
#define SYMBOL_CONFIGAADLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CONFIGAADLG_TITLE _("Configure Aardvark Adapter")
#define SYMBOL_CONFIGAADLG_IDNAME ID_CONFIGAADLG
#define SYMBOL_CONFIGAADLG_SIZE wxDefaultSize
#define SYMBOL_CONFIGAADLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ConfigAADlg class declaration
 */

class ConfigAADlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ConfigAADlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ConfigAADlg();
    ConfigAADlg( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGAADLG_IDNAME, const wxString& caption = SYMBOL_CONFIGAADLG_TITLE, const wxPoint& pos = SYMBOL_CONFIGAADLG_POSITION, const wxSize& size = SYMBOL_CONFIGAADLG_SIZE, long style = SYMBOL_CONFIGAADLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGAADLG_IDNAME, const wxString& caption = SYMBOL_CONFIGAADLG_TITLE, const wxPoint& pos = SYMBOL_CONFIGAADLG_POSITION, const wxSize& size = SYMBOL_CONFIGAADLG_SIZE, long style = SYMBOL_CONFIGAADLG_STYLE );

    /// Destructor
    ~ConfigAADlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    /// Modify the GUI controls
    void ModifyControls(void);

    /// Find Devices
    void FindDevices(void);

////@begin ConfigAADlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_REFRESH_ADAPTERLIST
    void OnButtonRefreshAdapterlistClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOkUpdate( wxUpdateUIEvent& event );

////@end ConfigAADlg event handler declarations

////@begin ConfigAADlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ConfigAADlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ConfigAADlg member variables
////@end ConfigAADlg member variables
};

#endif
    // _CONFIGAADLG_H_
