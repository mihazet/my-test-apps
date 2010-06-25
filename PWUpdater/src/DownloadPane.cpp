// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "PWUpdater.h"
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
    EVT_THREAD(myID_THREAD_SERVER, DownloadPane::OnThreadTftpServer)
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

void DownloadPane::DoStartTftpServerThread()
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpServerThread *&pServer = wxGetApp().m_pTftpServerThread;

    cs.Enter();

    /* check if another tftp server thread is running... */
    if (pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is another tftp server thread running!"));
        return;
    }

    pServer = new TftpServerThread(this);

    if (pServer->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create tftp server thread!"));
        wxDELETE(pServer);
    }
    else
    {
        if (pServer->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the tftp server thread!"));
            wxDELETE(pServer);
        }
    }

    cs.Leave();
}

void DownloadPane::DoStopTftpServerThread()
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpServerThread *&pServer = wxGetApp().m_pTftpServerThread;

    cs.Enter();

    /* check if tftp server thread is already running... */
    if (!pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is no tftp server thread running."));
        return;
    }

    if (pServer->Delete() != wxTHREAD_NO_ERROR)
        wxLogError(wxT("Can't delete tftp server thread!"));

    cs.Leave();
}

//
// event handlers
//
void DownloadPane::OnButtonStartTftp(wxCommandEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Start tftp server thread..."));
    DoStartTftpServerThread();
}

void DownloadPane::OnButtonStopTftp(wxCommandEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Stop tftp server thread..."));
    DoStopTftpServerThread();
}

void DownloadPane::OnThreadTftpServer(wxThreadEvent &event)
{
    TftpServerMessage msg = event.GetPayload<TftpServerMessage>();
    int num;
    wxString str;
    wxIPV4address remote;

    msg.GetValue(num);
    msg.GetValue(str);
    remote = msg.GetRemote();
    wxLogMessage(wxT("evt = %d, num = %d, str = %s, remote ip = %s, port = %d"),
        msg.GetEvent(), num, str, remote.IPAddress(), remote.Service());
}
