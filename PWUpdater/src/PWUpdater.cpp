/*
 *  PWUpdater.c - PixelWorks firmware update application for Ruby platform.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  $Id$
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
#include <wx/aboutdlg.h>
#include "PWUpdater.h"
#include "TftpdThread.h"
#include "RockeyThread.h"
#include "UartThread.h"
#include "DownloadPane.h"
#include "LogPane.h"
#include "PreferenceDlg.h"
#include "WidgetsId.h"
#include "version.h"

#include "xpm/ruby_16.xpm"
#include "xpm/ruby_32.xpm"
#include "xpm/ruby_48.xpm"

#define wxLOG_COMPONENT "PWUpdater/ui/frame"

class AppLanguageInfo
{
public:
    AppLanguageInfo(wxLanguage langId, const wxString &iso639)
    {
        m_langId = langId;
        m_iso639 = iso639;
        m_description = wxEmptyString;
        m_installed = false;
    }
    wxLanguage m_langId;
    wxString m_iso639;
    wxString m_description;
    bool m_installed;
};

// ------------------------------------------------------------------------
// Application implementation
// ------------------------------------------------------------------------
IMPLEMENT_APP(PWUpdaterApp)

void PWUpdaterApp::Init()
{
    /* application information */
    SetVendorName(wxT("delta"));
    SetVendorDisplayName(wxT("Delta Electronics, Inc."));
    SetAppName(wxT("PWUpdater"));
    SetAppDisplayName(wxT("PixelWorks Ruby Platform Updater"));

    /* database */
    m_pOpt = new AppOptions;

    /* network adapter list */
    m_adapterList.clear();

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

    m_uartCS.Enter();
    m_pUartThread = NULL;
    m_uartCS.Leave();

    /* thread queue */
    m_pUartQueue = new ThreadSafeQueue<UartMessage>;

    /* create supported language map */
    AddSupportedLanguages();
}

void PWUpdaterApp::Term()
{
    wxDELETE(m_pOpt);
    wxDELETE(m_pUartQueue);

    wxVector<AppLanguageInfo *>::iterator it;
    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
        wxDELETE(*it);
}

bool PWUpdaterApp::OnInit()
{
    /* init locale */
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString localePath = wxFileName(stdPaths.GetExecutablePath()).GetPath(true) + wxT("locale");
    if (!_locale.Init(DetectInstalledLanguages()))
    {
        wxLogWarning(wxT("Selected language is not supported by system."));
    }
    _locale.AddCatalogLookupPathPrefix(localePath);
    _locale.AddCatalog(GetAppName());
    _locale.AddCatalog(wxT("wxstd"));

    /* add description after locale initialization, so that description can be translated. */
    AddLanguageDescriptions();

    /* collect network adapter info */
    if (DetectNetAdapter())
    {
        if (m_adapterList.empty())
        {
            wxLogError(_("No network adapter is availabled, force to use external tftp server!"));
            m_pOpt->SetOption(wxT("UseInternalTftp"), false);
        }
    }
    else
    {
        wxLogError(_("Failed to detect network adapters info, application will be terminated!"));
        return false;
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
       (1) path stored in database doesn't exist now any more
     */
    bool useCurrentPath = false;
    wxString rootPath = m_pOpt->GetOption(wxT("TftpdRoot"));
    if (!rootPath.empty())
    {
        if (!wxFileName::DirExists(rootPath))
            useCurrentPath = true;
    }
    if (useCurrentPath)
        m_pOpt->SetOption(wxT("TftpdRoot"), wxString(wxEmptyString));

    /* create main frame */
    PWUpdaterFrame *frame = new PWUpdaterFrame(NULL, myID_FRAME);
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
    IP_ADAPTER_INFO *pAdapterInfo = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
    if (pAdapterInfo == NULL)
    {
        wxLogError(_("Fail to allocate memory space for GetAdaptersInfo."));
        return false;
    }

    /* get network adapter list */
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (pAdapterInfo == NULL)
        {
            wxLogError(_("Fail to allocate memory space for GetAdaptersInfo."));
            return false;
        }

        if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR)
        {
            free(pAdapterInfo);
            wxLogError(_("GetAdaptersInfo failed with error code = %d"), dwRetVal);
            return false;
        }
    }

    /* iterate list to retrieve info */
    IP_ADDR_STRING *pIpAddrString = NULL;
    wxString name, ip, netmask;
    for (IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
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
                    delete temp;
                }
            }
        }
    }

    free(pAdapterInfo);
    return true;
