#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/tipwin.h>
#include <wx/wxsqlite3.h>
#include <wx/tokenzr.h>
#include "SimVPApp.h"
#include "SimVPFrame.h"
#include "WidgetId.h"
#if defined (__WXMSW__)
#include "Rockey4_ND_32.h"
#elif defined (__WXGTK__)
#include "rockey.h"
#endif

////////////////////////////////////////////////////////////////////////////
enum
{
    SIMVP_STB_INFO,
    SIMVP_STB_DOWNLOAD,
    SIMVP_STB_ADAPTER,

    SIMVP_STB_MAX
};

BEGIN_EVENT_TABLE(SimVPStatusBar, wxStatusBar)
    EVT_SIZE(SimVPStatusBar::OnSize)
END_EVENT_TABLE()

SimVPStatusBar::SimVPStatusBar(wxWindow *parent, long style)
    : wxStatusBar(parent, wxID_ANY, style)
{
    Init();
    Create(parent, style);
}

SimVPStatusBar::~SimVPStatusBar()
{
}

bool SimVPStatusBar::Create(wxWindow *parent, long style)
{
    bool result;
    int widths[SIMVP_STB_MAX] = { -1, 250, 180 };
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;

    result = wxStatusBar::Create(parent, wxID_ANY, style);
    SetFieldsCount(WXSIZEOF(widths));
    SetStatusWidths(WXSIZEOF(widths), widths);
    SetStatusText(_("Welcome to Video Processor Simulator."), SIMVP_STB_INFO);
    SetStatusText(_("Idle"), SIMVP_STB_DOWNLOAD);

    /* adapters information - show on a wxStaticText object */
    _adaptersInfo = new wxStaticText(this, wxID_ANY,
        wxString::Format(_("Adapters: %d"), adapters.size()));
    _adaptersInfo->Bind(wxEVT_LEFT_UP, &SimVPStatusBar::OnAdaptersLUp, this);
    _adaptersInfo->Bind(wxEVT_CONTEXT_MENU, &SimVPStatusBar::OnAdaptersContextMenu, this);

    return result;
}

void SimVPStatusBar::Init()
{
}

void SimVPStatusBar::OnSize(wxSizeEvent &event)
{
    wxRect rect;

    if (_adaptersInfo && GetFieldRect(SIMVP_STB_ADAPTER, rect))
    {
        wxRect rectCheck = rect;
        rectCheck.Deflate(2);
        _adaptersInfo->SetSize(rectCheck);
    }

    event.Skip();
}

void SimVPStatusBar::OnAdaptersLUp(wxMouseEvent &WXUNUSED(event))
{
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;
    wxString info, line;
    size_t i = 0;
    int maxLenInPixel = 0;

    info << _("Local Adapters Info:");
    for (wxVector<NetAdapter>::iterator it = adapters.begin();
        it != adapters.end();
        it++, i++)
    {
        // adapter id and name
        line.clear();
        line << wxT("\n ") << _("Adapter") << wxT("[") << i << wxT("] (")
            << it->GetName() << wxT(")");
        maxLenInPixel = wxMax(maxLenInPixel, GetTextExtent(line).GetWidth());
        info << line;

        // addresses
        line.clear();
        line << wxT("\n  ") << _("ip")  << wxT(": ") << it->GetIp() << wxT(", ")
            << _("netmask") << wxT(": ") << it->GetNetmask() << wxT(", ")
            << _("broadcast") << wxT(": ") << it->GetBroadcast();
        maxLenInPixel = wxMax(maxLenInPixel, GetTextExtent(line).GetWidth());
        info << line;

        // statistics
        line.clear();
        line << wxT("\n  ") << wxT("UDP ") << _("Rx (Bytes/Counts)") << wxT(": ")
            << it->m_udpStatus[NetAdapter::RX_BYTE] << wxT("/")
            << it->m_udpStatus[NetAdapter::RX_COUNT] << wxT(", ")
            << _("Tx (Bytes/Counts)") << wxT(": ")
            << it->m_udpStatus[NetAdapter::TX_BYTE] << wxT("/")
            << it->m_udpStatus[NetAdapter::TX_COUNT];
        maxLenInPixel = wxMax(maxLenInPixel, GetTextExtent(line).GetWidth());
        info << line;
    }

    wxTipWindow *tip = new wxTipWindow(this, info, maxLenInPixel);
    tip->SetTipWindowPtr(&tip);
    tip->Show();
}

