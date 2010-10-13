// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/tipwin.h>
#include <wx/tokenzr.h>
#include "UpdaterApp.h"
#include "UpdaterFrame.h"
#include "WidgetId.h"
#include "DataModel.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
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
// Implementation
// ------------------------------------------------------------------------
IMPLEMENT_APP(UpdaterApp)

BEGIN_EVENT_TABLE(UpdaterApp, wxApp)
    EVT_IDLE(UpdaterApp::OnAppIdle)
END_EVENT_TABLE()

UpdaterApp::UpdaterApp()
{
    Init();
}

UpdaterApp::~UpdaterApp()
{
    if (m_pAppOptions)
        delete m_pAppOptions;

    wxDELETE(m_reportModel);

    wxVector<AppLanguageInfo *>::iterator it;
    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
        wxDELETE(*it);
}

void UpdaterApp::Init()
{
    /* application information */
    SetVendorName(wxT("delta"));
    SetVendorDisplayName(wxT("Delta Electronics, Inc."));
    SetAppName(wxT("Updater"));
    SetAppDisplayName(wxT("Geo semi REALTA Platform Updater"));

    /* backend database */
    m_pAppOptions = new AppOptions();
    m_Adapters.clear();
    if (!m_pAppOptions->GetOption(wxT("SkipVMInterface")).Cmp(wxT("Yes")))
        _skipVmwareNetworkAdapter = true;
    else
        _skipVmwareNetworkAdapter = false;

    /* create supported language map */
    AddSupportedLanguages();

    /* init report folder when it is empty */
    if (m_pAppOptions->GetOption(wxT("ReportFolder")).IsEmpty())
    {
        wxStandardPaths& stdPaths = wxStandardPaths::Get();
        wxString defaultReportFolder = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep() + wxT("report");
        m_pAppOptions->SetOption(wxT("ReportFolder"), defaultReportFolder);
        if (!wxFileName::DirExists(defaultReportFolder))
            wxFileName::Mkdir(defaultReportFolder);
    }

    /* thread management */
    m_UpdateThreadCount = 0;
    m_SearchThreadRunning = false;

    /* report data model */
    m_reportModel = new ReportDataModel();
}

bool UpdaterApp::OnInit()
{
    /* init locale */
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString localePath = wxFileName(stdPaths.GetExecutablePath()).GetPath(true) + wxT("locale");
    if (!_locale.Init(DetectInstalledLanguages()))
    {
        wxLogWarning(wxT("This language is not supported by system."));
        // continue nevertheless
    }
    _locale.AddCatalogLookupPathPrefix(localePath);
    _locale.AddCatalog(GetAppName());
    _locale.AddCatalog(wxT("wxstd"));

    /* add description after locale initialization, so that description can be translated. */
    AddLanguageDescriptions();

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

    /* 
       use default (the first) active interface when 
       (1) database is new created, or
       (2) interface store in database doesn't exist now
     */
    bool useDefaultInterface = false;
    wxString ifName = m_pAppOptions->GetOption(wxT("ActivedInterface"));
    if (ifName.IsEmpty())
        useDefaultInterface = true;
    else
    {
        wxVector<NetAdapter>::iterator it;
        for (it = m_Adapters.begin(); it != m_Adapters.end(); ++it)
        {
            if (ifName == it->GetName())
                break;
        }

        if (it == m_Adapters.end())
            useDefaultInterface = true;
    }
    if (useDefaultInterface)
        m_pAppOptions->SetOption(wxT("ActivedInterface"), m_Adapters.at(0).GetName());

    /* For socket works in secondary thread, we need to do this */
    wxSocketBase::Initialize();

    /* init the main frame */
    UpdaterFrame *frame = new UpdaterFrame(NULL);
    frame->Show();
    return true;
}

int UpdaterApp::OnExit()
{
    return wxApp::OnExit();
}

