/*
 *  PWUpdater.c - PixelWorks firmware update application for Ruby platform.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>
#include <wx/iconbndl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "PWUpdater.h"
#include "TftpdThread.h"
#include "RockeyThread.h"
#include "DownloadPane.h"
#include "LogPane.h"
#include "PreferenceDlg.h"
#include "WidgetsId.h"

#include "xpm/ruby_16.xpm"
#include "xpm/ruby_32.xpm"
#include "xpm/ruby_48.xpm"

#define wxLOG_COMPONENT "PWUpdater/ui/frame"

#define ID_THREAD_ROCKEY    wxID_HIGHEST + 1

// ------------------------------------------------------------------------
// Application implementation
// ------------------------------------------------------------------------
IMPLEMENT_APP(PWUpdaterApp)

void PWUpdaterApp::Init()
{
    /* application information */
    SetVendorName(wxT("delta"));
    SetVendorDisplayName(wxT("Delta Electronics, Inc."));
    SetAppName(wxT("pwupdater"));
    SetAppDisplayName(wxT("PixelWorks Ruby Platform Updater"));

    /* database */
    m_pOpt = new AppOptions;

    /* network adapter list */
    m_adapterList.clear();
    _adapterInfo = NULL;

    /* usb key state */
    m_keyFound = false;

    /* threads management */
    m_serverCS.Enter();
    m_pTftpdServerThread = NULL;
    m_serverCS.Leave();

    m_transmissionCS.Enter();
    m_tftpdTransmissionThreads.clear();
    m_transmissionCS.Leave();

    m_rockeyCS.Enter();
    m_pRockeyThread = NULL;
    m_rockeyCS.Leave();   
}

void PWUpdaterApp::Term()
{
    wxDELETE(m_pOpt);
}

bool PWUpdaterApp::OnInit()
{
    /* collect network adapter info */
    if (DetectNetAdapter())
    {
        if (m_adapterList.empty())
        {
            wxLogError(_("No network adapter is availabled, tftp server is force stopped!"));
            m_pOpt->SetOption(wxT("TftpdAutoStart"), false);
        }
    }
    else
    {
        wxLogError(_("Failed to detect network adapters info, tftp server is force stopped!"));
        m_pOpt->SetOption(wxT("TftpdAutoStart"), false);
    }

    /* 
       use default (the first) active interface when 
       (1) database is new created, or
       (2) interface store in database doesn't exist now
     */
    bool useDefaultInterface = false;
    wxString ifName = m_pOpt->GetOption(wxT("ActivedInterface"));
    if (ifName.IsEmpty())
        useDefaultInterface = true;
    else
    {
        wxVector<NetAdapter>::iterator it;
        for (it = m_adapterList.begin(); it != m_adapterList.end(); ++it)
        {
            if (ifName == it->GetName())
                break;
        }

        if (it == m_adapterList.end())
            useDefaultInterface = true;
    }
    if (useDefaultInterface && (m_adapterList.size() > 0))
        m_pOpt->SetOption(wxT("ActivedInterface"), m_adapterList.at(0).GetName());

    /*
       use current working path when
       (1) database is new created (empty), or
       (2) path stored in database doesn't exist now
     */
    bool useCurrentPath = false;
    wxString rootPath = m_pOpt->GetOption(wxT("TftpdRoot"));
    if (rootPath.empty())
        useCurrentPath = true;
    else
    {
        if (!wxFileName::DirExists(rootPath))
            useCurrentPath = true;
    }
    if (useCurrentPath)
    {
        wxStandardPaths &stdPaths = wxStandardPaths::Get();
        wxFileName exec = wxFileName(stdPaths.GetExecutablePath());
        m_pOpt->SetOption(wxT("TftpdRoot"), exec.GetPath(wxPATH_GET_VOLUME));
    }

    /* create main frame */
    PWUpdaterFrame *frame = new PWUpdaterFrame(NULL);
    frame->Show();
    return true;
}