void SimVPStatusBar::OnAdaptersContextMenu(wxContextMenuEvent &WXUNUSED(event))
{
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;
    // Only 1st level pop up menu on stack, and others on heap
    wxMenu adapterListMenu;
    int id = myID_CXTMENU_ADAPTER_FIRST;

    for (wxVector<NetAdapter>::iterator it = adapters.begin();
        it != adapters.end();
        it++, id += 10)
    {
        wxMenu *adapter = new wxMenu;
        wxMenuItem *item = NULL;
        /* item: udp */
        item = adapter->AppendCheckItem(id, _("UDP Socket Enabled"),
            _("Enable or disable the UDP socket transmision and receive."));
        if (it->udp && item)
        {
            it->udp->m_tempContextMenuItemId = id;
            adapter->Check(id, it->udp->m_enabled);
        }
        adapterListMenu.Bind(wxEVT_COMMAND_MENU_SELECTED,
            &SimVPStatusBar::OnToggleUDPSocket, this, id);
        /* item: tcp */
        item = adapter->AppendCheckItem(id + 1, _("TCP Socket Enabled"),
            _("Enable or disable the TCP socket transmission and receive."));
        adapter->Check(id + 1, true);

        adapterListMenu.AppendSubMenu(adapter, it->GetIp(), it->GetName());
    }

    PopupMenu(&adapterListMenu);
}

void SimVPStatusBar::OnToggleUDPSocket(wxCommandEvent &event)
{
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;

    wxLogMessage(event.IsChecked() ? wxT("Checked event") : wxT("Unchecked event"));

    for (wxVector<NetAdapter>::iterator it = adapters.begin();
        it != adapters.end();
        it++)
    {
        if (it->udp)
        {
            if (it->udp->m_tempContextMenuItemId == event.GetId())
            {
                wxLogMessage(it->GetIp());
                it->udp->Notify(event.IsChecked());
                it->udp->m_enabled = event.IsChecked();
                break;
            }
        }
    }

    event.Skip();
}

////////////////////////////////////////////////////////////////////////////
IMPLEMENT_APP(SimVPApp)

BEGIN_EVENT_TABLE(SimVPApp, wxApp)
    EVT_IDLE(SimVPApp::OnAppIdle)
END_EVENT_TABLE()

SimVPApp::SimVPApp()
{
    Init();
}

SimVPApp::~SimVPApp()
{
    wxDELETE(m_PeerData);
    wxDELETE(m_HistoryData);

    if (_propertyDB)
        _propertyDB->Close();
    if (_configDB)
        _configDB->Close();
    if (_trapDB)
        _trapDB->Close();
    if (_historyDB)
        _historyDB->Close();
}

