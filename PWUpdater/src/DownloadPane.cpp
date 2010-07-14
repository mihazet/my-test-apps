// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpdThread.h"
#include "PWUpdater.h"
#include "AppOptions.h"
#include "NetAdapter.h"

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
    EVT_THREAD(myID_THREAD_TFTPD, DownloadPane::OnThreadTftpd)
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
    /* create ui */
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();

    /* start internal tftp server if need */
    bool useInternalTftp = false;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    wxVector<NetAdapter> &adapterList = wxGetApp().m_adapterList;
    wxVector<NetAdapter>::iterator it;
    if ((pOpt->GetOption(wxT("UseInternalTftp"), &useInternalTftp)) && useInternalTftp)
    {
        wxString intf, tftpRoot;
        if ((pOpt->GetOption(wxT("ActivedInterface"), intf))
            && (pOpt->GetOption(wxT("TftpdRoot"), tftpRoot)))
        {
            for (it = adapterList.begin(); it != adapterList.end(); ++it)
            {
                if (it->GetName() == intf)
                {
                    DoStartTftpServerThread(it->GetIp(), tftpRoot);
                    break;
                }
            }
        }
    }
    else
    {
        /* TODO: create a tftp client thread here to communicate with 
           external tftp server and establish the download file list.
         */
    }

    return true;
}

void DownloadPane::CreateControls()
{
    wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);

    SetSizerAndFit(paneSizer);
}

void DownloadPane::DoStartTftpServerThread(const wxString &ipAddr,
                                           const wxString &root)
{
    wxIPV4address local;
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;
    
    /* assign preferred interface */
    if (ipAddr.empty())
        local.AnyAddress();
    else
        local.Hostname(ipAddr);

    cs.Enter();

    /* check if another tftp server thread is running... */
    if (pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is another tftp server thread running!"));
        return;
    }

    pServer = new TftpdServerThread(this, myID_THREAD_TFTPD, local, root);

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
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;

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
void DownloadPane::OnThreadTftpd(wxThreadEvent &event)
{
    TftpdMessage msg = event.GetPayload<TftpdMessage>();
    int transferMode, errorCode, currentBlock, totalBlock;
    wxString fileName, errorMessage;

    switch (msg.GetEvent())
    {
    case TFTPD_EVENT_READ_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_READ_TRANSFER_UPDATE:
    case TFTPD_EVENT_READ_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_WRITE_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_WRITE_TRANSFER_UPDATE:
    case TFTPD_EVENT_WRITE_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_ERROR:
        errorCode = msg.GetNum1();
        errorMessage = msg.GetString();
        wxLogMessage(wxT("Tftp error: error code = %d, error message = %s"),
            errorCode, errorMessage);
        break;
    default:
        wxLogError(wxT("Unknown TFTPD messge received! (%d"), msg.GetEvent());
        return;
    }
}