void UpdaterApp::OnAppIdle(wxIdleEvent &WXUNUSED(event))
{
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
bool UpdaterApp::DetectNetAdapter(bool *changed)
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
    wxString name, ip, netmask, broadcast;
    for (IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
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
                    delete temp;
                }
            }
        }
    }

    /* TODO: judge if result change. */
    free(pAdapterInfo);
    if (changed)
        *changed = false;
    return true;
#elif defined (__WXGTK__)
    wxString name, ip, netmask, broadcast;
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
        free(ifc.ifc_buf);
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
            delete temp;
        }
    }

    /* TODO: judge if result change. */
    if (changed)
        *changed = false;
    free(pAdapterInfo);
    close(socketFd);
    return true;
#else
    return false;
#endif
}

wxString UpdaterApp::CalculateSubnetBroadcastAddress(wxString ipAddr, wxString netmaskAddr)
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

long UpdaterApp::HalfMAC2Long(const wxString &halfMAC)
{
    long longTemp = 0, loop = 0, longValue = 0;
    wxStringTokenizer tokenzr(halfMAC, wxT(":-"));
    wxString token;

    while (tokenzr.HasMoreTokens())
    {
        token = tokenzr.GetNextToken();
        switch (loop++)
        {
        case 0: token.ToLong(&longTemp, 16); longValue += (longTemp << 16); break;
        case 1: token.ToLong(&longTemp, 16); longValue += (longTemp << 8); break;
        case 2: token.ToLong(&longTemp, 16); longValue += longTemp; break;
        }
    }

    return longValue;
}

long long UpdaterApp::FullMAC2LongLong(const wxString &fullMAC)
{
    long long longlongValue = 0, longlongTemp;
    long longTemp = 0, loop = 0;
    wxStringTokenizer tokenzr(fullMAC, wxT(":-"));
    wxString token;

    while (tokenzr.HasMoreTokens())
    {
        token = tokenzr.GetNextToken();
        switch (loop++)
        {
        case 0: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += (longlongTemp << 40); break;
        case 1: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += (longlongTemp << 32); break;
        case 2: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += (longlongTemp << 24); break;
        case 3: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += (longlongTemp << 16); break;
        case 4: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += (longlongTemp << 8); break;
        case 5: token.ToLong(&longTemp, 16); longlongTemp = longTemp; longlongValue += longlongTemp; break;
        }
    }

    return longlongValue;
}

void UpdaterApp::AddSupportedLanguages()
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

void UpdaterApp::AddLanguageDescriptions()
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

void UpdaterApp::AddSupportedLanguage(wxLanguage langId, const wxString &iso639)
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

void UpdaterApp::AddLanguageDescription(wxLanguage langId, const wxString &desc)
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

wxLanguage UpdaterApp::DetectInstalledLanguages()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString localePath = wxFileName(stdPaths.GetExecutablePath()).GetPath(true) + wxT("locale");
    wxString selectedLang = m_pAppOptions->GetOption(wxT("Language"));
    wxLanguage result = wxLANGUAGE_ENGLISH;
    bool selectedIsInstalled = false;
    wxVector<AppLanguageInfo *>::iterator it;

    /* scan dir to check if each supported language is installed or not */
    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((wxFileName::DirExists(localePath + wxFileName::GetPathSeparator() + (*it)->m_iso639))
            || ((*it)->m_iso639 == wxT("en")))
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

wxString UpdaterApp::GetLanguageDescriptionFromISO639Code(const wxString &iso639)
{
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_iso639 == iso639)
            return (*it)->m_description;
    }

    return wxEmptyString;
}

wxString UpdaterApp::GetLanguageISO639CodeFromDescription(const wxString &desc)
{
    wxVector<AppLanguageInfo *>::iterator it;

    for (it = _appLanguages.begin(); it != _appLanguages.end(); ++it)
    {
        if ((*it)->m_description == desc)
            return (*it)->m_iso639;
    }

    return wxEmptyString;
}

wxVector<wxString> UpdaterApp::GetInstalledLanguages()
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
