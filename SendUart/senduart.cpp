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
#include "wx/radiobox.h"

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
    EVT_GRID_CMD_SELECT_CELL( ID_GRID_CMD_LIST, SendUart::OnGridCmdListSelectCell )

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

    EVT_BUTTON( ID_BUTTON_OP1, SendUart::OnButtonOp1Click )

    EVT_BUTTON( ID_BUTTON_GEN_DATA_CLEAR, SendUart::OnButtonGenDataClearClick )
    EVT_UPDATE_UI( ID_BUTTON_GEN_DATA_CLEAR, SendUart::OnButtonGenDataClearUpdate )

    EVT_BUTTON( ID_BUTTON_GEN_DATA_SAVE, SendUart::OnButtonGenDataSaveClick )
    EVT_UPDATE_UI( ID_BUTTON_GEN_DATA_SAVE, SendUart::OnButtonGenDataSaveUpdate )

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
    cfg->Write(wxT("GenDataDestination"), ((wxRadioBox *)FindWindow(ID_RADIOBOX_GEN_DATA_TO))->GetSelection());
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
    logTextCtrl_ = NULL;
    m_genDataCount = 0;
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
    wxGrid* itemGrid5 = new wxGrid( itemPanel2, ID_GRID_CMD_LIST, wxDefaultPosition, wxSize(230, 400), wxNO_BORDER|wxFULL_REPAINT_ON_RESIZE|wxVSCROLL );
    itemGrid5->SetDefaultColSize(60);
    itemGrid5->SetDefaultRowSize(18);
    itemGrid5->SetColLabelSize(18);
    itemGrid5->SetRowLabelSize(28);
    itemGrid5->CreateGrid(1, 2, wxGrid::wxGridSelectRows);
    itemStaticBoxSizer4->Add(itemGrid5, 1, wxGROW|wxALL, 5);

    wxPanel* itemPanel6 = new wxPanel( itemPanel2, ID_PANEL_CMD_INFO, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
    itemStaticBoxSizer4->Add(itemPanel6, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemPanel6->SetSizer(itemBoxSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel6, wxID_STATIC_CMD_DESCRIPTION, _T(""), wxDefaultPosition, wxSize(-1, 80), wxFULL_REPAINT_ON_RESIZE );
    itemBoxSizer7->Add(itemStaticText8, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer9, 1, wxGROW|wxALL, 10);
    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel2, wxID_STATIC, _("Cube ID Input Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText10->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer9->Add(itemStaticText10, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel2, wxID_STATIC, _("The number or asterisk button you pressed will change the number enclosed by rectangle; the button N will move the rectangle to the other digit."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText11->Wrap(300);
    itemBoxSizer9->Add(itemStaticText11, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer9->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer13 = new wxFlexGridSizer(4, 3, 0, 0);
    itemBoxSizer12->Add(itemFlexGridSizer13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
    wxButton* itemButton14 = new wxButton( itemPanel2, ID_BUTTON_ID_1, _("1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton15 = new wxButton( itemPanel2, ID_BUTTON_ID_2, _("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton15, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton16 = new wxButton( itemPanel2, ID_BUTTON_ID_3, _("3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton17 = new wxButton( itemPanel2, ID_BUTTON_ID_4, _("4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton18 = new wxButton( itemPanel2, ID_BUTTON_ID_5, _("5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton18, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton19 = new wxButton( itemPanel2, ID_BUTTON_ID_6, _("6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton20 = new wxButton( itemPanel2, ID_BUTTON_ID_7, _("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton20, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton21 = new wxButton( itemPanel2, ID_BUTTON_ID_8, _("8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton21, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton22 = new wxButton( itemPanel2, ID_BUTTON_ID_9, _("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton22, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton23 = new wxButton( itemPanel2, ID_BUTTON_ID_0, _("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton23, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton24 = new wxButton( itemPanel2, ID_BUTTON_ID_STAR, _("*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton24, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxButton* itemButton25 = new wxButton( itemPanel2, ID_BUTTON_ID_NEXT, _("N"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemFlexGridSizer13->Add(itemButton25, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText* itemStaticText26 = new wxStaticText( itemPanel2, wxID_STATIC_ID0, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxSIMPLE_BORDER );
    itemStaticText26->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer12->Add(itemStaticText26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 20);

    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel2, wxID_STATIC_ID1, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    itemStaticText27->SetFont(wxFont(48, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Microsoft Sans Serif")));
    itemBoxSizer12->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 20);

    wxStaticLine* itemStaticLine28 = new wxStaticLine( itemPanel2, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer9->Add(itemStaticLine28, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel2, wxID_STATIC, _("Operations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText29->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer9->Add(itemStaticText29, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add the selected command(s) for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText30->Wrap(300);
    itemBoxSizer9->Add(itemStaticText30, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 10);

    wxButton* itemButton31 = new wxButton( itemPanel2, ID_BUTTON_OP1, _(">> Same ID + Selected Command(s) >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton31, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add one random command for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText32->Wrap(300);
    itemBoxSizer9->Add(itemStaticText32, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxButton* itemButton33 = new wxButton( itemPanel2, ID_BUTTON_OP2, _(">> Same ID + One Random Command >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton33, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText34 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add some random commands for above Cube ID."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText34->Wrap(300);
    itemBoxSizer9->Add(itemStaticText34, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer9->Add(itemBoxSizer35, 0, wxGROW, 0);
    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel2, wxID_STATIC, _("number of commands (max 99):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer35->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 15);

    wxSpinCtrl* itemSpinCtrl37 = new wxSpinCtrl( itemPanel2, ID_SPINCTRL_CMD_CNT1, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS|wxSP_WRAP, 0, 99, 0 );
    itemBoxSizer35->Add(itemSpinCtrl37, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton38 = new wxButton( itemPanel2, ID_BUTTON_OP3, _(">> Same ID + Above Random Commands >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton38, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel2, wxID_STATIC, _("Add some random commands for other Cube."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText39->Wrap(300);
    itemBoxSizer9->Add(itemStaticText39, 0, wxGROW|wxLEFT|wxRIGHT, 10);

    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer9->Add(itemBoxSizer40, 0, wxGROW, 0);
    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel2, wxID_STATIC, _("number of commands (max 99):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemStaticText41, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 15);

    wxSpinCtrl* itemSpinCtrl42 = new wxSpinCtrl( itemPanel2, ID_SPINCTRL_CMD_CNT2, _T("0"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 0 );
    itemBoxSizer40->Add(itemSpinCtrl42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton43 = new wxButton( itemPanel2, ID_BUTTON_OP4, _(">> Other ID + Above Random Commnds >>"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton43, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 10);

    wxStaticLine* itemStaticLine44 = new wxStaticLine( itemPanel2, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer9->Add(itemStaticLine44, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel2, wxID_STATIC, _("Execute result:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText45->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Times New Roman")));
    itemBoxSizer9->Add(itemStaticText45, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel2, wxID_STATIC_RESULT, _("Everything is fine."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText46->Wrap(300);
    itemBoxSizer9->Add(itemStaticText46, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Generated raw data"));
    wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer47, 0, wxGROW|wxALL, 5);
    wxGrid* itemGrid48 = new wxGrid( itemPanel2, ID_GRID_GEN_DATA, wxDefaultPosition, wxSize(-1, 400), wxNO_BORDER );
    itemGrid48->SetDefaultColSize(20);
    itemGrid48->SetDefaultRowSize(18);
    itemGrid48->SetColLabelSize(18);
    itemGrid48->SetRowLabelSize(40);
    itemGrid48->CreateGrid(1, 16, wxGrid::wxGridSelectCells);
    itemStaticBoxSizer47->Add(itemGrid48, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer47->Add(itemBoxSizer49, 0, wxGROW, 0);
    wxArrayString itemRadioBox50Strings;
    itemRadioBox50Strings.Add(_("&Internal buffer"));
    itemRadioBox50Strings.Add(_("&User File"));
    wxRadioBox* itemRadioBox50 = new wxRadioBox( itemPanel2, ID_RADIOBOX_GEN_DATA_TO, _("Save data to..."), wxDefaultPosition, wxDefaultSize, itemRadioBox50Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox50->SetSelection(0);
    itemBoxSizer49->Add(itemRadioBox50, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5);

    itemBoxSizer49->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton52 = new wxButton( itemPanel2, ID_BUTTON_GEN_DATA_CLEAR, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer49->Add(itemButton52, 0, wxGROW|wxALL, 5);

    wxButton* itemButton53 = new wxButton( itemPanel2, ID_BUTTON_GEN_DATA_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer49->Add(itemButton53, 0, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel2, _("Generation"));

    wxPanel* itemPanel54 = new wxPanel( GetBookCtrl(), ID_PANEL_TRANSMISSION, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
    itemPanel54->SetSizer(itemBoxSizer55);

    wxStaticBox* itemStaticBoxSizer56Static = new wxStaticBox(itemPanel54, wxID_ANY, _("Data Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer56 = new wxStaticBoxSizer(itemStaticBoxSizer56Static, wxVERTICAL);
    itemBoxSizer55->Add(itemStaticBoxSizer56, 1, wxGROW|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer57Static = new wxStaticBox(itemPanel54, wxID_ANY, _("Load data from..."));
    wxStaticBoxSizer* itemStaticBoxSizer57 = new wxStaticBoxSizer(itemStaticBoxSizer57Static, wxVERTICAL);
    itemStaticBoxSizer56->Add(itemStaticBoxSizer57, 0, wxGROW|wxALL, 5);
    wxRadioButton* itemRadioButton58 = new wxRadioButton( itemPanel54, ID_RADIOBUTTON_TRANSMIT_INTERNAL_BUFFER, _("Internale buffer"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton58->SetValue(true);
    itemStaticBoxSizer57->Add(itemRadioButton58, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer57->Add(itemBoxSizer59, 0, wxGROW, 0);
    wxRadioButton* itemRadioButton60 = new wxRadioButton( itemPanel54, ID_RADIOBUTTON_TRANSMIT_USER_FILE, _("User File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton60->SetValue(false);
    itemBoxSizer59->Add(itemRadioButton60, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxFilePickerCtrl* itemFilePickerCtrl61 = new wxFilePickerCtrl( itemPanel54, ID_FILECTRL_FILE_LOCATION, _T(""), _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
    itemBoxSizer59->Add(itemFilePickerCtrl61, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer56->Add(itemBoxSizer62, 1, wxGROW|wxALL, 0);
    wxGrid* itemGrid63 = new wxGrid( itemPanel54, ID_GRID_BYTE_COUNTER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemGrid63->SetDefaultColSize(45);
    itemGrid63->SetDefaultRowSize(18);
    itemGrid63->SetColLabelSize(18);
    itemGrid63->SetRowLabelSize(20);
    itemGrid63->CreateGrid(16, 16, wxGrid::wxGridSelectCells);
    itemBoxSizer62->Add(itemGrid63, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer64Static = new wxStaticBox(itemPanel54, wxID_ANY, _("Information"));
    wxStaticBoxSizer* itemStaticBoxSizer64 = new wxStaticBoxSizer(itemStaticBoxSizer64Static, wxVERTICAL);
    itemBoxSizer62->Add(itemStaticBoxSizer64, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer65 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer65->AddGrowableCol(1);
    itemStaticBoxSizer64->Add(itemFlexGridSizer65, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText66 = new wxStaticText( itemPanel54, wxID_STATIC, _("File Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText66, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText67 = new wxStaticText( itemPanel54, wxID_STATIC_FILE_SIZE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText67, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText68 = new wxStaticText( itemPanel54, wxID_STATIC, _("Most Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText68, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText69 = new wxStaticText( itemPanel54, wxID_STATIC_MU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText69, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText70 = new wxStaticText( itemPanel54, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText70, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText71 = new wxStaticText( itemPanel54, wxID_STATIC_MU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText71, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel54, wxID_STATIC, _("Least Used Byte:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText73 = new wxStaticText( itemPanel54, wxID_STATIC_LU_BYTE, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText73, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText74 = new wxStaticText( itemPanel54, wxID_STATIC, _("Count:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText75 = new wxStaticText( itemPanel54, wxID_STATIC_LU_CNT, _("show bytes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer65->Add(itemStaticText75, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer55->Add(itemBoxSizer76, 0, wxGROW|wxALL, 0);
    wxStaticBox* itemStaticBoxSizer77Static = new wxStaticBox(itemPanel54, wxID_ANY, _("Uart Setting"));
    wxStaticBoxSizer* itemStaticBoxSizer77 = new wxStaticBoxSizer(itemStaticBoxSizer77Static, wxVERTICAL);
    itemBoxSizer76->Add(itemStaticBoxSizer77, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer77->Add(itemBoxSizer78, 0, wxGROW|wxALL, 0);
    wxButton* itemButton79 = new wxButton( itemPanel54, ID_BUTTON_CONNECT, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemButton79, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton80 = new wxButton( itemPanel54, ID_BUTTON_DISCONNECT, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemButton80, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton81 = new wxButton( itemPanel54, ID_BUTTON_SCAN_PORT, _("Rescan Port"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemButton81, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlexGridSizer* itemFlexGridSizer82 = new wxFlexGridSizer(0, 2, 0, 5);
    itemFlexGridSizer82->AddGrowableCol(1);
    itemStaticBoxSizer77->Add(itemFlexGridSizer82, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText83 = new wxStaticText( itemPanel54, wxID_STATIC, _("Serial Port:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer82->Add(itemStaticText83, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice84Strings;
    wxChoice* itemChoice84 = new wxChoice( itemPanel54, ID_CHOICE_PORT, wxDefaultPosition, wxDefaultSize, itemChoice84Strings, 0 );
    itemFlexGridSizer82->Add(itemChoice84, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText85 = new wxStaticText( itemPanel54, wxID_STATIC, _("Baud Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer82->Add(itemStaticText85, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice86Strings;
    itemChoice86Strings.Add(_("115200"));
    itemChoice86Strings.Add(_("57600"));
    itemChoice86Strings.Add(_("38400"));
    itemChoice86Strings.Add(_("19200"));
    itemChoice86Strings.Add(_("9600"));
    wxChoice* itemChoice86 = new wxChoice( itemPanel54, ID_CHOICE_BAUD, wxDefaultPosition, wxDefaultSize, itemChoice86Strings, 0 );
    itemChoice86->SetStringSelection(_("115200"));
    itemFlexGridSizer82->Add(itemChoice86, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText87 = new wxStaticText( itemPanel54, wxID_STATIC, _("Char Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer82->Add(itemStaticText87, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice88Strings;
    itemChoice88Strings.Add(_("8"));
    itemChoice88Strings.Add(_("7"));
    itemChoice88Strings.Add(_("6"));
    itemChoice88Strings.Add(_("5"));
    wxChoice* itemChoice88 = new wxChoice( itemPanel54, ID_CHOICE_CHAR_SIZE, wxDefaultPosition, wxDefaultSize, itemChoice88Strings, 0 );
    itemChoice88->SetStringSelection(_("8"));
    itemFlexGridSizer82->Add(itemChoice88, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText89 = new wxStaticText( itemPanel54, wxID_STATIC, _("Parity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer82->Add(itemStaticText89, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString itemChoice90Strings;
    itemChoice90Strings.Add(_("None"));
    itemChoice90Strings.Add(_("Odd"));
    itemChoice90Strings.Add(_("Even"));
    itemChoice90Strings.Add(_("Mark"));
    itemChoice90Strings.Add(_("Space"));
    wxChoice* itemChoice90 = new wxChoice( itemPanel54, ID_CHOICE_PARITY, wxDefaultPosition, wxDefaultSize, itemChoice90Strings, 0 );
    itemChoice90->SetStringSelection(_("None"));
    itemFlexGridSizer82->Add(itemChoice90, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText91 = new wxStaticText( itemPanel54, wxID_STATIC, _("Stop bits"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer82->Add(itemStaticText91, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice92Strings;
    itemChoice92Strings.Add(_("1"));
    itemChoice92Strings.Add(_("2"));
    wxChoice* itemChoice92 = new wxChoice( itemPanel54, ID_CHOICE_STOP_BITS, wxDefaultPosition, wxDefaultSize, itemChoice92Strings, 0 );
    itemChoice92->SetStringSelection(_("1"));
    itemFlexGridSizer82->Add(itemChoice92, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer93Static = new wxStaticBox(itemPanel54, wxID_ANY, _("Transmission Information"));
    wxStaticBoxSizer* itemStaticBoxSizer93 = new wxStaticBoxSizer(itemStaticBoxSizer93Static, wxVERTICAL);
    itemBoxSizer76->Add(itemStaticBoxSizer93, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer93->Add(itemBoxSizer94, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText95 = new wxStaticText( itemPanel54, wxID_STATIC, _("In theory, transmitting these bytes requires:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer94->Add(itemStaticText95, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText96 = new wxStaticText( itemPanel54, wxID_STATIC_THEORY_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer94->Add(itemStaticText96, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer93->Add(itemBoxSizer97, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText98 = new wxStaticText( itemPanel54, wxID_STATIC, _("In pratice, transmitting these bytes spends:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer97->Add(itemStaticText98, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText99 = new wxStaticText( itemPanel54, wxID_STATIC_PRATICE_TRANSMIT_TIME, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer97->Add(itemStaticText99, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer93->Add(itemBoxSizer100, 1, wxGROW|wxALL, 5);
    wxButton* itemButton101 = new wxButton( itemPanel54, ID_BUTTON_TRANSMIT, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemButton101, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel54, _("Transmission"));

////@end SendUart content construction
    SetupLogWindow();
    ReplaceRowColLabel();
    LoadCommand();
    LoadAppConfig();
    ScanPort();
    itemPropertySheetDialog1->dummy_ = 0;
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
    ((wxGrid *)FindWindow(ID_GRID_GEN_DATA))->SetRowLabelValue(0, wxT("0"));
    
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->SetColLabelValue(0, _("Property"));
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->SetColLabelValue(1, _("Value"));
    
    /* disable drag row col size on some grid. */
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->DisableDragRowSize();
    ((wxGrid *)FindWindow(ID_GRID_CMD_LIST))->DisableDragColSize();
    ((wxGrid *)FindWindow(ID_GRID_GEN_DATA))->DisableDragRowSize();
    ((wxGrid *)FindWindow(ID_GRID_GEN_DATA))->DisableDragColSize();
    ((wxGrid *)FindWindow(ID_GRID_BYTE_COUNTER))->DisableDragRowSize();
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
    
    wxLogVerbose(wxT("Button#") + btn->GetLabel() + wxT(" is pressed."));

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
    
    wxLogVerbose(wxT("Button#N is pressed."));

    id[0] = (wxStaticText *)FindWindow(wxID_STATIC_ID0);
    id[1] = (wxStaticText *)FindWindow(wxID_STATIC_ID1);

    for (size_t idx = 0; idx < 2; idx++)
    {
        id[idx]->ToggleWindowStyle(wxSIMPLE_BORDER);
        id[idx]->SetLabel(id[idx]->GetLabel());
    }
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
    ((wxRadioBox *)FindWindow(ID_RADIOBOX_GEN_DATA_TO))->SetSelection(numVal);
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




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_OP1
 */

void SendUart::OnButtonOp1Click( wxCommandEvent& event )
{
    wxGrid *grid = (wxGrid *)FindWindow(ID_GRID_CMD_LIST);
    wxGrid *grid2 = (wxGrid *)FindWindow(ID_GRID_GEN_DATA);
    size_t index, count;
    wxString val, actGrp, cmdGrp, cmdType;
    wxConfig *cfg = wxGetApp().m_appConfig;
    long code, max[2], min[2];
    char id[4];
    int oldRowCount;

    wxLogVerbose(wxT("Operation #1 button pressed"));

    cfg->SetPath(wxT("/CommandGroup"));
    if (grid && grid->IsSelection() && cfg->Read(wxT("ActiveGroup"), &actGrp) && cfg->HasGroup(actGrp))
    {
        // according to doc, wxString::char_str() will the return an object with string data that is implicitly
        // convertible to char* pointer; while the return type of wxString::c_str() is depended on how wx lib
        // is built. (char* in ANSI build, wchar_t* in UNICODe build)
        sprintf(id, "%c%c", (*(char *)((wxStaticText *)FindWindow(wxID_STATIC_ID0))->GetLabel().char_str()),
            (*(char *)((wxStaticText *)FindWindow(wxID_STATIC_ID1))->GetLabel().char_str()));

        cfg->SetPath(actGrp);
        count = grid->GetNumberRows();
        for (index = 0; (index < count); index++)
        {
            if (grid->IsInSelection(index, 0))
            {
                val = grid->GetCellValue(index, 1);
                cmdGrp = wxString::Format(wxT("%03d"), index + 1);
                if (!val.IsEmpty() && cfg->HasGroup(cmdGrp))
                {
                    cfg->SetPath(cmdGrp);
                    if (cfg->Read(wxT("Code"), &code) && cfg->Read(wxT("Type"), &cmdType))
                    {
                        oldRowCount = grid2->GetNumberRows();
                        if (oldRowCount * 16 - m_genDataCount < 16)
                        {
                            grid2->AppendRows();
                            grid2->SetRowLabelValue(oldRowCount, wxString::Format(wxT("%X"), oldRowCount));
                        }
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("02")));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("04")));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("%02X"), id[0]));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("%02X"), id[1]));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("%02lX"), (code >> 24)));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("%02lX"), (code & 0xFF0000) >> 16));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("%02lX"), (code & 0xFF00) >> 8));
                        m_genDataCount++;
                        grid2->SetCellValue(m_genDataCount/16, m_genDataCount%16, wxString::Format(wxT("03")));
                        m_genDataCount++;
                    }
                    cfg->SetPath(wxT(".."));
                }
            }
        }
    }
}


/*!
 * setup app log window
 */

void SendUart::SetupLogWindow(void)
{
    wxConfig *cfg = wxGetApp().m_appConfig;
    long logWinEnable, level;

    wxPanel *logPanel = new wxPanel(GetBookCtrl(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
    wxBoxSizer *itemBoxSizer = new wxBoxSizer(wxVERTICAL);
    logPanel->SetSizer(itemBoxSizer);

    logTextCtrl_ = new wxTextCtrl(logPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_RICH2|wxTE_DONTWRAP);
    itemBoxSizer->Add(logTextCtrl_, 1, wxEXPAND|wxALL, 5);

    wxBoxSizer *itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer->Add(itemBoxSizer2, 0, wxALIGN_LEFT|wxALL, 5);
    wxButton *logClearBtn = new wxButton(logPanel, wxID_ANY, _("Clear Log"), wxDefaultPosition, wxDefaultSize, 0);
    logClearBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SendUart::OnClearLog), NULL, this);
    itemBoxSizer2->Add(logClearBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    if (logTextCtrl_ && logPanel)
    {
        cfg->SetPath(wxT("/"));
        cfg->Read(wxT("EnableLogWindow"), &logWinEnable, 0L);
        if (logWinEnable != 0)
            GetBookCtrl()->AddPage(logPanel, _("Log Window"));
        else
            logPanel->Hide();
        
        wxLog *logger = new wxLogTextCtrl(logTextCtrl_);
        delete wxLog::SetActiveTarget(logger);
        wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S] "));
        
        wxArrayString levels;
        levels.Add(wxT("Verbose"));
        levels.Add(wxT("Info"));
        levels.Add(wxT("Message"));
        levels.Add(wxT("Warning"));
        levels.Add(wxT("Error"));
        wxRadioBox *logLevel = new wxRadioBox(logPanel, wxID_ANY, wxString("Log Level"), wxDefaultPosition, wxDefaultSize, levels, 1, wxRA_SPECIFY_ROWS);
        logLevel->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(SendUart::OnLogLevelChange), NULL, this);
        cfg->Read(wxT("PreferredLogLevel"), &level, 2L);
        logLevel->SetSelection(level);
        ChangeLogLevel(level);
        itemBoxSizer2->Add(logLevel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    }
}


/*!
 * wxEVT_GRID_CMD_SELECT_CELL event handler for ID_GRID_CMD_LIST
 */

void SendUart::OnGridCmdListSelectCell( wxGridEvent& event )
{
    wxString actGrp, type;
    long max1, min1, max2, min2;
    wxConfig *cfg = wxGetApp().m_appConfig;
    int rowClicked = event.GetRow();
    
    wxLogVerbose(wxT("Cell %s event: Row %d, Col %d (AltDown:%s CtrlDown:%s ShiftDown:%s MetaDown:%s)"),
        event.Selecting() ? wxT("Select") : wxT("Deselect"), rowClicked, event.GetCol(),
        event.AltDown() ? wxT("T") : wxT("F"), event.ControlDown() ? wxT("T") : wxT("F"),
        event.ShiftDown() ? wxT("T") : wxT("F"), event.MetaDown() ? wxT("T") : wxT("F"));
    
    cfg->SetPath(wxT("/CommandGroup"));
    if (event.Selecting() && cfg->Read(wxT("ActiveGroup"), &actGrp) && cfg->HasGroup(actGrp))
    {
        cfg->SetPath(actGrp);
        cfg->Read(wxString::Format(wxT("%03d/Type"), rowClicked + 1), &type);
        if (!type.Cmp(wxT("Numeric")) || !type.Cmp(wxT("Numeric2Byte")) || !type.Cmp(wxT("NumericHex")))
        {
            cfg->Read(wxString::Format(wxT("%03d/Max"), rowClicked + 1), &max1);
            cfg->Read(wxString::Format(wxT("%03d/Min"), rowClicked + 1), &min1);
            if (!type.Cmp(wxT("NumericHex")))
                ((wxStaticText *)FindWindow(wxID_STATIC_CMD_DESCRIPTION))->SetLabel(wxString::Format(wxT("This command needs one parameter value, which ranges from [%lX, %lX]. You can input number directly in the value field."), min1, max1));
            else
                ((wxStaticText *)FindWindow(wxID_STATIC_CMD_DESCRIPTION))->SetLabel(wxString::Format(wxT("This command needs one parameter value, which ranges from [%ld, %ld]. You can input number directly in the value field."), min1, max1));
        }
        else if (!type.Cmp(wxT("Pair")))
        {
            cfg->Read(wxString::Format(wxT("%03d/Max1"), rowClicked + 1), &max1);
            cfg->Read(wxString::Format(wxT("%03d/Min1"), rowClicked + 1), &min1);
            cfg->Read(wxString::Format(wxT("%03d/Max2"), rowClicked + 1), &max2);
            cfg->Read(wxString::Format(wxT("%03d/Min2"), rowClicked + 1), &min2);
            ((wxStaticText *)FindWindow(wxID_STATIC_CMD_DESCRIPTION))->SetLabel(wxString::Format(wxT("This command needs two parameter values. The first value ranges from [%ld, %ld]; the second value ranges from [%ld, %ld]. In the value field, you can input these two numbers like this => 8, -5"), min1, max1, min2, max2));
        }
        ((wxStaticText *)FindWindow(wxID_STATIC_CMD_DESCRIPTION))->Wrap(220);
    }
    event.Skip();
}


/*!
 * Clear log window
 */

void SendUart::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
    logTextCtrl_->Clear();
}

/*!
 * Change the log level
 */

void SendUart::OnLogLevelChange(wxCommandEvent& event)
{
    wxRadioBox *src = (wxRadioBox *)FindWindow(event.GetId());

    if (src)
        ChangeLogLevel(src->GetSelection());
}

/*!
 * Change the log level
 */

void SendUart::ChangeLogLevel(long level)
{
    wxLogLevel logLevel;
    
    switch (level)
    {
        case 0:
        case 1: logLevel = wxLOG_Info; break;
        case 2:
        default: logLevel = wxLOG_Message; break;
        case 3: logLevel = wxLOG_Warning; break;
        case 4: logLevel = wxLOG_Error; break;
    }
    
    wxLog::SetVerbose(level == 0);
    wxLog::SetLogLevel(logLevel);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_GEN_DATA_CLEAR
 */

void SendUart::OnButtonGenDataClearClick( wxCommandEvent& event )
{
    wxGrid *grid = (wxGrid *)FindWindow(ID_GRID_GEN_DATA);

    wxLogVerbose(wxT("Clear generated data grid."));

    if (grid)
    {
        grid->ClearGrid();
        grid->DeleteRows(1, grid->GetNumberRows() - 1);
        m_genDataCount = 0;
    }
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_GEN_DATA_CLEAR
 */

void SendUart::OnButtonGenDataClearUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_genDataCount != 0);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_GEN_DATA_SAVE
 */

void SendUart::OnButtonGenDataSaveClick( wxCommandEvent& event )
{
    wxRadioBox *rb = (wxRadioBox *)FindWindow(ID_RADIOBOX_GEN_DATA_TO);

    wxLogVerbose(wxT("Save generated data grid to ") + rb->GetStringSelection());
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_GEN_DATA_SAVE in SendUart.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_GEN_DATA_SAVE in SendUart. 
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_BUTTON_GEN_DATA_SAVE
 */

void SendUart::OnButtonGenDataSaveUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_genDataCount != 0);
}

