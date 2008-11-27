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

    EVT_UPDATE_UI( ID_FILECTRL_USER_FILE_SAVE, SendUart::OnFilectrlUserFileSaveUpdate )

    EVT_FILEPICKER_CHANGED( ID_FILECTRL_FILE_LOCATION, SendUart::OnFileLocationChanged )
    EVT_UPDATE_UI( ID_FILECTRL_FILE_LOCATION, SendUart::OnFilectrlFileLocationUpdate )

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
    wxConfig *cfg = wxGetApp().m_appConfig;
    cfg->SetPath(wxT("/App")); 
    cfg->Write(wxT("ID0"), ((wxStaticText *)FindWindow(wxID_STATIC_ID0))->GetLabel());
    cfg->Write(wxT("ID1"), ((wxStaticText *)FindWindow(wxID_STATIC_ID1))->GetLabel());
    cfg->Write(wxT("GenDataDestination"), ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_GEN_TO_INTERNAL_BUFFER))->GetValue() ? 0L : 1L);
    cfg->Write(wxT("TransmitDataSource"), ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_TRANSMIT_INTERNAL_BUFFER))->GetValue() ? 0L : 1L);
    cfg->Write(wxT("UsedUartPort"), ((wxChoice *)FindWindow(ID_CHOICE_PORT))->GetStringSelection());
    cfg->Write(wxT("UsedUartBaud"), ((wxChoice *)FindWindow(ID_CHOICE_BAUD))->GetCurrentSelection());
    cfg->Write(wxT("UsedUartCharSize"), ((wxChoice *)FindWindow(ID_CHOICE_CHAR_SIZE))->GetCurrentSelection());
    cfg->Write(wxT("UsedUartParity"), ((wxChoice *)FindWindow(ID_CHOICE_PARITY))->GetCurrentSelection());
    cfg->Write(wxT("UsedUartStopBits"), ((wxChoice *)FindWindow(ID_CHOICE_STOP_BITS))->GetCurrentSelection());
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
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);
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
    itemBoxSizer3->Add(itemBoxSizer18, 1, wxGROW|wxALL, 10);
    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel2, wxID_STATIC, _("Cube ID Input Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText19->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer18->Add(itemStaticText19, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel2, wxID_STATIC, _("The number or asterisk button you pressed will change the number enclosed by rectangle; the button N will move the rectangle to the other digit."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText20->Wrap(300);
    itemBoxSizer18->Add(itemStaticText20, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer18->Add(itemBoxSizer21, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer22 = new wxFlexGridSizer(4, 3, 0, 0);
    itemBoxSizer21->Add(itemFlexGridSizer22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
    wxButton* itemButton23 = new wxButton( itemPanel2, ID_BUTTON_ID_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton23, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton24 = new wxButton( itemPanel2, ID_BUTTON_ID_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton24, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton25 = new wxButton( itemPanel2, ID_BUTTON_ID_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton25, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton26 = new wxButton( itemPanel2, ID_BUTTON_ID_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton26, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton27 = new wxButton( itemPanel2, ID_BUTTON_ID_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton27, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton28 = new wxButton( itemPanel2, ID_BUTTON_ID_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton28, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton29 = new wxButton( itemPanel2, ID_BUTTON_ID_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton30 = new wxButton( itemPanel2, ID_BUTTON_ID_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton30, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton31 = new wxButton( itemPanel2, ID_BUTTON_ID_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton31, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton32 = new wxButton( itemPanel2, ID_BUTTON_ID_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton33 = new wxButton( itemPanel2, ID_BUTTON_ID_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton33, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton34 = new wxButton( itemPanel2, ID_BUTTON_ID_NEXT, _("N"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer22->Add(itemButton34, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel2, wxID_STATIC_ID0, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxSIMPLE_BORDER );
    itemStaticText35->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer21->Add(itemStaticText35, 0, wxALIGN_CENTER_VERTICAL|wxALL, 20);

    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel2, wxID_STATIC_ID1, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText36->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer21->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 20);

    wxStaticLine* itemStaticLine37 = new wxStaticLine( itemPanel2, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer18->Add(itemStaticLine37, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel2, wxID_STATIC, _("Operations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText38->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer18->Add(itemStaticText38, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add the selected command(s) for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText39->Wrap(300);
    itemBoxSizer18->Add(itemStaticText39, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 10);

    wxButton* itemButton40 = new wxButton( itemPanel2, ID_BUTTON, _(">> Same ID + Selected Command(s) >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemButton40, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add one random command for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText41->Wrap(300);
    itemBoxSizer18->Add(itemStaticText41, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxButton* itemButton42 = new wxButton( itemPanel2, ID_BUTTON1, _(">> Same ID + One Random Command >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemButton42, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText43 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add some random commands for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText43->Wrap(300);
    itemBoxSizer18->Add(itemStaticText43, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer18->Add(itemBoxSizer44, 0, wxGROW, 0);
    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel2, wxID_STATIC, _("number of commands (max 99):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer44->Add(itemStaticText45, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 15);

    wxTextCtrl* itemTextCtrl46 = new wxTextCtrl( itemPanel2, ID_TEXTCTRL_CMD_COUNT, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
    itemTextCtrl46->SetMaxLength(2);
    itemBoxSizer44->Add(itemTextCtrl46, 0, wxALIGN_CENTER_VERTICAL, 10);

    wxButton* itemButton47 = new wxButton( itemPanel2, ID_BUTTON2, _(">> Same ID + Above Random Commands >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemButton47, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText48 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add some random commands for other Cube."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText48->Wrap(300);
    itemBoxSizer18->Add(itemStaticText48, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer18->Add(itemBoxSizer49, 0, wxGROW, 0);
    wxStaticText* itemStaticText50 = new wxStaticText( itemPanel2, wxID_STATIC, _("number of commands (max 99):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer49->Add(itemStaticText50, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 15);

    wxTextCtrl* itemTextCtrl51 = new wxTextCtrl( itemPanel2, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
    itemTextCtrl51->SetMaxLength(2);
    itemBoxSizer49->Add(itemTextCtrl51, 0, wxALIGN_CENTER_VERTICAL, 10);

    wxButton* itemButton52 = new wxButton( itemPanel2, ID_BUTTON3, _(">> Other ID + Above Random Commnds >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemButton52, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticBox* itemStaticBoxSizer53Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Generated raw data"));
    wxStaticBoxSizer* itemStaticBoxSizer53 = new wxStaticBoxSizer(itemStaticBoxSizer53Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer53, 0, wxGROW|wxALL, 5);
    wxGrid* itemGrid54 = new wxGrid( itemPanel2, ID_GRID_GEN_DATA, wxDefaultPosition, wxSize(-1, 400), wxNO_BORDER|wxVSCROLL );
    itemGrid54->SetDefaultColSize(20);
    itemGrid54->SetDefaultRowSize(18);
    itemGrid54->SetColLabelSize(18);
    itemGrid54->SetRowLabelSize(40);
    itemGrid54->CreateGrid(1, 16, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer53->Add(itemGrid54, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer55Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Save data to..."));
    wxStaticBoxSizer* itemStaticBoxSizer55 = new wxStaticBoxSizer(itemStaticBoxSizer55Static, wxVERTICAL);
    itemStaticBoxSizer53->Add(itemStaticBoxSizer55, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    wxRadioButton* itemRadioButton56 = new wxRadioButton( itemPanel2, ID_RADIOBUTTON_GEN_TO_INTERNAL_BUFFER, _("Internal buffer"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
    itemRadioButton56->SetValue(true);
    itemStaticBoxSizer55->Add(itemRadioButton56, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer55->Add(itemBoxSizer57, 0, wxGROW|wxLEFT, 0);
    wxRadioButton* itemRadioButton58 = new wxRadioButton( itemPanel2, ID_RADIOBUTTON_GEN_TO_USER_FILE, _("User File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton58->SetValue(false);
    itemBoxSizer57->Add(itemRadioButton58, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxFilePickerCtrl* itemFilePickerCtrl59 = new wxFilePickerCtrl( itemPanel2, ID_FILECTRL_USER_FILE_SAVE, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_SAVE|wxFLP_OVERWRITE_PROMPT );
    itemBoxSizer57->Add(itemFilePickerCtrl59, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel60 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxVERTICAL);
    itemPanel60->SetSizer(itemBoxSizer61);

    wxStaticBox* itemStaticBoxSizer62Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Data Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer62 = new wxStaticBoxSizer(itemStaticBoxSizer62Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer62, 1, wxGROW|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer63Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Load data from..."));
    wxStaticBoxSizer* itemStaticBoxSizer63 = new wxStaticBoxSizer(itemStaticBoxSizer63Static, wxVERTICAL);
    itemStaticBoxSizer62->Add(itemStaticBoxSizer63, 0, wxGROW|wxALL, 5);
    wxRadioButton* itemRadioButton64 = new wxRadioButton( itemPanel60, ID_RADIOBUTTON_TRANSMIT_INTERNAL_BUFFER, _("Internale buffer"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton64->SetValue(true);
    itemStaticBoxSizer63->Add(itemRadioButton64, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer63->Add(itemBoxSizer65, 0, wxGROW, 0);
    wxRadioButton* itemRadioButton66 = new wxRadioButton( itemPanel60, ID_RADIOBUTTON_TRANSMIT_USER_FILE, _("User File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton66->SetValue(false);
    itemBoxSizer65->Add(itemRadioButton66, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxFilePickerCtrl* itemFilePickerCtrl67 = new wxFilePickerCtrl( itemPanel60, ID_FILECTRL_FILE_LOCATION, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
    itemBoxSizer65->Add(itemFilePickerCtrl67, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer68, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid69 = new wxGrid( itemPanel60, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid69->SetDefaultColSize(45);
    itemGrid69->SetDefaultRowSize(18);
    itemGrid69->SetColLabelSize(18);
    itemGrid69->SetRowLabelSize(20);
    itemGrid69->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer68->Add(itemGrid69, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer70Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer70 = new wxStaticBoxSizer(itemStaticBoxSizer70Static, wxVERTICAL);
    itemBoxSizer68->Add(itemStaticBoxSizer70, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer71 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer71->AddGrowableCol(1);
    itemStaticBoxSizer70->Add(itemFlexGridSizer71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel60, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText73 = new wxStaticText( itemPanel60, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText73, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText74 = new wxStaticText( itemPanel60, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText75 = new wxStaticText( itemPanel60, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText75, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText76 = new wxStaticText( itemPanel60, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText76, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText77 = new wxStaticText( itemPanel60, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText77, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText78 = new wxStaticText( itemPanel60, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText78, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText79 = new wxStaticText( itemPanel60, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText79, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText80 = new wxStaticText( itemPanel60, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText80, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText81 = new wxStaticText( itemPanel60, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText81, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer61->Add(itemBoxSizer82, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer83Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer83 = new wxStaticBoxSizer(itemStaticBoxSizer83Static, wxVERTICAL);
    itemBoxSizer82->Add(itemStaticBoxSizer83, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer83->Add(itemBoxSizer84, 0, wxGROW|wxALL, 0);
    wxButton* itemButton85 = new wxButton( itemPanel60, ID_BUTTON_CONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer84->Add(itemButton85, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton86 = new wxButton( itemPanel60, ID_BUTTON_DISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer84->Add(itemButton86, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton87 = new wxButton( itemPanel60, ID_BUTTON_SCAN_PORT, _("Rescan Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer84->Add(itemButton87, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer88 = new wxFlexGridSizer(0, 2, 0, 5);
    itemFlexGridSizer88->AddGrowableCol(1);
    itemStaticBoxSizer83->Add(itemFlexGridSizer88, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText89 = new wxStaticText( itemPanel60, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer88->Add(itemStaticText89, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice90Strings;
    wxChoice* itemChoice90 = new wxChoice( itemPanel60, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice90Strings, 0 );
    itemFlexGridSizer88->Add(itemChoice90, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText91 = new wxStaticText( itemPanel60, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer88->Add(itemStaticText91, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice92Strings;
    itemChoice92Strings.Add(_("115200"));
    itemChoice92Strings.Add(_("57600"));
    itemChoice92Strings.Add(_("38400"));
    itemChoice92Strings.Add(_("19200"));
    itemChoice92Strings.Add(_("9600"));
    wxChoice* itemChoice92 = new wxChoice( itemPanel60, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice92Strings, 0 );
    itemChoice92->SetStringSelection(_("115200"));
    itemFlexGridSizer88->Add(itemChoice92, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText93 = new wxStaticText( itemPanel60, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer88->Add(itemStaticText93, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice94Strings;
    itemChoice94Strings.Add(_("8"));
    itemChoice94Strings.Add(_("7"));
    itemChoice94Strings.Add(_("6"));
    itemChoice94Strings.Add(_("5"));
    wxChoice* itemChoice94 = new wxChoice( itemPanel60, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice94Strings, 0 );
    itemChoice94->SetStringSelection(_("8"));
    itemFlexGridSizer88->Add(itemChoice94, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText95 = new wxStaticText( itemPanel60, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer88->Add(itemStaticText95, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice96Strings;
    itemChoice96Strings.Add(_("None"));
    itemChoice96Strings.Add(_("Odd"));
    itemChoice96Strings.Add(_("Even"));
    itemChoice96Strings.Add(_("Mark"));
    itemChoice96Strings.Add(_("Space"));
    wxChoice* itemChoice96 = new wxChoice( itemPanel60, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice96Strings, 0 );
    itemChoice96->SetStringSelection(_("None"));
    itemFlexGridSizer88->Add(itemChoice96, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText97 = new wxStaticText( itemPanel60, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer88->Add(itemStaticText97, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice98Strings;
    itemChoice98Strings.Add(_("1"));
    itemChoice98Strings.Add(_("2"));
    wxChoice* itemChoice98 = new wxChoice( itemPanel60, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice98Strings, 0 );
    itemChoice98->SetStringSelection(_("1"));
    itemFlexGridSizer88->Add(itemChoice98, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer99Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer99 = new wxStaticBoxSizer(itemStaticBoxSizer99Static, wxVERTICAL);
    itemBoxSizer82->Add(itemStaticBoxSizer99, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer99->Add(itemBoxSizer100, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText101 = new wxStaticText( itemPanel60, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText102 = new wxStaticText( itemPanel60, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText102, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer99->Add(itemBoxSizer103, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText104 = new wxStaticText( itemPanel60, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer103->Add(itemStaticText104, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText105 = new wxStaticText( itemPanel60, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer103->Add(itemStaticText105, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer99->Add(itemBoxSizer106, 1, wxGROW|wxALL, 5);
    wxButton* itemButton107 = new wxButton( itemPanel60, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer106->Add(itemButton107, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel60, _("Transmission"));

////@end SendUart content construction
    ReplaceRowColLabel();
    LoadCommand();
    LoadAppConfig();
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


/*!
 * wxEVT_UPDATE_UI event handler for ID_FILECTRL_USER_FILE_SAVE
 */

void SendUart::OnFilectrlUserFileSaveUpdate( wxUpdateUIEvent& event )
{
    wxRadioButton *rb = (wxRadioButton *)FindWindow(ID_RADIOBUTTON_GEN_TO_USER_FILE);
    if (rb)
        event.Enable(rb->GetValue());
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_FILECTRL_FILE_LOCATION
 */

void SendUart::OnFilectrlFileLocationUpdate( wxUpdateUIEvent& event )
{
    wxRadioButton *rb = (wxRadioButton *)FindWindow(ID_RADIOBUTTON_TRANSMIT_USER_FILE);
    if (rb)
        event.Enable(rb->GetValue());
}


/*!
 * Read application configuration
 */

void SendUart::LoadAppConfig(void)
{
    wxString strVal;
    long numVal;
    wxConfig *cfg = wxGetApp().m_appConfig;
    
    cfg->SetPath(wxT("/App"));

    cfg->Read(wxT("ID0"), &strVal);
    ((wxStaticText *)FindWindow(wxID_STATIC_ID0))->SetLabel(strVal);
    cfg->Read(wxT("ID1"), &strVal);
    ((wxStaticText *)FindWindow(wxID_STATIC_ID1))->SetLabel(strVal);
    cfg->Read(wxT("GenDataDestination"), &numVal);
    ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_GEN_TO_INTERNAL_BUFFER))->SetValue(numVal == 0);
    ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_GEN_TO_USER_FILE))->SetValue(numVal == 1);
    cfg->Read(wxT("TransmitDataSource"), &numVal);
    ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_TRANSMIT_INTERNAL_BUFFER))->SetValue(numVal == 0);
    ((wxRadioButton *)FindWindow(ID_RADIOBUTTON_TRANSMIT_USER_FILE))->SetValue(numVal == 1);
    cfg->Read(wxT("UsedUartBaud"), &numVal);
    ((wxChoice *)FindWindow(ID_CHOICE_BAUD))->SetSelection(numVal);
    cfg->Read(wxT("UsedUartCharSize"), &numVal);
    ((wxChoice *)FindWindow(ID_CHOICE_CHAR_SIZE))->SetSelection(numVal);
    cfg->Read(wxT("UsedUartParity"), &numVal);
    ((wxChoice *)FindWindow(ID_CHOICE_PARITY))->SetSelection(numVal);
    cfg->Read(wxT("UsedUartStopBits"), &numVal);
    ((wxChoice *)FindWindow(ID_CHOICE_STOP_BITS))->SetSelection(numVal);
}