#elif defined (__WXGTK__)
    wxString name, ip, netmask;
#define MAX_INTERFACE   10
    struct ifreq *pAdapterInfo = NULL;
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
    pAdapterInfo = (struct ifreq *)malloc(sizeof(struct ifreq) * MAX_INTERFACE);
    ifc.ifc_buf = (char *)pAdapterInfo;
    ifc.ifc_len = sizeof(struct ifreq) * MAX_INTERFACE;
    if (ioctl(socketFd, SIOCGIFCONF, &ifc) != 0)
    {
        wxLogError(_("Fail to retrieve interface configuration."));
        free(pAdapterInfo);
        close(socketFd);
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
            free(pAdapterInfo);
            close(socketFd);
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
            delete temp;
        }
    }

    free(pAdapterInfo);
    close(socketFd);
    return true;
#else
    return false;
#endif
}

void PWUpdaterApp::AddSupportedLanguages()
{
    /* add all supported language here, the iso639 name will also be treated
       as directory name under the locale directory */

    AddSupportedLanguage(wxLANGUAGE_ENGLISH, wxT("en"));
    AddSupportedLanguage(wxLANGUAGE_CHINESE_TRADITIONAL, wxT("zh_TW"));
    AddSupportedLanguage(wxLANGUAGE_CHINESE_SIMPLIFIED, wxT("zh_CN"));
    AddSupportedLanguage(wxLANGUAGE_FRENCH, wxT("fr_FR"));
    AddSupportedLanguage(wxLANGUAGE_GERMAN, wxT("de_DE"));
    AddSupportedLanguage(wxLANGUAGE_JAPANESE, wxT("ja_JP"));
    AddSupportedLanguage(wxLANGUAGE_KOREAN, wxT("ko_KR"));
    AddSupportedLanguage(wxLANGUAGE_RUSSIAN, wxT("ru_RU"));
    AddSupportedLanguage(wxLANGUAGE_SPANISH, wxT("es_ES"));
}

void PWUpdaterApp::AddLanguageDescriptions()
{
    AddLanguageDescription(wxLANGUAGE_ENGLISH, _("English"));
    AddLanguageDescription(wxLANGUAGE_CHINESE_TRADITIONAL, _("Traditional Chinese"));
    AddLanguageDescription(wxLANGUAGE_CHINESE_SIMPLIFIED, _("Simplified Chinese"));
    AddLanguageDescription(wxLANGUAGE_FRENCH, _("French"));
    AddLanguageDescription(wxLANGUAGE_GERMAN, _("German"));
    AddLanguageDescription(wxLANGUAGE_JAPANESE, _("Japanese"));
    AddLanguageDescription(wxLANGUAGE_KOREAN, _("Korean"));
    AddLanguageDescription(wxLANGUAGE_RUSSIAN, _("Russian"));
    AddLanguageDescription(wxLANGUAGE_SPANISH, _("Spanish"));
}

void PWUpdaterApp::AddSupportedLanguage(wxLanguage langId, const wxString &iso639)
{
    bool already_exist = false;
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_langId == langId)
        {
            already_exist = true;
            break;
        }
    }

    if (already_exist) // replace the old one
    {
        wxLogWarning(_("ISO639 code of language '%d' is changing from '%s' to '%s'."),
            (*it)->m_langId, (*it)->m_iso639, iso639);
        (*it)->m_iso639 = iso639;
    }
    else // create a new one
    {
        AppLanguageInfo *langInfo = new AppLanguageInfo(langId, iso639);
        _appLanguages.push_back(langInfo);
    }
}

void PWUpdaterApp::AddLanguageDescription(wxLanguage langId, const wxString &desc)
{
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_langId == langId)
            break;
    }

    if (it != _appLanguages.end())
    {
        (*it)->m_description = desc;
    }
    else
    {
        wxLogError(_("Add description '%s' for unsupported language '%d'"), desc, langId);
    }
}

