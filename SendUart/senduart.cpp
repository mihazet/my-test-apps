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
    EVT_BUTTON( ID_BUTTON_ID0_1, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_2, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_3, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_4, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_5, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_6, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_7, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_8, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_9, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_0, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID0_STAR, SendUart::OnButtonId0Click )

    EVT_BUTTON( ID_BUTTON_ID1_1, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_2, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_3, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_4, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_5, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_6, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_7, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_8, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_9, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_0, SendUart::OnButtonId1Click )

    EVT_BUTTON( ID_BUTTON_ID1_STAR, SendUart::OnButtonId1Click )

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

    wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PANEL_GENERATION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemPanel2->SetSizer(itemBoxSizer3);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 1, wxGROW|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer5Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Available commands"));
    wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(itemStaticBoxSizer5Static, wxVERTICAL);
    itemBoxSizer4->Add(itemStaticBoxSizer5, 0, wxALIGN_TOP|wxALL, 5);
    wxGrid* itemGrid6 = new wxGrid( itemPanel2, ID_GRID_CMD_LIST, wxDefaultPosition, wxSize(220, 400), wxDOUBLE_BORDER|wxVSCROLL );
    itemGrid6->SetDefaultColSize(80);
    itemGrid6->SetDefaultRowSize(18);
    itemGrid6->SetColLabelSize(18);
    itemGrid6->SetRowLabelSize(28);
    itemGrid6->CreateGrid(1, 2, wxGrid::wxGridSelectRows);
    itemStaticBoxSizer5->Add(itemGrid6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer7, 1, wxALIGN_TOP|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer8 = new wxFlexGridSizer(2, 2, 0, 0);
    itemBoxSizer7->Add(itemFlexGridSizer8, 0, wxGROW, 0);
    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel2, wxID_STATIC, _("ID0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxSHAPED, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel2, wxID_STATIC, _("ID1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxSHAPED, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer8->Add(itemBoxSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer12 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer11->Add(itemFlexGridSizer12, 0, wxALIGN_CENTER_VERTICAL, 0);
    wxButton* itemButton13 = new wxButton( itemPanel2, ID_BUTTON_ID0_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton14 = new wxButton( itemPanel2, ID_BUTTON_ID0_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton15 = new wxButton( itemPanel2, ID_BUTTON_ID0_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton15, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton16 = new wxButton( itemPanel2, ID_BUTTON_ID0_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton17 = new wxButton( itemPanel2, ID_BUTTON_ID0_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton18 = new wxButton( itemPanel2, ID_BUTTON_ID0_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton18, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton19 = new wxButton( itemPanel2, ID_BUTTON_ID0_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton20 = new wxButton( itemPanel2, ID_BUTTON_ID0_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton20, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton21 = new wxButton( itemPanel2, ID_BUTTON_ID0_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton21, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton22 = new wxButton( itemPanel2, ID_BUTTON_ID0_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton22, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton23 = new wxButton( itemPanel2, ID_BUTTON_ID0_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer12->Add(itemButton23, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText* itemStaticText24 = new wxStaticText( itemPanel2, wxID_STATIC_ID0, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxST_NO_AUTORESIZE|wxDOUBLE_BORDER );
    itemStaticText24->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer11->Add(itemStaticText24, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer8->Add(itemBoxSizer25, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText26 = new wxStaticText( itemPanel2, wxID_STATIC_ID1, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxST_NO_AUTORESIZE|wxDOUBLE_BORDER );
    itemStaticText26->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer25->Add(itemStaticText26, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxFlexGridSizer* itemFlexGridSizer27 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer25->Add(itemFlexGridSizer27, 0, wxALIGN_CENTER_VERTICAL, 0);
    wxButton* itemButton28 = new wxButton( itemPanel2, ID_BUTTON_ID1_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton28, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

    wxButton* itemButton29 = new wxButton( itemPanel2, ID_BUTTON_ID1_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton30 = new wxButton( itemPanel2, ID_BUTTON_ID1_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton30, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton31 = new wxButton( itemPanel2, ID_BUTTON_ID1_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton31, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

    wxButton* itemButton32 = new wxButton( itemPanel2, ID_BUTTON_ID1_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton33 = new wxButton( itemPanel2, ID_BUTTON_ID1_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton33, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton34 = new wxButton( itemPanel2, ID_BUTTON_ID1_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton34, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

    wxButton* itemButton35 = new wxButton( itemPanel2, ID_BUTTON_ID1_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton35, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton36 = new wxButton( itemPanel2, ID_BUTTON_ID1_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton36, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton37 = new wxButton( itemPanel2, ID_BUTTON_ID1_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton37, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

    wxButton* itemButton38 = new wxButton( itemPanel2, ID_BUTTON_ID1_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer27->Add(itemButton38, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Generated raw data"));
    wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxVERTICAL);
    itemBoxSizer4->Add(itemStaticBoxSizer39, 0, wxALIGN_TOP|wxALL, 5);
    wxGrid* itemGrid40 = new wxGrid( itemPanel2, ID_GRID_GEN_DATA, wxDefaultPosition, wxSize(-1, 400), wxNO_BORDER|wxVSCROLL );
    itemGrid40->SetDefaultColSize(20);
    itemGrid40->SetDefaultRowSize(18);
    itemGrid40->SetColLabelSize(18);
    itemGrid40->SetRowLabelSize(40);
    itemGrid40->CreateGrid(1, 16, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer39->Add(itemGrid40, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel41 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxVERTICAL);
    itemPanel41->SetSizer(itemBoxSizer42);

    wxStaticBox* itemStaticBoxSizer43Static = new wxStaticBox(itemPanel41, wxID_ANY, _("File Settting"));
    wxStaticBoxSizer* itemStaticBoxSizer43 = new wxStaticBoxSizer(itemStaticBoxSizer43Static, wxVERTICAL);
    itemBoxSizer42->Add(itemStaticBoxSizer43, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer43->Add(itemBoxSizer44, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel41, wxID_STATIC, _("File path:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer44->Add(itemStaticText45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFilePickerCtrl* itemFilePickerCtrl46 = new wxFilePickerCtrl( itemPanel41, ID_FILECTRL_FILE_LOCATION, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
    itemBoxSizer44->Add(itemFilePickerCtrl46, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer43->Add(itemBoxSizer47, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid48 = new wxGrid( itemPanel41, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid48->SetDefaultColSize(45);
    itemGrid48->SetDefaultRowSize(18);
    itemGrid48->SetColLabelSize(18);
    itemGrid48->SetRowLabelSize(20);
    itemGrid48->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer47->Add(itemGrid48, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer49Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer49 = new wxStaticBoxSizer(itemStaticBoxSizer49Static, wxVERTICAL);
    itemBoxSizer47->Add(itemStaticBoxSizer49, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer50 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer50->AddGrowableCol(1);
    itemStaticBoxSizer49->Add(itemFlexGridSizer50, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText51 = new wxStaticText( itemPanel41, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText51, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel41, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText52, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText53 = new wxStaticText( itemPanel41, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText53, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel41, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText54, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText55 = new wxStaticText( itemPanel41, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText55, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText56 = new wxStaticText( itemPanel41, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText56, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText57 = new wxStaticText( itemPanel41, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText57, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText58 = new wxStaticText( itemPanel41, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText58, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText59 = new wxStaticText( itemPanel41, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText59, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText60 = new wxStaticText( itemPanel41, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(itemStaticText60, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer42->Add(itemBoxSizer61, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer62Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer62 = new wxStaticBoxSizer(itemStaticBoxSizer62Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer62, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer63, 0, wxGROW|wxALL, 0);
    wxButton* itemButton64 = new wxButton( itemPanel41, ID_BUTTON_CONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemButton64, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton65 = new wxButton( itemPanel41, ID_BUTTON_DISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemButton65, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton66 = new wxButton( itemPanel41, ID_BUTTON_SCAN_PORT, _("Rescan Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemButton66, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer67 = new wxFlexGridSizer(0, 2, 0, 5);
    itemFlexGridSizer67->AddGrowableCol(1);
    itemStaticBoxSizer62->Add(itemFlexGridSizer67, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText68 = new wxStaticText( itemPanel41, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer67->Add(itemStaticText68, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice69Strings;
    wxChoice* itemChoice69 = new wxChoice( itemPanel41, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice69Strings, 0 );
    itemFlexGridSizer67->Add(itemChoice69, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText70 = new wxStaticText( itemPanel41, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer67->Add(itemStaticText70, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice71Strings;
    itemChoice71Strings.Add(_("115200"));
    itemChoice71Strings.Add(_("57600"));
    itemChoice71Strings.Add(_("38400"));
    itemChoice71Strings.Add(_("19200"));
    itemChoice71Strings.Add(_("9600"));
    wxChoice* itemChoice71 = new wxChoice( itemPanel41, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice71Strings, 0 );
    itemChoice71->SetStringSelection(_("115200"));
    itemFlexGridSizer67->Add(itemChoice71, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel41, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer67->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice73Strings;
    itemChoice73Strings.Add(_("8"));
    itemChoice73Strings.Add(_("7"));
    itemChoice73Strings.Add(_("6"));
    itemChoice73Strings.Add(_("5"));
    wxChoice* itemChoice73 = new wxChoice( itemPanel41, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice73Strings, 0 );
    itemChoice73->SetStringSelection(_("8"));
    itemFlexGridSizer67->Add(itemChoice73, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText74 = new wxStaticText( itemPanel41, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer67->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice75Strings;
    itemChoice75Strings.Add(_("None"));
    itemChoice75Strings.Add(_("Odd"));
    itemChoice75Strings.Add(_("Even"));
    itemChoice75Strings.Add(_("Mark"));
    itemChoice75Strings.Add(_("Space"));
    wxChoice* itemChoice75 = new wxChoice( itemPanel41, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice75Strings, 0 );
    itemChoice75->SetStringSelection(_("None"));
    itemFlexGridSizer67->Add(itemChoice75, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText76 = new wxStaticText( itemPanel41, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer67->Add(itemStaticText76, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice77Strings;
    itemChoice77Strings.Add(_("1"));
    itemChoice77Strings.Add(_("2"));
    wxChoice* itemChoice77 = new wxChoice( itemPanel41, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice77Strings, 0 );
    itemChoice77->SetStringSelection(_("1"));
    itemFlexGridSizer67->Add(itemChoice77, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer78Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer78 = new wxStaticBoxSizer(itemStaticBoxSizer78Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer78, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer78->Add(itemBoxSizer79, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText80 = new wxStaticText( itemPanel41, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer79->Add(itemStaticText80, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText81 = new wxStaticText( itemPanel41, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer79->Add(itemStaticText81, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer78->Add(itemBoxSizer82, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText83 = new wxStaticText( itemPanel41, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer82->Add(itemStaticText83, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText84 = new wxStaticText( itemPanel41, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer82->Add(itemStaticText84, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer78->Add(itemBoxSizer85, 1, wxGROW|wxALL, 5);
    wxButton* itemButton86 = new wxButton( itemPanel41, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer85->Add(itemButton86, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel41, _("Transmission"));

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
        grid->AutoSizeColumn(0);
    }
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID0_1
 */

void SendUart::OnButtonId0Click( wxCommandEvent& event )
{
    ((wxStaticText *)FindWindow(wxID_STATIC_ID0))->SetLabel(((wxButton *)FindWindow(event.GetId()))->GetLabel());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ID1_1
 */

void SendUart::OnButtonId1Click( wxCommandEvent& event )
{
    ((wxStaticText *)FindWindow(wxID_STATIC_ID1))->SetLabel(((wxButton *)FindWindow(event.GetId()))->GetLabel());
}

