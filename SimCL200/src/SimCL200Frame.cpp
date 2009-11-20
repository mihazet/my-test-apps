// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "SimCL200Frame.h"

#include "port_32_16.xpm"
#include "baud_32_16.xpm"
#include "csize_32_16.xpm"
#include "parity_32_16.xpm"
#include "stopb_32_16.xpm"

enum
{
    myID_PORT = wxID_HIGHEST + 1,
    myID_BAUDRATE,
    myID_CHARSIZE,
    myID_STOPBITS,
    myID_PARITY,
};

IMPLEMENT_CLASS(SimCL200Frame, wxFrame)

BEGIN_EVENT_TABLE(SimCL200Frame, wxFrame)
    EVT_RIBBONBUTTONBAR_CLICKED(myID_BAUDRATE, SimCL200Frame::OnBaudRateClicked)
    EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(myID_BAUDRATE, SimCL200Frame::OnBaudRateDropDown)
END_EVENT_TABLE()

SimCL200Frame::SimCL200Frame()
{
    Init();
}

SimCL200Frame::SimCL200Frame(wxWindow *parent, wxWindowID id,
                             const wxString &caption, const wxPoint &pos,
                             const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool SimCL200Frame::Create(wxWindow *parent, wxWindowID id,
                           const wxString &caption, const wxPoint &pos,
                           const wxSize &size, long style)
{
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();
    Centre();
    return true;
}

SimCL200Frame::~SimCL200Frame()
{
}

void SimCL200Frame::Init()
{
}

void SimCL200Frame::CreateControls()
{
    SetStatusBar(CreateStatusBar());

    m_ribbon = new wxRibbonBar(this);

    /* serial port page */
    wxRibbonPage *portPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("SerialPort"));
    wxRibbonPanel *connectionPanel = new wxRibbonPanel(portPage, wxID_ANY, wxT("Connection"), wxNullBitmap,
        wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
    wxRibbonToolBar *connectionToolBar = new wxRibbonToolBar(connectionPanel);
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Serial port selection"), wxRIBBON_BUTTON_HYBRID);
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Scan all not used port on system")); // scan
    connectionToolBar->AddSeparator();
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Connect to specific port")); // connect
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Disconnect with specific port")); // disconnect
    connectionToolBar->SetRows(2, 3);
    
    wxRibbonPanel *configPanel = new wxRibbonPanel(portPage, wxID_ANY, wxT("Configuration"), wxNullBitmap,
        wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
    wxRibbonButtonBar *configBtnBar = new wxRibbonButtonBar(configPanel);
    configBtnBar->AddButton(myID_BAUDRATE, wxT("Baud Rate"), baud_32_16_xpm, wxT("Baud Rate Selection"), wxRIBBON_BUTTON_HYBRID);
    configBtnBar->AddButton(myID_CHARSIZE, wxT("Char Size"), csize_32_16_xpm, wxT("Character Size Selection"));
    configBtnBar->AddButton(myID_PARITY, wxT("Parity"), parity_32_16_xpm, wxT("Parity Selection"), wxRIBBON_BUTTON_DROPDOWN);
    configBtnBar->AddButton(myID_STOPBITS, wxT("Stop Bit"), stopb_32_16_xpm, wxT("Stop Bit Selection"));

    /* dummy page */
    new wxRibbonPage(m_ribbon, wxID_ANY, wxT("CL-200"));

    m_ribbon->Realize();

    wxTextCtrl *log = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);

    wxSizer *frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(m_ribbon, 0, wxEXPAND);
    frameSizer->Add(log, 1, wxEXPAND);

    SetSizerAndFit(frameSizer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimCL200Frame::OnBaudRateClicked(wxRibbonButtonBarEvent &WXUNUSED(event))
{
    wxLogDebug(wxT("Baud rate button clicked"));
}

void SimCL200Frame::OnBaudRateDropDown(wxRibbonButtonBarEvent &WXUNUSED(event))
{
    wxLogDebug(wxT("Baud rate drop down clicked"));
}
