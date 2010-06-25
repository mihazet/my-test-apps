// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpServerThread.h"
#include "TftpTransmissionThread.h"

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

    paneSizer->Add(new wxButton(this, myID_BTN_START_TFTP, wxT("Start Server")), 0, wxALL, 5);
    paneSizer->Add(new wxButton(this, myID_BTN_STOP_TFTP, wxT("Stop Server")), 0, wxALL, 5);

    wxButton *startTx = new wxButton(this, wxID_ANY, wxT("Start Transfer"));
    startTx->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DownloadPane::OnButtonStartTransfer, this);
    paneSizer->Add(startTx, 0, wxALL, 5);

    wxButton *stopTx = new wxButton(this, wxID_ANY, wxT("Stop Transfer"));
    stopTx->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DownloadPane::OnButtonStopTransfer, this);
    paneSizer->Add(stopTx, 0, wxALL, 5);

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

void DownloadPane::DoStartTftpTransmissionThread(const wxIPV4address &remote,
                                                 const wxString &file,
                                                 bool read, int mode)
{
    wxCriticalSection &cs = wxGetApp().m_transmissionCS;
    wxVector<TftpTransmissionThread *> &transmissions
        = wxGetApp().m_tftpTransmissionThreads;
    TftpTransmissionThread *pTransmission;
    bool done = true;

    cs.Enter();

    pTransmission = new TftpTransmissionThread(this, remote, file, read, mode);

    if (pTransmission->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create tftp transmission thread!"));
        wxDELETE(pTransmission);
        done = false;
    }
    else
    {
        if (pTransmission->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the tftp server thread!"));
            wxDELETE(pTransmission);
            done = false;
        }
    }

    if (done)
        transmissions.push_back(pTransmission);

    cs.Leave();
}

void DownloadPane::DoStopTftpTransmissionThread()
{
    wxCriticalSection &cs = wxGetApp().m_transmissionCS;
    wxVector<TftpTransmissionThread *> &transmissions
        = wxGetApp().m_tftpTransmissionThreads;
    wxVector<TftpTransmissionThread *>::iterator it;

    cs.Enter();

    for (it = transmissions.begin(); it != transmissions.end(); ++it)
    {
        if (*it)
        {
            if ((*it)->Delete() != wxTHREAD_NO_ERROR)
                wxLogError(wxT("Can't delete tftp transmission thread!"));
        }
    }

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

void DownloadPane::OnButtonStartTransfer(wxCommandEvent &WXUNUSED(event))
{
    wxIPV4address dummy;
    wxLogMessage(wxT("Start tftp transfer thread..."));
    DoStartTftpTransmissionThread(dummy, wxT("StartTransferTest"),
        true, TFTP_TRANSFER_MODE_BINARY);
}

void DownloadPane::OnButtonStopTransfer(wxCommandEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Stop tftp transfer thread..."));
    DoStopTftpTransmissionThread();
}

void DownloadPane::OnThreadTftpServer(wxThreadEvent &event)
{
    TftpServerMessage msg = event.GetPayload<TftpServerMessage>();
    int mode = TFTP_TRANSFER_MODE_BINARY, ec = -1;
    wxString fileName, errorMsg;
    wxIPV4address remote;
    bool error = false, read = false;

    switch (msg.GetEvent())
    {
    case TFTP_SERVER_MSG_READ_REQUEST:
        read = true;
    case TFTP_SERVER_MSG_WRITE_REQUEST:
        msg.GetValue(mode);
        msg.GetValue(fileName);
        remote = msg.GetRemote();
        break;
    case TFTP_SERVER_MSG_ERROR:
        msg.GetValue(ec);
        msg.GetValue(errorMsg);
        remote = msg.GetRemote();
        error = true;
        break;
    default:
        wxLogError(wxT("Unknown TFTP Server messge received! (%d"), msg.GetEvent());
        return;
    }

    if (error)
    {
        wxLogMessage(wxT("Error reported by TFTP Client. Error Code = %d, Reason = %s"),
            ec, errorMsg);
    }
    else
    {
        DoStartTftpTransmissionThread(remote, fileName, read, mode);
    }
}
