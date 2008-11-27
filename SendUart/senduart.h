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
#include "wx/grid.h"
#include "wx/statline.h"
#include "wx/filepicker.h"
////@end includes
#include "serport.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SENDUART_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX
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

    /// scan available serial port
    void ScanPort(void);

    /// Check if we're in opened state
    bool IsOpened(void);

    /// Read command list from active group
    void LoadCommand(void);

    /// Read application configuration
    void LoadAppConfig(void);

////@begin SendUart event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID_1
    void OnButtonIdClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID_NEXT
    void OnButtonIdNextClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_FILECTRL_USER_FILE_SAVE
    void OnFilectrlUserFileSaveUpdate( wxUpdateUIEvent& event );

    /// wxEVT_FILEPICKER_CHANGED event handler for ID_FILECTRL_FILE_LOCATION
    void OnFileLocationChanged( wxFileDirPickerEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_FILECTRL_FILE_LOCATION
    void OnFilectrlFileLocationUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONNECT
    void OnButtonConnectClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_BUTTON_CONNECT
    void OnButtonConnectUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DISCONNECT
    void OnButtonDisconnectClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_BUTTON_DISCONNECT
    void OnButtonDisconnectUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SCAN_PORT
    void OnButtonScanPortClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHOICE_PORT
    void OnChoicePortUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_BAUD
    void OnChoiceBaudSelected( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHOICE_CHAR_SIZE
    void OnChoiceCharSizeUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHOICE_PARITY
    void OnChoiceParityUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHOICE_STOP_BITS
    void OnChoiceStopBitsUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TRANSMIT
    void OnButtonTransmitClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_BUTTON_TRANSMIT
    void OnButtonTransmitUpdate( wxUpdateUIEvent& event );

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
private:
    unsigned char * m_pBuffer;
    size_t m_bufferSize;
    wxSerialPort m_com;
    wxSerialPort_DCS m_serialDCS;
    /// Control identifiers
    enum {
        ID_SENDUART = 10022,
        ID_PANEL_GENERATION = 10002,
        ID_GRID_CMD_LIST = 10006,
        ID_PANEL_CMD_INFO = 10027,
        wxID_STATIC_CMD_SYNTAX = 10028,
        wxID_STATIC_VALUE1_MAX = 10032,
        wxID_STATIC_VALUE1_MIN = 10033,
        wxID_STATIC_VALUE2_MAX = 10034,
        wxID_STATIC_VALUE2_MIN = 10038,
        ID_BUTTON_ID_1 = 10023,
        ID_BUTTON_ID_2 = 10024,
        ID_BUTTON_ID_3 = 10025,
        ID_BUTTON_ID_4 = 10029,
        ID_BUTTON_ID_5 = 10030,
        ID_BUTTON_ID_6 = 10031,
        ID_BUTTON_ID_7 = 10035,
        ID_BUTTON_ID_8 = 10036,
        ID_BUTTON_ID_9 = 10037,
        ID_BUTTON_ID_0 = 10041,
        ID_BUTTON_ID_STAR = 10042,
        ID_BUTTON_ID_NEXT = 10026,
        wxID_STATIC_ID0 = 10043,
        wxID_STATIC_ID1 = 10046,
        ID_BUTTON = 10048,
        ID_BUTTON1 = 10049,
        ID_TEXTCTRL_CMD_COUNT = 10051,
        ID_BUTTON2 = 10050,
        ID_TEXTCTRL = 10053,
        ID_BUTTON3 = 10052,
        ID_GRID_GEN_DATA = 10007,
        ID_RADIOBUTTON_GEN_TO_INTERNAL_BUFFER = 10039,
        ID_RADIOBUTTON_GEN_TO_USER_FILE = 10040,
        ID_FILECTRL_USER_FILE_SAVE = 10044,
        ID_PANEL_TRANSMISSION = 10003,
        ID_RADIOBUTTON_TRANSMIT_INTERNAL_BUFFER = 10045,
        ID_RADIOBUTTON_TRANSMIT_USER_FILE = 10047,
        ID_FILECTRL_FILE_LOCATION = 10000,
        ID_GRID_BYTE_COUNTER = 10004,
        wxID_STATIC_FILE_SIZE = 10015,
        wxID_STATIC_MU_BYTE = 10016,
        wxID_STATIC_MU_CNT = 10017,
        wxID_STATIC_LU_BYTE = 10018,
        wxID_STATIC_LU_CNT = 10019,
        ID_BUTTON_CONNECT = 10001,
        ID_BUTTON_DISCONNECT = 10005,
        ID_BUTTON_SCAN_PORT = 10013,
        ID_CHOICE_PORT = 10008,
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