void SimVPApp::Init()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString dbPath, dbName, setting;
    wxSQLite3ResultSet set;

    /* database initialization */
    dbPath = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
    dbName = dbPath + wxT("property.db");
    if (NULL != (_propertyDB = new wxSQLite3Database))
        _propertyDB->Open(dbName);
    if (_propertyDB->IsOpen())
        InitDatabase(DB_PROPERTY);
    dbName = dbPath + wxT("config.db");
    if (NULL != (_configDB = new wxSQLite3Database))
        _configDB->Open(dbName);
    if (_configDB->IsOpen())
        InitDatabase(DB_CONFIG);
    dbName = dbPath + wxT("trap.db");
    if (NULL != (_trapDB = new wxSQLite3Database))
        _trapDB->Open(dbName);
    if (_trapDB->IsOpen())
        InitDatabase(DB_TRAP);
    if (NULL != (_historyDB = new wxSQLite3Database))
        _historyDB->Open(wxT(":memory:"));

    /* initialization based on database */
    // language
    set = _configDB->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Language'"));
    if (set.NextRow())
    {
        setting = set.GetAsString(0);
        if (setting == wxT("English"))
            _lang = wxLANGUAGE_ENGLISH;
        else if (setting == wxT("TraditionalChinese"))
            _lang = wxLANGUAGE_CHINESE_TRADITIONAL;
        else if (setting == wxT("SimplifiedChinese"))
            _lang = wxLANGUAGE_CHINESE_SIMPLIFIED;
        else
            _lang = wxLANGUAGE_DEFAULT;
    }
    else
        _lang = wxLANGUAGE_DEFAULT;
    set.Finalize();

    // using rockey4nd
    set = _configDB->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'UsingRockey4ND'"));
    if (set.NextRow())
    {
        setting = set.GetAsString(0);
        if ((setting == wxT("True")) || (setting == wxT("Yes")))
            _usingRockey = true;
        else if ((setting == wxT("False")) || (setting == wxT("No")))
            _usingRockey = false;
        else
            _usingRockey = false;
    }
    else
        _usingRockey = false;
    set.Finalize();
    // skip vmware network adapter
    set = _configDB->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'SkipVMWareNetAdapter'"));
    if (set.NextRow())
    {
        setting = set.GetAsString(0);
        if ((setting == wxT("True")) || (setting == wxT("Yes")))
            _skipVmwareNetworkAdapter = true;
        else if ((setting == wxT("False")) || (setting == wxT("No")))
            _skipVmwareNetworkAdapter = false;
        else
            _skipVmwareNetworkAdapter = true;
    }
    else
        _skipVmwareNetworkAdapter = true;
    set.Finalize();

    /* other data members */
    _locale = NULL;
    _onlyMe = NULL;
    m_Adapters.clear();
    _adapterInfo = NULL;
    _udpProtocol = NULL;
    _tcpProtocol = NULL;
    m_PeerData = new PeerDataModel();
    m_HistoryData = new HistoryDataModel(_historyDB);
}