wxLanguage PWUpdaterApp::DetectInstalledLanguages()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString localePath = wxFileName(stdPaths.GetExecutablePath()).GetPath(true) + wxT("locale");
    wxString selectedLang = m_pOpt->GetOption(wxT("Language"));
    wxLanguage result = wxLANGUAGE_ENGLISH;
    bool selectedIsInstalled = false;
    wxVector<AppLanguageInfo *>::iterator it;

    /* scan dir to check if each supported language is installed or not */
    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if (wxFileName::DirExists(localePath + wxFileName::GetPathSeparator() + (*it)->m_iso639))
        {
            (*it)->m_installed = true;
            if (selectedLang == (*it)->m_iso639)
            {
                selectedIsInstalled = true;
                result = (*it)->m_langId;
            }
        }
    }

    /* fallback to english if selected language is not installed */
    if (!selectedIsInstalled)
        result = wxLANGUAGE_ENGLISH;

    return result;
}

wxString PWUpdaterApp::GetLanguageDescriptionFromISO639Code(const wxString &iso639)
{
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_iso639 == iso639)
            return (*it)->m_description;
    }

    return wxEmptyString;
}

wxString PWUpdaterApp::GetLanguageISO639CodeFromDescription(const wxString &desc)
{
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_description == desc)
            return (*it)->m_iso639;
    }

    return wxEmptyString;
}

wxVector<wxString> PWUpdaterApp::GetInstalledLanguages()
{
    wxVector<wxString> result;
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_installed)
            result.push_back((*it)->m_iso639);
    }

    if (result.empty())
        result.push_back(wxT("en"));

    return result;
}

// ------------------------------------------------------------------------
// Main frame implementation
// ------------------------------------------------------------------------
#define PANE_NAME_LOG       wxT("LogPane")
#define PANE_NAME_DOWNLOAD  wxT("DownloadPane")

BEGIN_EVENT_TABLE(PWUpdaterFrame, wxFrame)
    EVT_CLOSE(PWUpdaterFrame::OnClose)
    EVT_MENU(wxID_EXIT, PWUpdaterFrame::OnQuit)
    EVT_MENU(wxID_PREFERENCES, PWUpdaterFrame::OnPref)
    EVT_MENU(wxID_ABOUT, PWUpdaterFrame::OnAbout)
    EVT_MENU_RANGE(myID_MBAR_VIEW_START, myID_MBAR_VIEW_END, PWUpdaterFrame::OnView)
    EVT_UPDATE_UI_RANGE(myID_MBAR_VIEW_START, myID_MBAR_VIEW_END, PWUpdaterFrame::OnUpdateUIView)
    EVT_THREAD(myID_THREAD_ROCKEY, PWUpdaterFrame::OnRockey)
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
    SetClientSize(740, 460);

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
    view_menu->AppendCheckItem(myID_MBAR_VIEW_LOG, _("Log Message\tCTRL+F1"), _("Show or hide the log message."));
    view_menu->AppendSeparator();
    view_menu->Append(wxID_PREFERENCES, _("&Preferences"), _("Modify user configuration."));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT, _("&About"), _("About this program."));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(file_menu, _("&File"));
    menuBar->Append(view_menu, _("&View"));
    menuBar->Append(help_menu, _("&Help"));
    SetMenuBar(menuBar);

    /* tool bar */

    /* status bar */
    wxStatusBar *statBar = new wxStatusBar(this, myID_FRAME_STATUSBAR, wxSTB_DEFAULT_STYLE);
    int stBarWidths[STATBAR_FLD_MAX] = { -1, -3 };
    statBar->SetFieldsCount(STATBAR_FLD_MAX);
    statBar->SetStatusWidths(STATBAR_FLD_MAX, stBarWidths);
    SetStatusBar(statBar);

    /* aui manager & panes */
    _auiMgr.SetManagedWindow(this);
    _auiMgr.SetFlags(_auiMgr.GetFlags() | wxAUI_MGR_LIVE_RESIZE);

    _auiMgr.AddPane(new LogPane(this),
        wxAuiPaneInfo().Name(PANE_NAME_LOG).Caption(_("Log Window")).
        CloseButton(true).DestroyOnClose(false).MinSize(750, 120).
        Bottom().LeftDockable(false).RightDockable(false).TopDockable(false).
        Hide());
    _auiMgr.AddPane(new DownloadPane(this, myID_PANE_DOWNLOAD),
        wxAuiPaneInfo().Name(PANE_NAME_DOWNLOAD).CaptionVisible(false).
        CenterPane().CloseButton(false).DestroyOnClose(false));

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

    pRockey = new RockeyThread(this, myID_THREAD_ROCKEY);

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
    wxCriticalSection &cs4 = wxGetApp().m_uartCS;
    UartThread *&pUart = wxGetApp().m_pUartThread;
    bool serverTerminated = false;
    bool allTransmissionTerminated = false;
    bool rockeyTerminated = false;
    bool uartTerminated = false;

    /* for uart thread termination, we send custom event to queue instead of wxThread::Delete() */
    ThreadSafeQueue<UartMessage> *&pUartQueue = wxGetApp().m_pUartQueue;
    UartMessage message(UART_EVENT_QUIT);

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

    /* delete uart thread if it is still running... */
    pUartQueue->EnQueue(message);

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

        if (!uartTerminated)
        {
            cs4.Enter();
            if (!pUart)
                uartTerminated = true;
            cs4.Leave();
        }

        if (serverTerminated && allTransmissionTerminated
            && rockeyTerminated && uartTerminated)
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

