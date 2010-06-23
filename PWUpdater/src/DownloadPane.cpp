// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpServerThread.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(DownloadPane, wxPanel)
    EVT_BUTTON(myID_BTN_START_TFTP, DownloadPane::OnButtonStartTftp)
    EVT_BUTTON(myID_BTN_STOP_TFTP, DownloadPane::OnButtonStopTftp)
END_EVENT_TABLE()

DownloadPane::DownloadPane()
{
    Init();
}

DownloadPane::DownloadPane(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

DownloadPane::~DownloadPane()
{
}

void DownloadPane::Init()
{
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void DownloadPane::CreateControls()
{
    wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);

    paneSizer->Add(new wxButton(this, myID_BTN_START_TFTP, wxT("Start")), 0, wxALL, 5);
    paneSizer->Add(new wxButton(this, myID_BTN_STOP_TFTP, wxT("Stop")), 0, wxALL, 5);

    SetSizerAndFit(paneSizer);
}

//
// event handlers
//
void DownloadPane::OnButtonStartTftp(wxCommandEvent &event)
{
    TftpServerThread *server = new TftpServerThread(this);

    if (server
        && (server->Create() == wxTHREAD_NO_ERROR)
        && (server->Run() == wxTHREAD_NO_ERROR))
    {
        wxLogMessage(wxT("Start tftp server thread..."));
    }
}

void DownloadPane::OnButtonStopTftp(wxCommandEvent &event)
{
    wxLogMessage(wxT("Stop tftp server thread..."));
}

