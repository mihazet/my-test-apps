#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/tipwin.h>
#include <wx/wxsqlite3.h>
#include <wx/tokenzr.h>
#include "SimCubeApp.h"
#include "SimCubeFrame.h"
#if defined (__WXMSW__)
#include "Rockey4_ND_32.h"
#elif defined (__WXGTK__)
#include "rockey.h"
#endif

////////////////////////////////////////////////////////////////////////////
enum
{
    SIMCUBE_STB_INFO,
    SIMCUBE_STB_DOWNLOAD,
    SIMCUBE_STB_ADAPTER,

    SIMCUBE_STB_MAX
};

BEGIN_EVENT_TABLE(SimCubeStatusBar, wxStatusBar)
    EVT_SIZE(SimCubeStatusBar::OnSize)
END_EVENT_TABLE()

SimCubeStatusBar::SimCubeStatusBar(wxWindow *parent, long style)
    : wxStatusBar(parent, wxID_ANY, style)
{
    Init();
    Create(parent, style);
}

SimCubeStatusBar::~SimCubeStatusBar()
{
}

bool SimCubeStatusBar::Create(wxWindow *parent, long style)
{
    bool result;
    int widths[SIMCUBE_STB_MAX] = { -1, 250, 180 };
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;

    result = wxStatusBar::Create(parent, wxID_ANY, style);
    SetFieldsCount(WXSIZEOF(widths));
    SetStatusWidths(WXSIZEOF(widths), widths);
    SetStatusText(_("Welcome to Cube Simulator."), SIMCUBE_STB_INFO);
    SetStatusText(_("Idle"), SIMCUBE_STB_DOWNLOAD);

    /* adapters information - show on a wxStaticText object */
    _adaptersInfo = new wxStaticText(this, wxID_ANY,
        wxString::Format(_("Adapters: %d"), adapters.size()));
    _adaptersInfo->Bind(wxEVT_LEFT_UP, &SimCubeStatusBar::OnAdaptersLUp, this);

    return result;
}

void SimCubeStatusBar::Init()
{
}

void SimCubeStatusBar::OnSize(wxSizeEvent &event)
{
    wxRect rect;

    if (_adaptersInfo && GetFieldRect(SIMCUBE_STB_ADAPTER, rect))
    {
        wxRect rectCheck = rect;
        rectCheck.Deflate(2);
        _adaptersInfo->SetSize(rectCheck);
    }

    event.Skip();
}

void SimCubeStatusBar::OnAdaptersLUp(wxMouseEvent &WXUNUSED(event))
{
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;
    wxString info, line;
    size_t i = 0, maxLenInPixel = 300, lineLen;

    info << _("Local Adapters Info:");
#if 1
    for (wxVector<NetAdapter>::iterator it = adapters.begin();
        it != adapters.end();
        it++, i++)
    {
        line.clear();
        //line << _("Adapter") << wxT("[") << i << wxT("] (") << iter->GetName() << wxT(")\n");
        //if (maxLenInPixel < (lineLen = GetTextExtent(line).GetWidth()))
        //    maxLenInPixel = lineLen;
        line = wxT("X");
        info << line;

        line.clear();
        //line << _("ip: ") << iter->GetIp() << _(" netmask: ") << iter->GetNetmask() << wxT("\n");
        //if (maxLenInPixel < (lineLen = GetTextExtent(line).GetWidth()))
        //    maxLenInPixel = lineLen;
        line = wxT("Y");
        info << line;
    }
#endif
    wxTipWindow *tip = new wxTipWindow(this, info, maxLenInPixel);
    tip->SetTipWindowPtr(&tip);
    tip->Show();
}

////////////////////////////////////////////////////////////////////////////
IMPLEMENT_APP(SimCubeApp)

BEGIN_EVENT_TABLE(SimCubeApp, wxApp)
    EVT_IDLE(SimCubeApp::OnAppIdle)
END_EVENT_TABLE()

SimCubeApp::SimCubeApp()
{
    Init();
}

SimCubeApp::~SimCubeApp()
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

void SimCubeApp::Init()
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

