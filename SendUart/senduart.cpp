/////////////////////////////////////////////////////////////////////////////
// Name:        senduart.cpp
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     23/11/2008 21:21:49
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/bookctrl.h"
////@end includes
#include "wx/file.h"
#include "wx/datetime.h"
#include "wx/longlong.h"
#include "wx/config.h"

#include "senduart.h"
#include "senduartapp.h"

////@begin XPM images
////@end XPM images


/*!
 * SendUart type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SendUart, wxPropertySheetDialog )


/*!
 * SendUart event table definition
 */

BEGIN_EVENT_TABLE( SendUart, wxPropertySheetDialog )

////@begin SendUart event table entries
    EVT_BUTTON( ID_BUTTON_ID_1, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_2, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_3, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_4, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_5, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_6, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_7, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_8, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_9, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_0, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_STAR, SendUart::OnButtonIdClick )

    EVT_BUTTON( ID_BUTTON_ID_NEXT, SendUart::OnButtonIdNextClick )

    EVT_FILEPICKER_CHANGED( ID_FILECTRL_FILE_LOCATION, SendUart::OnFileLocationChanged )

    EVT_BUTTON( ID_BUTTON_CONNECT, SendUart::OnButtonConnectClick )
    EVT_UPDATE_UI( ID_BUTTON_CONNECT, SendUart::OnButtonConnectUpdate )

    EVT_BUTTON( ID_BUTTON_DISCONNECT, SendUart::OnButtonDisconnectClick )
    EVT_UPDATE_UI( ID_BUTTON_DISCONNECT, SendUart::OnButtonDisconnectUpdate )

    EVT_BUTTON( ID_BUTTON_SCAN_PORT, SendUart::OnButtonScanPortClick )

    EVT_UPDATE_UI( ID_CHOICE_PORT, SendUart::OnChoicePortUpdate )

    EVT_CHOICE( ID_CHOICE_BAUD, SendUart::OnChoiceBaudSelected )

    EVT_UPDATE_UI( ID_CHOICE_CHAR_SIZE, SendUart::OnChoiceCharSizeUpdate )

    EVT_UPDATE_UI( ID_CHOICE_PARITY, SendUart::OnChoiceParityUpdate )

    EVT_UPDATE_UI( ID_CHOICE_STOP_BITS, SendUart::OnChoiceStopBitsUpdate )

    EVT_BUTTON( ID_BUTTON_TRANSMIT, SendUart::OnButtonTransmitClick )
    EVT_UPDATE_UI( ID_BUTTON_TRANSMIT, SendUart::OnButtonTransmitUpdate )

////@end SendUart event table entries

END_EVENT_TABLE()


/*!
 * SendUart constructors
 */

SendUart::SendUart()
{
    Init();
}

SendUart::SendUart( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * SendUart creator
 */

bool SendUart::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SendUart creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    SetSheetStyle(wxPROPSHEET_DEFAULT);
    wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

    CreateButtons(wxOK|wxCANCEL|wxHELP);
    CreateControls();
    LayoutDialog();
    Centre();
////@end SendUart creation
    return true;
}


/*!
 * SendUart destructor
 */

SendUart::~SendUart()
{
////@begin SendUart destruction
////@end SendUart destruction
    if (m_pBuffer)
        free(m_pBuffer);
    if (IsOpened())
        m_com.Close();
}


/*!
 * Member initialisation
 */

void SendUart::Init()
{
////@begin SendUart member initialisation
    m_pBuffer = NULL;
    m_bufferSize = 0;
////@end SendUart member initialisation
}


/*!
 * Control creation for SendUart
 */