bool SimVPApp::OnInit()
{
    /* init locale */
    wxStandardPaths &stdPaths = wxStandardPaths::Get();

    _locale = new wxLocale();
    if (_locale && _locale->Init(_lang))
    {
        wxString localePath = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
        localePath += wxT("locale");
        _locale->AddCatalogLookupPathPrefix(localePath);
        _locale->AddCatalog(GetAppName());
    }
    else
    {
        wxLogError(wxT("Requested language is not supported by current operation system"));
        // TODO: reset to default
        return false;
    }

    /* check single instance only */
    _onlyMe = new wxSingleInstanceChecker();
    if (_onlyMe->IsAnotherRunning())
    {
        wxLogWarning(_("SimVP is already running, this instance will be terminated!"));
        return false;
    }

    /* check for USB dongle */
    if (!CheckRockey())
    {
        wxLogError(_("Please insert USB dongle and restart application!"));
        return false;
    }

    /* get network adapter info */
    if (DetectNetAdapter())
    {
        if (m_Adapters.empty())
        {
            wxLogError(_("No network adapter is availabled, this application will be terminated!"));
            return false;
        }
    }
    else
    {
        wxLogError(_("Failed to detect network adapters information, this application will be terminated!"));
        return false;
    }

    /* generate protocol objects.
       Each protocol object will init its related sockets per network adapter. */
    _udpProtocol = new UDPProtocol();
    _tcpProtocol = new TCPProtocol();

    /* init the status bar
     * GTK doesn't allow this to be done in SimVPApp::Init() */
    m_StatusBar = new SimVPStatusBar();

    /* init the main frame */
    SimVPFrame *frame = new SimVPFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int SimVPApp::OnExit()
{
    if (_adapterInfo)
    {
        free(_adapterInfo);
        _adapterInfo = NULL;
    }
    wxDELETE(_udpProtocol);
    wxDELETE(_tcpProtocol);

    return wxApp::OnExit();
}

void SimVPApp::OnAppIdle(wxIdleEvent &WXUNUSED(event))
{
    if (!CheckRockey())
    {
        wxMessageBox(_("USB dongle is removed, application will be terminated now!"), _("Error"));
        ExitMainLoop();
    }
}

bool SimVPApp::CheckRockey()
{
#if defined (__WXGTK__)
#define WORD unsigned short
#define DWORD unsigned int
#define BYTE unsigned char
#define ROCKEY_API_CALL rockey
#elif defined (__WXMSW__)
#define ROCKEY_API_CALL Rockey
#endif
    if (_usingRockey)
    {
        WORD handle[16], p1, p2, p3, p4, retcode;
        DWORD lp1, lp2;
        BYTE buffer[1024];

        p1 = 0xC44C;
        p2 = 0xC8F8;
        p3 = 0x0799;
        p4 = 0xC43B;

        retcode = ROCKEY_API_CALL(RY_FIND, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
        if (retcode)
            return false;
    }

    return true;
#if defined (__WXGTK__)
#undef WORD
#undef DWORD
#undef BYTE
#endif

#undef ROCKEY_API_CALL
}

//^^
// Description:
//      This function use platform-specific method to detect network adapter
//      information.
// Output:
//      changed
//          true - if detected adapter info is changed in the two
//                 successive call.
//          false - if detected adapter info is the same in the two
//                  successive call.
// Return:
//      true - successful detect network adapter info.
//      false - failed to detect network adapter info.
//
bool SimVPApp::DetectNetAdapter(bool *changed)
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
    wxString name, ip, netmask, broadcast;
    for (IP_ADAPTER_INFO *pAdapter = _adapterInfo;
        pAdapter != NULL;
        pAdapter = pAdapter->Next)
    {
        if (pAdapter->Type == MIB_IF_TYPE_ETHERNET)
        {
            name = wxString(pAdapter->Description, *wxConvCurrent);
            /* skip by adapter description */
            if (_skipVmwareNetworkAdapter 
                && name.Matches(wxT("VMware Virtual Ethernet Adapter*")))
                continue;
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
                    broadcast = CalculateSubnetBroadcastAddress(ip, netmask);
                    NetAdapter *temp = new NetAdapter(name, ip, netmask, broadcast);
                    m_Adapters.push_back(*temp);
                }
            }
        }
    }

    /* TODO: judge if result change. */
    if (changed)
        *changed = false;
    return true;
#elif defined (__WXGTK__)
    wxString name, ip, netmask, broadcast;
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

        /* broadcast */
        if (ioctl(socketFd, SIOCGIFBRDADDR, ifr) != 0)
        {
            wxLogError(_("Fail to get broadcast address!"));
            return false;
        }
        memset(ipBuf, 0, INET_ADDRSTRLEN);
        addr = &(ifr->ifr_broadaddr);
        inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr),
            ipBuf, INET_ADDRSTRLEN);
        broadcast = wxString(ipBuf, *wxConvCurrent);

        if (ip.Cmp(wxEmptyString) && ip.Cmp(wxT("0.0.0.0"))
            && netmask.Cmp(wxEmptyString) && netmask.Cmp(wxT("0.0.0.0")))
        {
            NetAdapter *temp = new NetAdapter(name, ip, netmask, broadcast);
            m_Adapters.push_back(*temp);
        }
    }

    /* TODO: judge if result change. */
    if (changed)
        *changed = false;
    return true;