//^^
// Description:
//      This function use platform-specific method to detect network adapter
//      information.
// Return:
//      true - successful detect network adapter info.
//      false - failed to detect network adapter info.
//
bool PWUpdaterApp::DetectNetAdapter()
{
#ifdef __WXMSW__
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    _adapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
    if (_adapterInfo == NULL)
    {
        wxLogError(_("Fail to allocate memory space for GetAdaptersInfo."));
        return false;
    }

    /* get network adapter list */
    if (GetAdaptersInfo(_adapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(_adapterInfo);
        _adapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (_adapterInfo == NULL)
        {
            wxLogError(_("Fail to allocate memory space for GetAdaptersInfo."));
            return false;
        }

        if ((dwRetVal = GetAdaptersInfo(_adapterInfo, &ulOutBufLen)) != NO_ERROR)
        {
            wxLogError(_("GetAdaptersInfo failed with error code = %d"), dwRetVal);
            return false;
        }
    }

    /* iterate list to retrieve info */
    IP_ADDR_STRING *pIpAddrString = NULL;
    wxString name, ip, netmask;
    for (IP_ADAPTER_INFO *pAdapter = _adapterInfo;
        pAdapter != NULL;
        pAdapter = pAdapter->Next)
    {
        if (pAdapter->Type == MIB_IF_TYPE_ETHERNET)
        {
            name = wxString(pAdapter->Description, *wxConvCurrent);
            /* iterate ip address list */
            for (pIpAddrString = &pAdapter->IpAddressList;
                pIpAddrString != NULL;
                pIpAddrString = pIpAddrString->Next)
            {
                ip = wxString(pIpAddrString->IpAddress.String, *wxConvCurrent);
                netmask = wxString(pIpAddrString->IpMask.String, *wxConvCurrent);
                if (ip.Cmp(wxEmptyString) && ip.Cmp(wxT("0.0.0.0"))
                    && netmask.Cmp(wxEmptyString) && netmask.Cmp(wxT("0.0.0.0")))
                {
                    NetAdapter *temp = new NetAdapter(name, ip, netmask);
                    m_adapterList.push_back(*temp);
                }
            }
        }
    }

    return true;
#elif defined (__WXGTK__)
    wxString name, ip, netmask;
#define MAX_INTERFACE   10
    struct ifconf ifc;
    struct ifreq *ifr;
    int socketFd;
    size_t numInterfaces, idx;
    char ipBuf[INET_ADDRSTRLEN];

    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1)
    {
        wxLogError(_("Fail to create UDP socket. errno = %d"), errno);
        return false;
    }

    /* get the active interface list */
    _adapterInfo = (struct ifreq *)malloc(sizeof(struct ifreq) * MAX_INTERFACE);
    ifc.ifc_buf = (char *)_adapterInfo;
    ifc.ifc_len = sizeof(struct ifreq) * MAX_INTERFACE;
    if (ioctl(socketFd, SIOCGIFCONF, &ifc) != 0)
    {
        wxLogError(_("Fail to retrieve interface configuration."));
        free(ifc.ifc_buf);
        return false;
    }

    /* retrieve info from list */
    numInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for (idx = 0; idx < numInterfaces; idx++)
    {
        ifr = &ifc.ifc_req[idx];

        /* name */
        name = wxString(ifr->ifr_name, *wxConvCurrent);
        
        if (!name.Cmp(wxT("lo")))
            continue;

        /* ip */
        memset(ipBuf, 0, INET_ADDRSTRLEN);
        struct sockaddr *addr = &(ifr->ifr_addr);
        inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr),
            ipBuf, INET_ADDRSTRLEN);
        ip = wxString(ipBuf, *wxConvCurrent);

        /* netmask */
        if (ioctl(socketFd, SIOCGIFNETMASK, ifr) != 0)
        {
            wxLogError(_("Fail to get netmask address!"));
            return false;
        }
        memset(ipBuf, 0, INET_ADDRSTRLEN);
        addr = &(ifr->ifr_netmask);
        inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr),
            ipBuf, INET_ADDRSTRLEN);
        netmask = wxString(ipBuf, *wxConvCurrent);

        if (ip.Cmp(wxEmptyString) && ip.Cmp(wxT("0.0.0.0"))
            && netmask.Cmp(wxEmptyString) && netmask.Cmp(wxT("0.0.0.0")))
        {
            NetAdapter *temp = new NetAdapter(name, ip, netmask);
            m_adapterList.push_back(*temp);
        }
    }

    return true;
#else
    return false;
#endif
}

// ------------------------------------------------------------------------
// Main frame implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(PWUpdaterFrame, wxFrame)
    EVT_CLOSE(PWUpdaterFrame::OnClose)
    EVT_MENU(wxID_EXIT, PWUpdaterFrame::OnQuit)
    EVT_MENU(wxID_PREFERENCES, PWUpdaterFrame::OnPref)
    EVT_THREAD(ID_THREAD_ROCKEY, PWUpdaterFrame::OnRockey)
END_EVENT_TABLE()

PWUpdaterFrame::PWUpdaterFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos, const wxSize &size,
    long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

PWUpdaterFrame::~PWUpdaterFrame()
{
    _auiMgr.UnInit();
}

bool PWUpdaterFrame::Create(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos, const wxSize &size,
    long style)
{
    bool result;

    /* create gui */
    result = wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();

    /* create threads */
    StartRockeyThread();

    return result;
}

void PWUpdaterFrame::Init()
{
}