void SendUart::CreateControls()
{    
////@begin SendUart content construction
    SendUart* itemPropertySheetDialog1 = this;

    wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PANEL_GENERATION, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel2->SetSizer(itemBoxSizer3);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Available commands"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL|wxADJUST_MINSIZE, 5);
    wxGrid* itemGrid5 = new wxGrid( itemPanel2, ID_GRID_CMD_LIST, wxDefaultPosition, wxSize(230, 400), wxNO_BORDER|wxVSCROLL );
    itemGrid5->SetDefaultColSize(60);
    itemGrid5->SetDefaultRowSize(18);
    itemGrid5->SetColLabelSize(18);
    itemGrid5->SetRowLabelSize(28);
    itemGrid5->CreateGrid(1, 2, wxGrid::wxGridSelectRows);
    itemStaticBoxSizer4->Add(itemGrid5, 1, wxGROW|wxALL, 5);

    wxPanel* itemPanel6 = new wxPanel( itemPanel2, ID_PANEL_CMD_INFO, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
    itemStaticBoxSizer4->Add(itemPanel6, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
    itemPanel6->SetSizer(itemFlexGridSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel6, wxID_STATIC, _("Value Syntax:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel6, wxID_STATIC_CMD_SYNTAX, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText9, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel6, wxID_STATIC, _("Value1 Max:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel6, wxID_STATIC_VALUE1_MAX, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel6, wxID_STATIC, _("Value1 Min:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText12, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel6, wxID_STATIC_VALUE1_MIN, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel6, wxID_STATIC, _("Value2 Max:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel6, wxID_STATIC_VALUE2_MAX, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText15, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel6, wxID_STATIC, _("Value2 Min:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel6, wxID_STATIC_VALUE2_MIN, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer18, 1, wxALIGN_TOP|wxALL, 5);
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer18->Add(itemBoxSizer19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer20 = new wxFlexGridSizer(4, 3, 0, 0);
    itemBoxSizer19->Add(itemFlexGridSizer20, 0, wxALIGN_CENTER_VERTICAL, 0);
    wxButton* itemButton21 = new wxButton( itemPanel2, ID_BUTTON_ID_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton21, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton22 = new wxButton( itemPanel2, ID_BUTTON_ID_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton22, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton23 = new wxButton( itemPanel2, ID_BUTTON_ID_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton23, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton24 = new wxButton( itemPanel2, ID_BUTTON_ID_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton24, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton25 = new wxButton( itemPanel2, ID_BUTTON_ID_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton25, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton26 = new wxButton( itemPanel2, ID_BUTTON_ID_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton26, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton27 = new wxButton( itemPanel2, ID_BUTTON_ID_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton27, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton28 = new wxButton( itemPanel2, ID_BUTTON_ID_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton28, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton29 = new wxButton( itemPanel2, ID_BUTTON_ID_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton30 = new wxButton( itemPanel2, ID_BUTTON_ID_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton30, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton31 = new wxButton( itemPanel2, ID_BUTTON_ID_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton31, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton32 = new wxButton( itemPanel2, ID_BUTTON_ID_NEXT, _("N"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer20->Add(itemButton32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel2, wxID_STATIC_ID0, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxSIMPLE_BORDER );
    itemStaticText33->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer19->Add(itemStaticText33, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText34 = new wxStaticText( itemPanel2, wxID_STATIC_ID1, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText34->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer19->Add(itemStaticText34, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticBox* itemStaticBoxSizer35Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Generated raw data"));
    wxStaticBoxSizer* itemStaticBoxSizer35 = new wxStaticBoxSizer(itemStaticBoxSizer35Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer35, 0, wxGROW|wxALL, 5);
    wxGrid* itemGrid36 = new wxGrid( itemPanel2, ID_GRID_GEN_DATA, wxDefaultPosition, wxSize(-1, 400), wxNO_BORDER|wxVSCROLL );
    itemGrid36->SetDefaultColSize(20);
    itemGrid36->SetDefaultRowSize(18);
    itemGrid36->SetColLabelSize(18);
    itemGrid36->SetRowLabelSize(40);
    itemGrid36->CreateGrid(1, 16, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer35->Add(itemGrid36, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel37 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxVERTICAL);
    itemPanel37->SetSizer(itemBoxSizer38);

    wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemPanel37, wxID_ANY, _("File Settting"));
    wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxVERTICAL);
    itemBoxSizer38->Add(itemStaticBoxSizer39, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer40, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel37, wxID_STATIC, _("File path:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemStaticText41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFilePickerCtrl* itemFilePickerCtrl42 = new wxFilePickerCtrl( itemPanel37, ID_FILECTRL_FILE_LOCATION, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
    itemBoxSizer40->Add(itemFilePickerCtrl42, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer39->Add(itemBoxSizer43, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid44 = new wxGrid( itemPanel37, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid44->SetDefaultColSize(45);
    itemGrid44->SetDefaultRowSize(18);
    itemGrid44->SetColLabelSize(18);
    itemGrid44->SetRowLabelSize(20);
    itemGrid44->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer43->Add(itemGrid44, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer45Static = new wxStaticBox(itemPanel37, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer45 = new wxStaticBoxSizer(itemStaticBoxSizer45Static, wxVERTICAL);
    itemBoxSizer43->Add(itemStaticBoxSizer45, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer46 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer46->AddGrowableCol(1);
    itemStaticBoxSizer45->Add(itemFlexGridSizer46, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText47 = new wxStaticText( itemPanel37, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText47, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText48 = new wxStaticText( itemPanel37, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText48, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText49 = new wxStaticText( itemPanel37, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText49, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText50 = new wxStaticText( itemPanel37, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText50, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText51 = new wxStaticText( itemPanel37, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText51, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel37, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText52, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText53 = new wxStaticText( itemPanel37, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText53, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel37, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText54, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText55 = new wxStaticText( itemPanel37, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText55, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText56 = new wxStaticText( itemPanel37, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer46->Add(itemStaticText56, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer38->Add(itemBoxSizer57, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer58Static = new wxStaticBox(itemPanel37, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer58 = new wxStaticBoxSizer(itemStaticBoxSizer58Static, wxVERTICAL);
    itemBoxSizer57->Add(itemStaticBoxSizer58, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer58->Add(itemBoxSizer59, 0, wxGROW|wxALL, 0);
    wxButton* itemButton60 = new wxButton( itemPanel37, ID_BUTTON_CONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(itemButton60, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton61 = new wxButton( itemPanel37, ID_BUTTON_DISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(itemButton61, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton62 = new wxButton( itemPanel37, ID_BUTTON_SCAN_PORT, _("Rescan Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(itemButton62, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer63 = new wxFlexGridSizer(0, 2, 0, 5);
    itemFlexGridSizer63->AddGrowableCol(1);
    itemStaticBoxSizer58->Add(itemFlexGridSizer63, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText64 = new wxStaticText( itemPanel37, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText64, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice65Strings;
    wxChoice* itemChoice65 = new wxChoice( itemPanel37, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice65Strings, 0 );
    itemFlexGridSizer63->Add(itemChoice65, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText66 = new wxStaticText( itemPanel37, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText66, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice67Strings;
    itemChoice67Strings.Add(_("115200"));
    itemChoice67Strings.Add(_("57600"));
    itemChoice67Strings.Add(_("38400"));
    itemChoice67Strings.Add(_("19200"));
    itemChoice67Strings.Add(_("9600"));
    wxChoice* itemChoice67 = new wxChoice( itemPanel37, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice67Strings, 0 );
    itemChoice67->SetStringSelection(_("115200"));
    itemFlexGridSizer63->Add(itemChoice67, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText68 = new wxStaticText( itemPanel37, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText68, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice69Strings;
    itemChoice69Strings.Add(_("8"));
    itemChoice69Strings.Add(_("7"));
    itemChoice69Strings.Add(_("6"));
    itemChoice69Strings.Add(_("5"));
    wxChoice* itemChoice69 = new wxChoice( itemPanel37, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice69Strings, 0 );
    itemChoice69->SetStringSelection(_("8"));
    itemFlexGridSizer63->Add(itemChoice69, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText70 = new wxStaticText( itemPanel37, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText70, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice71Strings;
    itemChoice71Strings.Add(_("None"));
    itemChoice71Strings.Add(_("Odd"));
    itemChoice71Strings.Add(_("Even"));
    itemChoice71Strings.Add(_("Mark"));
    itemChoice71Strings.Add(_("Space"));
    wxChoice* itemChoice71 = new wxChoice( itemPanel37, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice71Strings, 0 );
    itemChoice71->SetStringSelection(_("None"));
    itemFlexGridSizer63->Add(itemChoice71, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel37, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice73Strings;
    itemChoice73Strings.Add(_("1"));
    itemChoice73Strings.Add(_("2"));
    wxChoice* itemChoice73 = new wxChoice( itemPanel37, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice73Strings, 0 );
    itemChoice73->SetStringSelection(_("1"));
    itemFlexGridSizer63->Add(itemChoice73, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer74Static = new wxStaticBox(itemPanel37, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer74 = new wxStaticBoxSizer(itemStaticBoxSizer74Static, wxVERTICAL);
    itemBoxSizer57->Add(itemStaticBoxSizer74, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer75 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer74->Add(itemBoxSizer75, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText76 = new wxStaticText( itemPanel37, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer75->Add(itemStaticText76, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText77 = new wxStaticText( itemPanel37, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer75->Add(itemStaticText77, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer74->Add(itemBoxSizer78, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText79 = new wxStaticText( itemPanel37, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemStaticText79, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText80 = new wxStaticText( itemPanel37, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemStaticText80, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer74->Add(itemBoxSizer81, 1, wxGROW|wxALL, 5);
    wxButton* itemButton82 = new wxButton( itemPanel37, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer81->Add(itemButton82, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel37, _("Transmission"));

////@end SendUart content construction
    ReplaceRowColLabel();
    LoadCommand();
    ScanPort();
}


/*!
 * Should we show tooltips?
 */

bool SendUart::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SendUart::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SendUart bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SendUart bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SendUart::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SendUart icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SendUart icon retrieval
}


/*!
 * wxEVT_FILEPICKER_CHANGED event handler for ID_FILECTRL_FILE_LOCATION
 */

void SendUart::OnFileLocationChanged( wxFileDirPickerEvent& event )
{
    wxFile file;
    uint32_t byteCounter[256], muCnt = 0, luCnt = 0xFFFFFFFFL;
    size_t id, row, col;
    unsigned char muByte = 0, luByte = 0;
    wxString str;
    double num;
    
    if (file.Open(event.GetPath().c_str()) && file.IsOpened())
    {
        // reset byte counter
        for (id = 0; id < 256; id++)
            byteCounter[id] = 0;
        
        m_bufferSize = (size_t)file.Length();
        str.Printf(_("%u"), m_bufferSize);
        ((wxStaticText *)FindWindow(wxID_STATIC_FILE_SIZE))->SetLabel(str);
        if (m_pBuffer)
            free(m_pBuffer);
        m_pBuffer = (unsigned char *)malloc(m_bufferSize);
        if (m_pBuffer)
        {
            // show theory transmit time
            if (((wxChoice *)FindWindow(ID_CHOICE_BAUD))->GetStringSelection().ToDouble(&num))
                ((wxStaticText *)FindWindow(wxID_STATIC_THEORY_TRANSMIT_TIME))->SetLabel(wxString::Format(_("%f second."), m_bufferSize * 10 / num));
            ((wxStaticText *)FindWindow(wxID_STATIC_PRATICE_TRANSMIT_TIME))->SetLabel(_("-"));

            if (m_bufferSize == (size_t)file.Read(m_pBuffer, m_bufferSize))
                for (id = 0; id < m_bufferSize; id++)
                    byteCounter[m_pBuffer[id]]++;
            for (row = 0; row < 16; row++)
            {
                for (col = 0; col < 16; col++)
                {
                    id = 16 * row + col;
                    str.Printf(wxT("%u"), byteCounter[id]);
                    if (byteCounter[id] > muCnt)
                    {
                        muByte = id;
                        muCnt = byteCounter[id];
                    }
                    if ((byteCounter[id] < luCnt) && (byteCounter[id] != 0))
                    {
                        luByte = id;
                        luCnt = byteCounter[id];
                    }
                    ((wxGrid *)FindWindow(ID_GRID_BYTE_COUNTER))->SetCellValue(row, col, str);
                    ((wxGrid *)FindWindow(ID_GRID_BYTE_COUNTER))->SetReadOnly(row, col);
                }
            }
            str.Printf(wxT("0x%X"), muByte);
            ((wxStaticText *)FindWindow(wxID_STATIC_MU_BYTE))->SetLabel(str);
            str.Printf(wxT("%u"), muCnt);
            ((wxStaticText *)FindWindow(wxID_STATIC_MU_CNT))->SetLabel(str);
            str.Printf(wxT("0x%X"), luByte);
            ((wxStaticText *)FindWindow(wxID_STATIC_LU_BYTE))->SetLabel(str);
            str.Printf(wxT("%u"), luCnt);
            ((wxStaticText *)FindWindow(wxID_STATIC_LU_CNT))->SetLabel(str);
        }
            
        // close file
        file.Close();
    }
}


/*!
 * Replace byte counter grid row and column label
 */

void SendUart::ReplaceRowColLabel(void)
{
    wxString label;
    size_t id;
    
    for (id = 0; id < 16; id++)
    {
        label.Printf(wxT("%X"), id);
        ((wxGrid *)FindWindow(ID_GRID_BYTE_COUNTER))->SetRowLabelValue(id, label);
        ((wxGrid *)FindWindow(ID_GRID_BYTE_COUNTER))->SetColLabelValue(id, label);
        ((wxGrid *)FindWindow(ID_GRID_GEN_DATA))->SetColLabelValue(id, label);
    }
    
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->SetColLabelValue(0, _("Property"));
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->SetColLabelValue(1, _("Value"));
}

/*!
 * scan available serial port
 */

void SendUart::ScanPort(void)
{
    const char *ports[] = {
        wxCOM1, wxCOM2, wxCOM3, wxCOM4, wxCOM5, wxCOM6, wxCOM7, wxCOM8,
        wxCOM9, wxCOM10, wxCOM11, wxCOM12, wxCOM13, wxCOM14, wxCOM15, wxCOM16,
    };
    wxArrayString result;
    for (size_t id = 0; id < (sizeof(ports) >> 2); id++)
    {
        COMMCONFIG cc;
        DWORD dwSize = sizeof(cc);
        if (::GetDefaultCommConfig(ports[id], &cc, &dwSize)) {
		  if(cc.dwProviderSubType == PST_RS232) {
			 if(m_com.Open(ports[id]) >= 0) {
				result.Add(wxString(ports[id]));
				m_com.Close();
			 }
		  }
	   }
    }
    
    ((wxChoice *)FindWindow(ID_CHOICE_PORT))->Clear();
    ((wxChoice *)FindWindow(ID_CHOICE_PORT))->Append(result);
    if (!result.IsEmpty())
        ((wxChoice *)FindWindow(ID_CHOICE_PORT))->SetSelection(0);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SCAN_PORT
 */

void SendUart::OnButtonScanPortClick( wxCommandEvent& event )
{
    if (!IsOpened())
        ScanPort();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TRANSMIT
 */

void SendUart::OnButtonTransmitClick( wxCommandEvent& event )
{
    size_t writeByte = 0;
    wxDateTime start, end;
    wxTimeSpan diff;

    if (m_com.IsOpen() && m_pBuffer)
    {
        start = wxDateTime::Now();
        writeByte = m_com.Write((char *)m_pBuffer, m_bufferSize);
        if (writeByte != m_bufferSize)
            wxLogError(_("Failed to write data to serial port! Error = %d"), writeByte);
        else
        {
            end = wxDateTime::Now();
            diff = end.Subtract(start);
            ((wxStaticText *)FindWindow(wxID_STATIC_PRATICE_TRANSMIT_TIME))->SetLabel(diff.GetSeconds().ToString() + wxString::Format(_(" second.")));
        }
    }
    else
    {
        wxLogError(_("Can not transmit in non-connection state!"));
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_TRANSMIT
 */

void SendUart::OnButtonTransmitUpdate( wxUpdateUIEvent& event )
{
    event.Enable((m_pBuffer != NULL) && (m_bufferSize > 0) && IsOpened());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONNECT
 */

void SendUart::OnButtonConnectClick( wxCommandEvent& event )
{
    wxString selectedPort = ((wxChoice *)FindWindow(ID_CHOICE_PORT))->GetStringSelection();
    long num = 0;
    
    if (!selectedPort.IsEmpty())
    {
        if (((wxChoice *)FindWindow(ID_CHOICE_BAUD))->GetStringSelection().ToLong(&num))
            m_serialDCS.baud = (wxBaud)num;
        if (((wxChoice *)FindWindow(ID_CHOICE_CHAR_SIZE))->GetStringSelection().ToLong(&num))
            m_serialDCS.wordlen = (unsigned char)num;
        num = ((wxChoice *)FindWindow(ID_CHOICE_PARITY))->GetCurrentSelection();
        m_serialDCS.parity = (wxParity)num;
        if (((wxChoice *)FindWindow(ID_CHOICE_STOP_BITS))->GetStringSelection().ToLong(&num))
            m_serialDCS.stopbits = (unsigned char)num;
        m_com.Open(selectedPort.c_str(), &m_serialDCS);
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_CONNECT
 */

void SendUart::OnButtonConnectUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsOpened());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DISCONNECT
 */

void SendUart::OnButtonDisconnectClick( wxCommandEvent& event )
{
    if (m_com.IsOpen())
        m_com.Close();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_DISCONNECT
 */

void SendUart::OnButtonDisconnectUpdate( wxUpdateUIEvent& event )
{
    event.Enable(IsOpened());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICE_PORT
 */

void SendUart::OnChoicePortUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsOpened());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICE_CHAR_SIZE
 */

void SendUart::OnChoiceCharSizeUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsOpened());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICE_PARITY
 */

void SendUart::OnChoiceParityUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsOpened());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHOICE_STOP_BITS
 */

void SendUart::OnChoiceStopBitsUpdate( wxUpdateUIEvent& event )
{
    event.Enable(!IsOpened());
}


/*!
 * Check if we're in opened state
 */

bool SendUart::IsOpened(void)
{
    return (m_com.IsOpen() == 1);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_BAUD
 */

void SendUart::OnChoiceBaudSelected( wxCommandEvent& event )
{
    long num;
    double dNum;
    if (event.GetString().ToLong(&num))
        if (IsOpened())
            m_com.SetBaudRate((wxBaud)num);
    if ((event.GetString().ToDouble(&dNum)) && (m_pBuffer != NULL) && (m_bufferSize > 0))
        ((wxStaticText *)FindWindow(wxID_STATIC_THEORY_TRANSMIT_TIME))->SetLabel(wxString::Format(_("%f millisecond."), m_bufferSize * 10000 / dNum));
}


/*!
 * Read command list from active group
 */

void SendUart::LoadCommand(void)
{
    wxString actGrp, propIdx, propStr;
    bool cont;
    long dummy;
    int row = 0;
    wxGridCellAttr *cellAttr = NULL;
    wxGrid *grid = (wxGrid *)FindWindow(ID_GRID_CMD_LIST);
    wxConfig *cfg = wxGetApp().m_appConfig;
    
    cfg->SetPath(wxT("/CommandGroup")); 
    if (cfg->Read(wxT("ActiveGroup"), &actGrp) && cfg->HasGroup(actGrp) && grid)
    {
        // command enumerration
        cfg->SetPath(actGrp);
        cont = cfg->GetFirstGroup(propIdx, dummy);
        while (cont)
        {
            cfg->Read(propIdx + wxT("/Property"), &propStr);
            grid->SetCellValue(row++, 0, propStr);
            cont = cfg->GetNextGroup(propIdx, dummy);
            if (cont)
                grid->AppendRows();
        }
        
        // set read only on property column
        cellAttr = grid->GetOrCreateCellAttr(0, 0);
        if (cellAttr)
            cellAttr->SetReadOnly();
        grid->SetColAttr(0, cellAttr);
        
        // autosize
        grid->AutoSize();
    }
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID_1
 */

void SendUart::OnButtonIdClick( wxCommandEvent& event )
{
    wxStaticText *id[2];
    wxButton *btn = (wxButton *)FindWindow(event.GetId());
    long style;

    id[0] = (wxStaticText *)FindWindow(wxID_STATIC_ID0);
    id[1] = (wxStaticText *)FindWindow(wxID_STATIC_ID1);

    for (size_t idx = 0; idx < 2; idx++)
    {
        style = id[idx]->GetWindowStyleFlag();
        id[idx]->ToggleWindowStyle(wxSIMPLE_BORDER);
        if (style & wxSIMPLE_BORDER)
            id[idx]->SetLabel(btn->GetLabel());
        else
            id[idx]->SetLabel(id[idx]->GetLabel());
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID_NEXT
 */

void SendUart::OnButtonIdNextClick( wxCommandEvent& event )
{
    wxStaticText *id[2];

    id[0] = (wxStaticText *)FindWindow(wxID_STATIC_ID0);
    id[1] = (wxStaticText *)FindWindow(wxID_STATIC_ID1);

    for (size_t idx = 0; idx < 2; idx++)
    {
        id[idx]->ToggleWindowStyle(wxSIMPLE_BORDER);
        id[idx]->SetLabel(id[idx]->GetLabel());
    }
}