void PWUpdaterFrame::OnView(wxCommandEvent &event)
{
    int objId = event.GetId();
    wxString paneName;
    bool invalid = false;

    switch (objId)
    {
    case myID_MBAR_VIEW_LOG:
        paneName = PANE_NAME_LOG;
        break;
    default:
        invalid = true;
        break;
    }

    if (!invalid)
    {
        _auiMgr.GetPane(paneName).Show(event.IsChecked());
        _auiMgr.Update();
    }
}

void PWUpdaterFrame::OnUpdateUIView(wxUpdateUIEvent &event)
{
    int objId = event.GetId();
    wxString paneName;
    bool invalid = false;

    switch (objId)
    {
    case myID_MBAR_VIEW_LOG:
        paneName = PANE_NAME_LOG;
        break;
    default:
        invalid = true;
        break;
    }

    if (!invalid)
        event.Check(_auiMgr.GetPane(paneName).IsShown());
}

void PWUpdaterFrame::OnPref(wxCommandEvent &WXUNUSED(event))
{
    PrefDlg dlg(this, myID_PREF_DLG, wxGetApp().m_keyFound);

    dlg.ShowModal();
}

void PWUpdaterFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxAboutDialogInfo info;
    wxString copyright, description, version, longVersion;
    wxString &user = wxGetApp().m_user;
    wxString &contact = wxGetApp().m_contact;

    copyright
        << _("Copyright (c) 2010, Delta Electronics, Inc. All Rights Reserved.");
    description
        << _("This program is able to upgrade the firmware and/or bootloader\n")
        << _("into flash memory chips for the Pixelworks Ruby platform.\n")
        << wxT("\n") << _("Operator: ");
    if (!user.empty() || !contact.empty())
        description << user;
    else
    {
        description
            << _("anonymous") << wxT("\n\n")
            << _("No valid USB dongle inserted, the following features are disabled\n")
            << _("1. Manage flash layout setting\n")
            << _("2. Upgrade bootloader\n");
    }
    version
        << VER_MAJOR_STRING << "." << VER_MINOR_STRING << "." << VER_RELEASE_STRING;
    longVersion = version + "." + VER_BUILD_STRING;

    info.SetDescription(description);
    info.SetCopyright(copyright);
    info.SetIcon(wxIcon(ruby_48_xpm));
    info.SetName(wxT("PWUpdater"));
    info.SetVersion(version, longVersion);

    wxAboutBox(info, this);
}

void PWUpdaterFrame::OnRockey(wxThreadEvent &event)
{
    DownloadPane *pDownloadPane = wxDynamicCast(FindWindow(myID_PANE_DOWNLOAD), DownloadPane);
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
    {
        wxGetApp().m_keyFound = true;
        wxGetApp().m_user = msg.GetUser();
        wxGetApp().m_contact = msg.GetContact();
    }
    else if (evt == ROCKEY_EVENT_KEY_REMOVED)
    {
        wxGetApp().m_keyFound = false;
        wxGetApp().m_user = wxEmptyString;
        wxGetApp().m_contact = msg.GetContact();
    }

    /* key state changed, need to rescan download files */
    if (pDownloadPane)
    {
        pDownloadPane->RescanImageFiles();
    }
}
