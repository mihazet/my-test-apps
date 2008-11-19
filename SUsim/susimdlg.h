/////////////////////////////////////////////////////////////////////////////
// Name:        susimdlg.h
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/19/2008 2:46:43 PM
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SUSIMDLG_H_
#define _SUSIMDLG_H_


/*!
 * Includes
 */

#include "wx/socket.h"
////@begin includes
#include "wx/tglbtn.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SUSIMDLG 10000
#define ID_TEXTCTRL1 10004
#define ID_LISTBOX 10003
#define ID_LISTBOX1 10005
#define ID_TOGGLEBUTTON 10001
#define ID_TEXTCTRL 10002
#define SYMBOL_SUSIMDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SUSIMDLG_TITLE _("Software Update SimTest")
#define SYMBOL_SUSIMDLG_IDNAME ID_SUSIMDLG
#define SYMBOL_SUSIMDLG_SIZE wxSize(400, 300)
#define SYMBOL_SUSIMDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SUSimDlg class declaration
 */

class SUSimDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SUSimDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SUSimDlg();
    SUSimDlg( wxWindow* parent, wxWindowID id = SYMBOL_SUSIMDLG_IDNAME, const wxString& caption = SYMBOL_SUSIMDLG_TITLE, const wxPoint& pos = SYMBOL_SUSIMDLG_POSITION, const wxSize& size = SYMBOL_SUSIMDLG_SIZE, long style = SYMBOL_SUSIMDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SUSIMDLG_IDNAME, const wxString& caption = SYMBOL_SUSIMDLG_TITLE, const wxPoint& pos = SYMBOL_SUSIMDLG_POSITION, const wxSize& size = SYMBOL_SUSIMDLG_SIZE, long style = SYMBOL_SUSIMDLG_STYLE );

    /// Destructor
    ~SUSimDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SUSimDlg event handler declarations

////@end SUSimDlg event handler declarations

////@begin SUSimDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SUSimDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SUSimDlg member variables
    wxTextCtrl* m_boardNameTextCtrl;
    wxListBox* m_response;
    wxListBox* m_interface;
    wxToggleButton* m_enableBtn;
private:
    wxDatagramSocket * m_udpSocket;
////@end SUSimDlg member variables
};

#endif
    // _SUSIMDLG_H_