#else
    return false;
#endif
}

wxString SimVPApp::CalculateSubnetBroadcastAddress(wxString ipAddr, wxString netmaskAddr)
{
    if (ipAddr.IsEmpty() || netmaskAddr.IsEmpty()
        || !ipAddr.Cmp(wxT("0.0.0.0")) || !netmaskAddr.Cmp(wxT("0.0.0.0")))
        return wxEmptyString;
    unsigned char ip[4], netmask[4], inetmask[4], broadcast[4], loop;
    unsigned long slice_num;
    wxStringTokenizer ipString(ipAddr, wxT("."));
    wxString token;
    for (loop = 0; ipString.HasMoreTokens() && (loop < 4); loop++)
    {
        token = ipString.GetNextToken();
        if (token.ToULong(&slice_num))
            ip[loop] = (unsigned char)(slice_num & 0xFF);
    }
    wxStringTokenizer netmaskString(netmaskAddr, wxT("."));
    for (loop = 0; netmaskString.HasMoreTokens() && (loop < 4); loop++)
    {
        token = netmaskString.GetNextToken();
        if (token.ToULong(&slice_num))
        {
            netmask[loop] = (unsigned char)(slice_num & 0xFF);
            inetmask[loop] = ~netmask[loop] & 0xFF;
        }
    }
    for (loop = 0; loop < 4; loop++)
        broadcast[loop] = ip[loop] | inetmask[loop];

    return wxString::Format(wxT("%u.%u.%u.%u"),
        broadcast[0], broadcast[1], broadcast[2], broadcast[3]);
}

