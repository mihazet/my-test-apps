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
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer9 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer8->Add(itemFlexGridSizer9, 0, wxALIGN_CENTER_VERTICAL, 0);
    wxButton* itemButton10 = new wxButton( itemPanel2, ID_BUTTON_ID_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton11 = new wxButton( itemPanel2, ID_BUTTON_ID_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);

    wxButton* itemButton12 = new wxButton( itemPanel2, ID_BUTTON_ID_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton12, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton13 = new wxButton( itemPanel2, ID_BUTTON_ID_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton14 = new wxButton( itemPanel2, ID_BUTTON_ID_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);

    wxButton* itemButton15 = new wxButton( itemPanel2, ID_BUTTON_ID_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton15, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton16 = new wxButton( itemPanel2, ID_BUTTON_ID_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton17 = new wxButton( itemPanel2, ID_BUTTON_ID_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);

    wxButton* itemButton18 = new wxButton( itemPanel2, ID_BUTTON_ID_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton18, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

    wxButton* itemButton19 = new wxButton( itemPanel2, ID_BUTTON_ID_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton20 = new wxButton( itemPanel2, ID_BUTTON_ID_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer9->Add(itemButton20, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel2, wxID_STATIC_ID0, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxSIMPLE_BORDER );
    itemStaticText21->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer8->Add(itemStaticText21, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel2, wxID_STATIC_ID1, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText22->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer8->Add(itemStaticText22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticBox* itemStaticBoxSizer23Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Generated raw data"));
    wxStaticBoxSizer* itemStaticBoxSizer23 = new wxStaticBoxSizer(itemStaticBoxSizer23Static, wxVERTICAL);
    itemBoxSizer4->Add(itemStaticBoxSizer23, 0, wxALIGN_TOP|wxALL, 5);
    wxGrid* itemGrid24 = new wxGrid( itemPanel2, ID_GRID_GEN_DATA, wxDefaultPosition, wxSize(-1, 400), wxNO_BORDER|wxVSCROLL );
    itemGrid24->SetDefaultColSize(20);
    itemGrid24->SetDefaultRowSize(18);
    itemGrid24->SetColLabelSize(18);
    itemGrid24->SetRowLabelSize(40);
    itemGrid24->CreateGrid(1, 16, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer23->Add(itemGrid24, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel25 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
    itemPanel25->SetSizer(itemBoxSizer26);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(itemPanel25, wxID_ANY, _("File Settting"));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxVERTICAL);
    itemBoxSizer26->Add(itemStaticBoxSizer27, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer27->Add(itemBoxSizer28, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel25, wxID_STATIC, _("File path:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer28->Add(itemStaticText29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFilePickerCtrl* itemFilePickerCtrl30 = new wxFilePickerCtrl( itemPanel25, ID_FILECTRL_FILE_LOCATION, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
    itemBoxSizer28->Add(itemFilePickerCtrl30, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer27->Add(itemBoxSizer31, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid32 = new wxGrid( itemPanel25, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid32->SetDefaultColSize(45);
    itemGrid32->SetDefaultRowSize(18);
    itemGrid32->SetColLabelSize(18);
    itemGrid32->SetRowLabelSize(20);
    itemGrid32->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer31->Add(itemGrid32, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer33Static = new wxStaticBox(itemPanel25, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer33 = new wxStaticBoxSizer(itemStaticBoxSizer33Static, wxVERTICAL);
    itemBoxSizer31->Add(itemStaticBoxSizer33, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer34 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer34->AddGrowableCol(1);
    itemStaticBoxSizer33->Add(itemFlexGridSizer34, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel25, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText35, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel25, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText36, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText37 = new wxStaticText( itemPanel25, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText37, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel25, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel25, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText39, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel25, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel25, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText41, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemPanel25, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText42, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText43 = new wxStaticText( itemPanel25, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText43, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel25, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText44, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer45, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer46Static = new wxStaticBox(itemPanel25, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer46 = new wxStaticBoxSizer(itemStaticBoxSizer46Static, wxVERTICAL);
    itemBoxSizer45->Add(itemStaticBoxSizer46, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer46->Add(itemBoxSizer47, 0, wxGROW|wxALL, 0);
    wxButton* itemButton48 = new wxButton( itemPanel25, ID_BUTTON_CONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton48, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton49 = new wxButton( itemPanel25, ID_BUTTON_DISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton49, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton50 = new wxButton( itemPanel25, ID_BUTTON_SCAN_PORT, _("Rescan Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton50, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer51 = new wxFlexGridSizer(0, 2, 0, 5);
    itemFlexGridSizer51->AddGrowableCol(1);
    itemStaticBoxSizer46->Add(itemFlexGridSizer51, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel25, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText52, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice53Strings;
    wxChoice* itemChoice53 = new wxChoice( itemPanel25, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice53Strings, 0 );
    itemFlexGridSizer51->Add(itemChoice53, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel25, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText54, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice55Strings;
    itemChoice55Strings.Add(_("115200"));
    itemChoice55Strings.Add(_("57600"));
    itemChoice55Strings.Add(_("38400"));
    itemChoice55Strings.Add(_("19200"));
    itemChoice55Strings.Add(_("9600"));
    wxChoice* itemChoice55 = new wxChoice( itemPanel25, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice55Strings, 0 );
    itemChoice55->SetStringSelection(_("115200"));
    itemFlexGridSizer51->Add(itemChoice55, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText56 = new wxStaticText( itemPanel25, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText56, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice57Strings;
    itemChoice57Strings.Add(_("8"));
    itemChoice57Strings.Add(_("7"));
    itemChoice57Strings.Add(_("6"));
    itemChoice57Strings.Add(_("5"));
    wxChoice* itemChoice57 = new wxChoice( itemPanel25, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice57Strings, 0 );
    itemChoice57->SetStringSelection(_("8"));
    itemFlexGridSizer51->Add(itemChoice57, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText58 = new wxStaticText( itemPanel25, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText58, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice59Strings;
    itemChoice59Strings.Add(_("None"));
    itemChoice59Strings.Add(_("Odd"));
    itemChoice59Strings.Add(_("Even"));
    itemChoice59Strings.Add(_("Mark"));
    itemChoice59Strings.Add(_("Space"));
    wxChoice* itemChoice59 = new wxChoice( itemPanel25, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice59Strings, 0 );
    itemChoice59->SetStringSelection(_("None"));
    itemFlexGridSizer51->Add(itemChoice59, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText60 = new wxStaticText( itemPanel25, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText60, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice61Strings;
    itemChoice61Strings.Add(_("1"));
    itemChoice61Strings.Add(_("2"));
    wxChoice* itemChoice61 = new wxChoice( itemPanel25, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice61Strings, 0 );
    itemChoice61->SetStringSelection(_("1"));
    itemFlexGridSizer51->Add(itemChoice61, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer62Static = new wxStaticBox(itemPanel25, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer62 = new wxStaticBoxSizer(itemStaticBoxSizer62Static, wxVERTICAL);
    itemBoxSizer45->Add(itemStaticBoxSizer62, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer63, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText64 = new wxStaticText( itemPanel25, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemStaticText64, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText65 = new wxStaticText( itemPanel25, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemStaticText65, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer66, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText67 = new wxStaticText( itemPanel25, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemStaticText67, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText68 = new wxStaticText( itemPanel25, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemStaticText68, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer69, 1, wxGROW|wxALL, 5);
    wxButton* itemButton70 = new wxButton( itemPanel25, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer69->Add(itemButton70, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel25, _("Transmission"));

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