bool SimCubeApp::OnInit()
{
    /* init locale */
    wxStandardPaths &stdPaths = wxStandardPaths::Get();

    _locale = new wxLocale();
    if (_locale && _locale->Init(_lang, wxLOCALE_CONV_ENCODING))
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
        wxLogWarning(_("SimCube is already running, this instance will be terminated!"));
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
     * GTK doesn't allow this to be done in SimCubeApp::Init() */
    m_StatusBar = new SimCubeStatusBar();

    /* init the main frame */
    SimCubeFrame *frame = new SimCubeFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int SimCubeApp::OnExit()
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

void SimCubeApp::OnAppIdle(wxIdleEvent &WXUNUSED(event))
{
    if (!CheckRockey())
    {
        wxMessageBox(_("USB dongle is removed, application will be terminated now!"), _("Error"));
        ExitMainLoop();
    }
}

bool SimCubeApp::CheckRockey()
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
bool SimCubeApp::DetectNetAdapter(bool *changed)
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

wxString SimCubeApp::CalculateSubnetBroadcastAddress(wxString ipAddr, wxString netmaskAddr)
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
void SimCubeApp::InitDatabase(eDB db)
{
    wxString sql;

    switch (db)
    {
    case DB_PROPERTY:
        sql << wxT("BEGIN TRANSACTION;")
            << wxT("CREATE TABLE IF NOT EXISTS PropTbl (DisplayedName TEXT UNIQUE, ")
                << wxT("ProtocolName TEXT UNIQUE, PropertyType TEXT, ")
                << wxT("PropertyFormat TEXT, InitValue TEXT, CurrentValue TEXT);")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"BoardName\", \"BOARD_NAME\", \"String\", \"19\", \"SimCube\", \"SimCube\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Board H/W Revison\", \"BOARD_HW_REV\", \"String\", \"11\", \"2973043701\", \"2973043701\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"DHCP Client\", \"DHCP_CLIENT\", \"List\", \"DISABLE;ENABLE\", \"ENABLE\", \"ENABLE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Host Domain\", \"HOST_DOMAIN\", \"String\", \"31\", \"deltaww.com\", \"deltaww.com\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"MAC Address\", \"MAC_ADDR\", \"String\", \"19\", \"00:18:23:4D:41:43\", \"00:18:23:4D:41:43\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Netmask\", \"NETMASK\", \"String\", \"19\", \"255.255.255.0\", \"255.255.255.0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Static IP\", \"STATIC_IP\", \"String\", \"19\", \"192.168.0.1\", \"192.168.0.1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Brightness\", \"BRIGHTNESS\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Contrast\", \"CONTRAST\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Color Temp\", \"DLP_COLOR_TEMPERATURE\", \"List\", \"6500K;9300K;USER1;USER2;USER3\", \"USER1\", \"USER1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"CutOff R\", \"R_CUTOFF\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"CutOff G\", \"G_CUTOFF\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"CutOff B\", \"B_CUTOFF\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Drive R\", \"R_DRIVE\", \"Numeric\", \"0;200\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Drive G\", \"G_DRIVE\", \"Numeric\", \"0;200\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Drive B\", \"B_DRIVE\", \"Numeric\", \"0;200\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Color\", \"ADV_COLOR\", \"Numeric\", \"0;100\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Hue\", \"ADV_HUE\", \"Numeric\", \"0;360\", \"180\", \"180\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Black Level\", \"ADV_BLACK\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Sharpness\", \"ADV_SHARPNESS\", \"Numeric\", \"0;100\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Detail Enhancement\", \"ADV_DETAIL_ENHANCEMENT\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Offset R\", \"R_INPUT_OFFSET\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Offset G\", \"G_INPUT_OFFSET\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Offset B\", \"B_INPUT_OFFSET\", \"Numeric\", \"0;127\", \"63\", \"63\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Gain R\", \"R_INPUT_GAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Gain G\", \"G_INPUT_GAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Input Gain B\", \"B_INPUT_GAIN\", \"Numeric\", \"0;255\", \"127\", \"127\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Brightness\", \"VIDEO_BRIGHTNESS\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Contrast\", \"VIDEO_CONTRAST\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Sharpness\", \"VIDEO_SHARPNESS\", \"Numeric\", \"0;6\", \"3\", \"3\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Color\", \"VIDEO_COLOR\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Tint\", \"VIDEO_TINT\", \"Numeric\", \"0;255\", \"128\", \"128\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Standard\", \"VIDEO_STANDARD\", \"List\", \"AUTO;NTSC_M;NTSC_44;PAL_BG_DK;PAL_M;PAL_N;SECAM\", \"AUTO\", \"AUTO\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Black Level\", \"VIDEO_BLACK\", \"List\", \"0IRE;7.5IRE\", \"0IRE\", \"0IRE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Video Overscan\", \"VIDEO_OVERSCAN\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Aspect Ratio\", \"ASPECT_RATIO\", \"List\", \"4_BY_3;16_BY_9\", \"4_BY_3\", \"4_BY_3\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Function Type\", \"POSITION_TYPE\", \"List\", \"LEGACY;ANYPLACE\", \"ANYPLACE\", \"ANYPLACE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Position H\", \"H_POSITION\", \"Numeric\", \"-15;15\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Position V\", \"V_POSITION\", \"Numeric\", \"-5;5\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Top Left H\", \"TOPLEFT_H_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Top Left V\", \"TOPLEFT_V_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Top Right H\", \"TOPRIGHT_H_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Top Right V\", \"TOPRIGHT_V_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Bottom Left H\", \"BOTTOMLEFT_H_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Bottom Left V\", \"BOTTOMLEFT_V_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Bottom Right H\", \"BOTTOMRIGHT_H_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Bottom Right V\", \"BOTTOMRIGHT_V_POSITION\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Reset Corner Position\", \"RESET_CORNER_POSITION\", \"List\", \"TOPLEFT;TOPRIGHT;BOTTOMLEFT;BOTTOMRIGHT;ALL\", \"ALL\", \"ALL\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Capture H\", \"H_CAPTURE\", \"Numeric\", \"-200;200\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"CApture V\", \"V_CAPTURE\", \"Numeric\", \"-50;50\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Frequency\", \"ADC_FREQUENCY\", \"Numeric\", \"-100;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"ADC Phase\", \"ADC_PHASE\", \"Numeric\", \"-16;15\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Main Input\", \"MAIN_INPUT\", \"List\", \"VGA1;VGA2;BNC1;BNC2;YPBPR1;YPBPR2;VIDEO;DVI1;DVI2;DVI3;DVI4\", \"VGA1\", \"VGA1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"PIP Mode\", \"PIP_MODE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"PIP Input\", \"PIP_INPUT\", \"List\", \"VIDEO\", \"VIDEO\", \"VIDEO\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"PIP H Position\", \"PIP_H_POSITION\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"PIP V Position\", \"PIP_V_POSITION\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Load User Mode\", \"USER_MODE_LOAD\", \"Numeric\", \"1;30\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Save User Mode\", \"USER_MODE_SAVE\", \"Numeric\", \"1;30\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Language\", \"LANGUAGE\", \"List\", \"ENGLISH;SIMPLIFIED_CHINESE\", \"ENGLISH\", \"ENGLISH\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Image Orientation\", \"IMAGE_ORIENTATION\", \"List\", \"FRONT;REAR;INVERTED_FRONT;INVERTED_REAR\", \"FRONT\", \"FRONT\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Baud Rate\", \"BAUD_RATE\", \"List\", \"9600;19200;57600;115200\", \"115200\", \"115200\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Gamma\", \"DLP_GAMMA\", \"List\", \"FILM;GRAPHIC;VIDEO;LINEAR;NORMAL;MAX.BRIGHTNESS\", \"FILM\", \"FILM\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Menu H Position\", \"MENU_H_POSITION\", \"Numeric\", \"0;50\", \"50\", \"50\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Menu V Position\", \"MENU_V_POSITION\", \"Numeric\", \"0;50\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp Switch\", \"LAMP_SELECTION\", \"List\", \"LAMPA;LAMPB;LAMPA+B\", \"LAMPA+B\", \"LAMPA+B\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp A Power\", \"BALLAST_A_POWER\", \"Numeric\", \"0;14\", \"14\", \"14\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp B Power\", \"BALLAST_B_POWER\", \"Numeric\", \"0;14\", \"14\", \"14\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp Hot Swap\", \"LAMP_HOT_SWAP\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp Error Auto Switch\", \"LAMP_ERROR_AUTO_SWITCH\", \"List\", \"OFF;ON\", \"ON\", \"ON\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp Timeout Change\", \"LAMP_TIMEOUT_CHANGE\", \"List\", \"OFF;ON\", \"ON\", \"ON\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp Timeout Change Hours\", \"LAMP_TIMEOUT_CHANGE_HOURS\", \"Numeric\", \"100;1000\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp A Hours Threshold\", \"LAMP_A_HOURS_THRESHOLD\", \"Numeric\", \"100;10000\", \"3500\", \"3500\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp B Hours Threshold\", \"LAMP_B_HOURS_THRESHOLD\", \"Numeric\", \"100;10000\", \"3500\", \"3500\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp A Lit Count Threshold\", \"LAMP_A_LIT_COUNT_THRESHOLD\", \"Numeric\", \"100;5000\", \"1000\", \"1000\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lamp B Lit Count Threshold\", \"LAMP_B_LIT_COUNT_THRESHOLD\", \"Numeric\", \"100;5000\", \"1000\", \"1000\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"All Number Of Column\", \"WALL_COLUMN_DIMENSION\", \"Numeric\", \"1;8\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"All Number Of Row\", \"WALL_ROW_DIMENSION\", \"Numeric\", \"1;8\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Projector Column\", \"PROJECTOR_COLUMN_LOCATION\", \"Numeric\", \"1;8\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Projector Row\", \"PROJECTOR_ROW_LOCATION\", \"Numeric\", \"1;8\", \"1\", \"1\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Projector Id\", \"PROJECTOR_ID\", \"Numeric\", \"0:99\", \"00\", \"00\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Power Delay\", \"POWER_DELAY\", \"Numeric\", \"0;10\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Frame Lock\", \"FRAME_LOCK\", \"List\", \"OFF;ON\", \"ON\", \"ON\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"RGB First Auto\", \"RGB_FIRST_AUTO\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"AGC\", \"VIDEO_AGC\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Test Pattern\", \"SCALAR_TEST_PATTERN\", \"Numeric\", \"0;59\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"CCA\", \"CCA\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"R.Primary Red\", \"RED_PRIMARY_RED\", \"Numeric\", \"0;100\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"R.Primary Green\", \"RED_PRIMARY_GREEN\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"R.Primary Blue\", \"RED_PRIMARY_BLUE\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"G.Primary Red\", \"GREEN_PRIMARY_RED\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"G.Primary Green\", \"GREEN_PRIMARY_GREEN\", \"Numeric\", \"0;100\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"G.Primary Blue\", \"GREEN_PRIMARY_BLUE\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"B.Primary Red\", \"BLUE_PRIMARY_RED\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"B.Primary Green\", \"BLUE_PRIMARY_GREEN\", \"Numeric\", \"0;100\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"B.Primary Blue\", \"BLUE_PRIMARY_BLUE\", \"Numeric\", \"0;100\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Force Timing\", \"FORCE_TIMING\", \"List\", \"DISABLE;640x350;720x400\", \"DISABLE\", \"DISABLE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"White Peaking\", \"WHITE_PEAKING\", \"Numeric\", \"0;100\", \"100\", \"100\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"DVI34 EQ Setting\", \"DVI34_EQ\", \"List\", \"NORMAL;HIGH\", \"NORMAL\", \"NORMAL\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Brilliant Color Mode\", \"BRILLIANT_COLOR\", \"List\", \"DISABLE;ENABLE\", \"ENABLE\", \"ENABLE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Edge Blending Alpha\", \"OVL_EB_ALPHA\", \"Numeric\", \"0;255\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Upper Blending\", \"OVL_EB_TOP_ENABLE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Upper Width\", \"OVL_EB_TOP_WIDTH\", \"Numeric\", \"0;128\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lower Blending\", \"OVL_EB_BOTTOM_ENABLE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Lower Width\", \"OVL_EB_BOTTOM_WIDTH\", \"Numeric\", \"0;128\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Left Blending\", \"OVL_EB_LEFT_ENABLE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Left Width\", \"OVL_EB_LEFT_WIDTH\", \"Numeric\", \"0;128\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Right Blending\", \"OVL_EB_RIGHT_ENABLE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Right Width\", \"OVL_EB_RIGHT_WIDTH\", \"Numeric\", \"0;128\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"EWarp App\", \"EWARP_APP\", \"List\", \"ANYPLACE;KEYSTONE;ROTATION\", \"ANYPLACE\", \"ANYPLACE\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"H Keystone\", \"H_KEYSTONE\", \"Numeric\", \"-40;40\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"V Keystone\", \"V_KEYSTONE\", \"Numeric\", \"-30;30\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Rotation\", \"ROTATION\", \"Numeric\", \"-180;180\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Pincushion\", \"PINCUSHION\", \"Numeric\", \"-20;20\", \"0\", \"0\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Projector Mounting\", \"PROJECTOR_MOUNTING\", \"List\", \"FRONT_TABLETOP;FRONT_CEILING;REAR_TABLETOP;REAR_CEILING\", \"FRONT_TABLETOP\", \"FRONT_TABLETOP\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Freeze\", \"FREEZE\", \"List\", \"OFF;ON\", \"OFF\", \"OFF\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Index Delay\", \"INDEX_DELAY\", \"Numeric\", \"0;359\", \"180\", \"180\");")
            << wxT("INSERT OR IGNORE INTO PropTbl VALUES (\"Key\", \"KEY\", \"List\", \"AUTO;BNC;BRIGHTNESS;CCA;CONTRAST;DIGITAL;DOWN;ENTER;FREEZE;LAMP;LEFT;MENU;OSDOFF;PIP;PIXEL;POSITION;POWER;POWERON;POWEROFF;PROJECTOR;RIGHT;STATUS;TEST.P;UP;VGA;VIDEO;YPBPR;0;1;2;3;4;5;6;7;8;9\", \"MENU\", \"MENU\");")
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
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameX\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameY\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameW\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"FrameH\", \"-1\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"Perspective\", \"\");")
            << wxT("INSERT OR IGNORE INTO CfgTbl VALUES (\"DefaultPerspective\", \"\");")
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