void PWUpdaterFrame::CreateControls()
{
    /* icons */
    wxIconBundle icons;
    icons.AddIcon(wxIcon(ruby_16_xpm));
    icons.AddIcon(wxIcon(ruby_32_xpm));
    icons.AddIcon(wxIcon(ruby_48_xpm));
    SetIcons(icons);

    /* menu bar */
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("&Quit"), _("Quit this program."));
    wxMenu *view_menu = new wxMenu;
    view_menu->Append(wxID_PREFERENCES, _("&Preferences"), _("Modify user configuration."));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(file_menu, _("&File"));
    menuBar->Append(view_menu, _("&View"));
    SetMenuBar(menuBar);

    /* tool bar */

    /* status bar */

    /* aui manager & panes */
    _auiMgr.SetManagedWindow(this);
    _auiMgr.SetFlags(_auiMgr.GetFlags() | wxAUI_MGR_LIVE_RESIZE);

    _auiMgr.AddPane(new LogPane(this),
        wxAuiPaneInfo().Name(wxT("LogPane")).Caption(_("Log Window")).
        CloseButton(true).DestroyOnClose(false).MinSize(560, 420).
        Float());
    _auiMgr.AddPane(new DownloadPane(this),
        wxAuiPaneInfo().Name(wxT("DownloadPane")).CaptionVisible(false).
        Center().CloseButton(false).DestroyOnClose(false).
        MaximizeButton(true));

    _auiMgr.Update();
}

void PWUpdaterFrame::StartRockeyThread()
{
    wxCriticalSection &cs = wxGetApp().m_rockeyCS;
    RockeyThread *&pRockey = wxGetApp().m_pRockeyThread;

    cs.Enter();

    /* check if another rockey thread is running... */
    if (pRockey)
    {
        cs.Leave();
        wxLogWarning(_("Rockey thread is already running!"));
        return;
    }

    pRockey = new RockeyThread(this, ID_THREAD_ROCKEY);

    if (pRockey->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(_("Can't create rockey thread!"));
        wxDELETE(pRockey);
    }
    else
    {
        if (pRockey->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(_("Can't run the rockey thread!"));
            wxDELETE(pRockey);
        }
    }

    cs.Leave();
}

void PWUpdaterFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;
    wxCriticalSection &cs2 = wxGetApp().m_transmissionCS;
    wxVector<TftpdTransmissionThread *> &transmissions
        = wxGetApp().m_tftpdTransmissionThreads;
    wxVector<TftpdTransmissionThread *>::iterator it;
    wxCriticalSection &cs3 = wxGetApp().m_rockeyCS;
    RockeyThread *&pRockey = wxGetApp().m_pRockeyThread;
    bool serverTerminated = false;
    bool allTransmissionTerminated = false;
    bool rockeyTerminated = false;

    /* delete tftp server thread if it is still running... */
    cs.Enter();
    if (pServer)
        pServer->Delete();
    cs.Leave();

    /* delete tftp transmission threads if any are still running... */
    cs2.Enter();
    for (it = transmissions.begin(); it != transmissions.end(); ++it)
    {
        if (*it)
            (*it)->Delete();
    }
    cs2.Leave();

    /* delete rockey thread if it is still running... */
    cs3.Enter();
    if (pRockey)
        pRockey->Delete();
    cs3.Leave();

    /* make sure tftp server and transmissions terminated. */
    while (true)
    {
        if (!serverTerminated)
        {
            cs.Enter();
            if (!pServer)
                serverTerminated = true;
            cs.Leave();
        }

        if (!allTransmissionTerminated)
        {
            allTransmissionTerminated = true;
            cs2.Enter();
            for (it = transmissions.begin(); it != transmissions.end(); ++it)
            {
                if (*it != NULL)
                {
                    allTransmissionTerminated = false;
                    break;
                }
            }
            cs2.Leave();
        }

        if (!rockeyTerminated)
        {
            cs3.Enter();
            if (!pRockey)
                rockeyTerminated = true;
            cs3.Leave();
        }

        if (serverTerminated && allTransmissionTerminated && rockeyTerminated)
            break;

        /* give the tftp server a chance to terminated. */
        wxMilliSleep(100);
    }

    Destroy();
}

void PWUpdaterFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

void PWUpdaterFrame::OnPref(wxCommandEvent &WXUNUSED(event))
{
    PrefDlg dlg(this, myID_PREF_DLG, wxGetApp().m_keyFound);

    dlg.ShowModal();
}

void PWUpdaterFrame::OnRockey(wxThreadEvent &event)
{
    RockeyMessage msg = event.GetPayload<RockeyMessage>();
    int evt = msg.GetEvent();

    /* iterate all top level windows to check if PrefDilg is on */
    wxWindowList::iterator it;
    for (it = wxTopLevelWindows.begin(); it != wxTopLevelWindows.end(); ++it)
    {
        wxWindow *tlw = *it;
        if (tlw && (tlw->GetId() == myID_PREF_DLG))
        {
            PrefDlg *dlg = wxDynamicCast(tlw, PrefDlg);

            if (evt == ROCKEY_EVENT_KEY_INSERTED)
                dlg->AddAuthorizedPage();
            else if (evt == ROCKEY_EVENT_KEY_REMOVED)
                dlg->RemoveAuthorizedPage();
            break;
        }
    }

    /* update global flag in frame */
    if (evt == ROCKEY_EVENT_KEY_INSERTED)
        wxGetApp().m_keyFound = true;
    else if (evt == ROCKEY_EVENT_KEY_REMOVED)
        wxGetApp().m_keyFound = false;
}
