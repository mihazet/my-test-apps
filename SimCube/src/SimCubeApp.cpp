#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wxsqlite3.h>
#include <wx/tokenzr.h>
#include "SimCubeApp.h"
#include "SimCubeFrame.h"
#include "Rockey4_ND_32.h"

IMPLEMENT_APP(SimCubeApp)

BEGIN_EVENT_TABLE(SimCubeApp, wxApp)
#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    EVT_IDLE(SimCubeApp::OnAppIdle)
#endif
END_EVENT_TABLE()

SimCubeApp::SimCubeApp()
{
    Init();
}

void SimCubeApp::Init()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString dbName;

    /* database initialization */
    dbName = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
    dbName += wxT("SimCube.db");
    if (NULL != (_mainDB = new wxSQLite3Database))
        _mainDB->Open(dbName);
    if (NULL != (_memDB = new wxSQLite3Database))
        _memDB->Open(wxT(":memory:"));

    /* other data members */
    _locale = NULL;
    _onlyMe = NULL;
    m_Adapters.clear();
    _adapterInfo = NULL;
    _udpProtocol = NULL;
}

bool SimCubeApp::OnInit()
{
    /* init locale */
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    int lang = wxLANGUAGE_DEFAULT;
    if (_mainDB->IsOpen())
    {
        wxSQLite3ResultSet set = _mainDB->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Language'"));
        if (set.NextRow())
        {
            wxString langSetting = set.GetAsString(0);
            if (langSetting == wxT("English"))
                lang = wxLANGUAGE_ENGLISH;
            else if (langSetting == wxT("TraditionalChinese"))
                lang = wxLANGUAGE_CHINESE_TRADITIONAL;
            else if (langSetting == wxT("SimplifiedChinese"))
                lang = wxLANGUAGE_CHINESE_SIMPLIFIED;
        }
        set.Finalize();
        _locale = new wxLocale();
        if (_locale && _locale->Init(lang, wxLOCALE_CONV_ENCODING))
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
    }
    else
    {
        wxLogError(wxT("Database corruption! Re-install the application may help!"));
        return false;
    }

    /* check single instance only */
    _onlyMe = new wxSingleInstanceChecker(GetAppName());
    if (_onlyMe->IsAnotherRunning())
    {
        wxLogWarning(_("SimCube is already running, this instance will be terminated!"));
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

    /* generate protocol object and socket per network adapter */
    _udpProtocol = new UDPProtocol();

#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    /* check for USB dongle */
    if (!CheckRockey())
    {
        wxLogError(_("Please insert USB dongle and restart application!"));
        return false;
    }
#endif

    /* init the main frame */
    SimCubeFrame *frame = new SimCubeFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int SimCubeApp::OnExit()
{
    if (_mainDB)
        _mainDB->Close();
    if (_memDB)
        _memDB->Close();

    if (_adapterInfo)
    {
        free(_adapterInfo);
        _adapterInfo = NULL;
    }
    if (_udpProtocol)
    {
        delete _udpProtocol;
        _udpProtocol = NULL;
    }
    return wxApp::OnExit();
}

#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
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
    WORD handle[16], p1, p2, p3, p4, retcode;
    DWORD lp1, lp2;
    BYTE buffer[1024];

    p1 = 0xC44C;
    p2 = 0xC8F8;
    p3 = 0x0799;
    p4 = 0xC43B;

    retcode = Rockey(RY_FIND, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
    if (retcode)
        return false;

    return true;
}
#endif

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
            name = wxString(pAdapter->AdapterName, *wxConvCurrent);
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