//
// This function init all database tables need in the whole application
//
void SimVPApp::InitDatabase(eDB db)
{
    wxString sql;

    switch (db)
    {
    case DB_PROPERTY:
        sql << wxT("BEGIN TRANSACTION;")
            << wxT("CREATE TABLE IF NOT EXISTS PropTbl (DisplayedName TEXT UNIQUE, ")
                << wxT("ProtocolName TEXT UNIQUE, PropertyType TEXT, ")
                << wxT("PropertyFormat TEXT, InitValue TEXT, CurrentValue TEXT);")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"BoardName\", \"BOARD_NAME\", \"String\", \"19\", \"SimVP\", \"SimVP\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board type @ Slot#1\", \"SLOT1_BOARD_TYPE\", \"List\", \"NOT_INSTALLED;TYPE_A;TYPE_B;TYPE_C;TYPE_D;TYPE_E;TYPE_F;TYPE_G;TYPE_H;TYPE_I;TYPE_J;TYPE_K;TYPE_L;TYPE_M;TYPE_N;TYPE_O\", \"TYPE_A\", \"TYPE_A\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board type @ Slot#2\", \"SLOT2_BOARD_TYPE\", \"List\", \"NOT_INSTALLED;TYPE_A;TYPE_B;TYPE_C;TYPE_D;TYPE_E;TYPE_F;TYPE_G;TYPE_H;TYPE_I;TYPE_J;TYPE_K;TYPE_L;TYPE_M;TYPE_N;TYPE_O\", \"NOT_INSTALLED\", \"NOT_INSTALLED\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board type @ Slot#3\", \"SLOT3_BOARD_TYPE\", \"List\", \"NOT_INSTALLED;TYPE_A;TYPE_B;TYPE_C;TYPE_D;TYPE_E;TYPE_F;TYPE_G;TYPE_H;TYPE_I;TYPE_J;TYPE_K;TYPE_L;TYPE_M;TYPE_N;TYPE_O\", \"NOT_INSTALLED\", \"NOT_INSTALLED\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board type @ Slot#4\", \"SLOT4_BOARD_TYPE\", \"List\", \"NOT_INSTALLED;TYPE_A;TYPE_B;TYPE_C;TYPE_D;TYPE_E;TYPE_F;TYPE_G;TYPE_H;TYPE_I;TYPE_J;TYPE_K;TYPE_L;TYPE_M;TYPE_N;TYPE_O\", \"NOT_INSTALLED\", \"NOT_INSTALLED\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board type @ Slot#5\", \"SLOT5_BOARD_TYPE\", \"List\", \"NOT_INSTALLED;TYPE_A;TYPE_B;TYPE_C;TYPE_D;TYPE_E;TYPE_F;TYPE_G;TYPE_H;TYPE_I;TYPE_J;TYPE_K;TYPE_L;TYPE_M;TYPE_N;TYPE_O\", \"NOT_INSTALLED\", \"NOT_INSTALLED\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input for board @ Slot#1\", \"SLOT1_INPUT\", \"List\", \"NOTME;DVI;VGA;YPbPr;VIDEO\", \"NOTME\", \"NOTME\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input for board @ Slot#2\", \"SLOT2_INPUT\", \"List\", \"NOTME;SDI1;SDI2\", \"NOTME\", \"NOTME\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input for board @ Slot#3\", \"SLOT3_INPUT\", \"List\", \"NOTME\", \"NOTME\", \"NOTME\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input for board @ Slot#4\", \"SLOT4_INPUT\", \"List\", \"NOTME\", \"NOTME\", \"NOTME\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input for board @ Slot#5\", \"SLOT5_INPUT\", \"List\", \"NOTME\", \"NOTME\", \"NOTME\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Main source selection\", \"MAIN_SLOT\", \"List\", \"NONE;SLOT1;SLOT2;SLOT3;SLOT4;SLOT5\", \"SLOT1\", \"SLOT1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Pip source selection\", \"PIP_SLOT\", \"List\", \"NONE;SLOT1;SLOT2;SLOT3;SLOT4;SLOT5\", \"NONE\", \"NONE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-Offset (Main)\", \"H_OFFSET_MAIN\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-Offset (Pip)\", \"H_OFFSET_PIP\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-Offset (Main)\", \"V_OFFSET_MAIN\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-Offset (Pip)\", \"V_OFFSET_PIP\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-ScaleSize (Main)\", \"H_SCALE_SIZE_MAIN\", \"Numeric\", \"0;1024\", \"320\", \"320\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-ScaleSize (Pip)\", \"H_SCALE_SIZE_PIP\", \"Numeric\", \"0;1024\", \"320\", \"320\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-ScaleSize (Main)\", \"V_SCALE_SIZE_MAIN\", \"Numeric\", \"0;768\", \"240\", \"240\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-ScaleSize (Pip)\", \"V_SCALE_SIZE_PIP\", \"Numeric\", \"0;768\", \"240\", \"240\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-SrcOffset (Main)\", \"H_SRC_OFFSET_MAIN\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-SrcOffset (Pip)\", \"H_SRC_OFFSET_PIP\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-SrcOffset (Main)\", \"V_SRC_OFFSET_MAIN\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-SrcOffset (Pip)\", \"V_SRC_OFFSET_PIP\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Capture Width (Main)\", \"CAPTURE_WIDTH_MAIN\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Capture Width (Pip)\", \"CAPTURE_WIDTH_PIP\", \"Numeric\", \"0;1024\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Capture Height (Main)\", \"CAPTURE_HEIGHT_MAIN\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Capture Height (Pip)\", \"CAPTURE_HEIGHT_PIP\", \"Numeric\", \"0;768\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"DHCP Client\", \"DHCP_CLIENT\", \"List\", \"DISABLE;ENABLE\", \"ENABLE\", \"ENABLE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Subnet mask\", \"SUBNET_MASK_ADDR\", \"String\", \"19\", \"255.255.255.0\", \"255.255.255.0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Static IP Address\", \"STATIC_IP_ADDR\", \"String\", \"19\", \"192.168.0.1\", \"192.168.0.1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Brightness (Main)\", \"BRIGHTNESS_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Brightness (Pip)\", \"BRIGHTNESS_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Contrast (Main)\", \"CONTRAST_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Contrast (Pip)\", \"CONTRAST_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Color Temperature\", \"COLOR_TEMPERATURE\", \"List\", \"5500K;6500K;9300K\", \"6500K\", \"6500K\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Gamma\", \"GAMMA\", \"List\", \"1.0;1.8;2.0;2.2;2.4\", \"2.2\", \"2.2\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Saturation (Main)\", \"ADV_SATURATION_MAIN\", \"Numeric\", \"0;100\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Saturation (Pip)\", \"ADV_SATURATION_PIP\", \"Numeric\", \"0;100\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Hue (Main)\", \"ADV_HUE_MAIN\", \"Numeric\", \"0;360\", \"180\", \"180\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Hue (Pip)\", \"ADV_HUE_PIP\", \"Numeric\", \"0;360\", \"180\", \"180\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Black Level (Main)\", \"ADV_BLACK_MAIN\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Black Level (Pip)\", \"ADV_BLACK_PIP\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Sharpness\", \"ADV_SHARPNESS\", \"Numeric\", \"0;100\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Detail Enhancement\", \"ADV_DETAIL\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Clock (Main)\", \"ADC_CLOCK_MAIN\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Clock (Pip)\", \"ADC_CLOCK_PIP\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Phase (Main)\", \"ADC_PHASE_MAIN\", \"Numeric\", \"-16;15\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Phase (Pip)\", \"ADC_PHASE_PIP\", \"Numeric\", \"-16;15\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Red Input Gain (Main)\", \"ADC_R_GAIN_MAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Green Input Gain (Main)\", \"ADC_G_GAIN_MAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Blue Input Gain (Main)\", \"ADC_B_GAIN_MAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Red Input Offset (Main)\", \"ADC_R_OFFSET_MAIN\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Green Input Offset (Main)\", \"ADC_G_OFFSET_MAIN\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Blue Input Offset (Main)\", \"ADC_B_OFFSET_MAIN\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Red Input Gain (Pip)\", \"ADC_R_GAIN_PIP\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Green Input Gain (Pip)\", \"ADC_G_GAIN_PIP\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Blue Input Gain (Pip)\", \"ADC_B_GAIN_PIP\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Red Input Offset (Pip)\", \"ADC_R_OFFSET_PIP\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Green Input Offset (Pip)\", \"ADC_G_OFFSET_PIP\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Blue Input Offset (Pip)\", \"ADC_B_OFFSET_PIP\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Brightness (Main)\", \"VIDEO_BRIGHTNESS_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Brightness (Pip)\", \"VIDEO_BRIGHTNESS_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Contrast (Main)\", \"VIDEO_CONTRAST_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Contrast (Pip)\", \"VIDEO_CONTRAST_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Sharpness (Main)\", \"VIDEO_SHARPNESS_MAIN\", \"Numeric\", \"0;6\", \"3\", \"3\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Sharpness (Pip)\", \"VIDEO_SHARPNESS_PIP\", \"Numeric\", \"0;6\", \"3\", \"3\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Color (Main)\", \"VIDEO_COLOR_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Color (Pip)\", \"VIDEO_COLOR_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Tint (Main)\", \"VIDEO_TINT_MAIN\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Tint (Pip)\", \"VIDEO_TINT_PIP\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Standard (Main)\", \"VIDEO_STANDARD_MAIN\", \"List\", \"AUTO;NTSC_M;NTSC_44;PAL_BG_DK;PAL_M;PAL_N;SECAM\", \"AUTO\", \"AUTO\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Standard (Pip)\", \"VIDEO_STANDARD_PIP\", \"List\", \"AUTO;NTSC_M;NTSC_44;PAL_BG_DK;PAL_M;PAL_N;SECAM\", \"AUTO\", \"AUTO\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Black Level (Main)\", \"VIDEO_BLACK_MAIN\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Black Level (Pip)\", \"VIDEO_BLACK_PIP\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Overscan (Main)\", \"VIDEO_OVERSCAN_MAIN\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Overscan (Pip)\", \"VIDEO_OVERSCAN_PIP\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video AGC (Main)\", \"VIDEO_AGC_MAIN\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video AGC (Pip)\", \"VIDEO_AGC_PIP\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Aspect Ratio\", \"ASPECT_RATIO\", \"List\", \"STANDARD;FULLSCREEN\", \"FULLSCREEN\", \"FULLSCREEN\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-Display Position\", \"H_DISPLAY_POSITION\", \"Numeric\", \"-15;15\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-Display Position\", \"V_DISPLAY_POSITION\", \"Numeric\", \"-5;5\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-Capture (Main)\", \"H_CAPTURE_START_MAIN\", \"Numeric\", \"-200;200\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-Capture (Main)\", \"V_CAPTURE_START_MAIN\", \"Numeric\", \"-50;50\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H-Capture (Pip)\", \"H_CAPTURE_START_PIP\", \"Numeric\", \"-200;200\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V-Capture (PIp)\", \"V_CAPTURE_START_PIP\", \"Numeric\", \"-50;50\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"PIP Mode\", \"PIP_MODE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"OSD Language\", \"OSD_LANGUAGE\", \"List\", \"ENGLISH;CHINESE\", \"ENGLISH\", \"ENGLISH\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Image Orientation\", \"IMAGE_ORIENTATION\", \"List\", \"FRONT;REAR;INVERTED_FRONT;INVERTED_REAR\", \"FRONT\", \"FRONT\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Frame Lock\", \"FRAME_LOCK\", \"List\", \"OFF;ON\", \"ON\", \"ON\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Force Timing\", \"FORCE_TIMING\", \"List\", \"DISABLE;640x350;720x400\", \"DISABLE\", \"DISABLE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"DVI EQ Setting\", \"DVI_EQ\", \"List\", \"NORMAL;LONG_CABLE\", \"NORMAL\", \"NORMAL\");")
            << wxT("COMMIT TRANSACTION;");
        _propertyDB->ExecuteUpdate(sql);
        break;

    case DB_CONFIG:
        sql << wxT("BEGIN TRANSACTION;")
            << wxT("CREATE TABLE IF NOT EXISTS CfgTbl (ConfigName TEXT UNIQUE, ConfigValue TEXT);")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"Language\", \"Default\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"AutoSaveHistory\", \"True\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"UsingRockey4ND\", \"False\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"LampHoursIncreaseInterval\", \"0\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"MaxConnection\", \"0\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"SkipVMWareNetAdapter\", \"True\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameX\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameY\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameW\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameH\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"Perspective\", \"\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"DefaultPerspective\", \"\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"DebugVerbose\", \"0\");")
            << wxT("COMMIT TRANSACTION;");
        _configDB->ExecuteUpdate(sql);
        break;

    case DB_TRAP:
        sql << wxT("BEGIN TRANSACTION;")
            << wxT("CREATE TABLE IF NOT EXISTS TrapTbl (ProtocolName TEXT UNIQUE, CurrentValue TEXT);")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LEDSTATUS\", \"8738\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_A_STATUS\", \"IN_USE\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_A_HOURS\", \"2345\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_A_LIT_COUNT\", \"50\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_A_TEMP_COND\", \"OK\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_B_STATUS\", \"IN_USE\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_B_HOURS\", \"3456\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_B_LIT_COUNT\", \"50\");")
            << wxT("INSERT OR IGNORE INTO TrapTbl VALUES (\"LAMP_B_TEMP_COND\", \"OK\");")
            << wxT("COMMIT TRANSACTION;");
        _trapDB->ExecuteUpdate(sql);
        break;

    default:
        break;
    }
}

