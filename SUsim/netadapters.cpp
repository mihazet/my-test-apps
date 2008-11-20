#include <wx/wx.h>
#include <wx/tokenzr.h>
#include "netadapters.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(ArrayOfNetAdapter);

NetAdapters::NetAdapters()
{
    m_pAdaptersInfo = NULL;
    if (DetectAdapters())
        InitAdapters();
}

NetAdapters::~NetAdapters()
{
    if (m_pAdaptersInfo)
        free(m_pAdaptersInfo);
        
    size_t numAdapters = adapters.GetCount();
    if (numAdapters > 0) {
        for (size_t id = 0; id < numAdapters; id++)
            adapters.RemoveAt(0);
    }
}

bool NetAdapters::DetectAdapters()
{
    bool result = false;
#ifdef __WXMSW__
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    
    if (m_pAdaptersInfo) {
        wxLogWarning(_("Adapters detection has already done, skip!..."));
        return result;
    }
    
    m_pAdaptersInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
    if (m_pAdaptersInfo == NULL) {
        wxLogError(_("Fail to allocate memory needed by GetAdaptersInfo()."));
        return result;
    }
    
    /* Make an initial call to GetAdaptersInfo to get the necessary size into
       the ulOutBufLen variable */
    if (GetAdaptersInfo(m_pAdaptersInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(m_pAdaptersInfo);
        m_pAdaptersInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (m_pAdaptersInfo == NULL) {
            wxLogError(_("Fail to allocate memory needed by GetAdaptersInfo()."));
            return result;
        }
    }
    
    if ((dwRetVal = GetAdaptersInfo(m_pAdaptersInfo, &ulOutBufLen)) != NO_ERROR) {
        wxLogError(_("GetAdaptersInfo failed with error: %d"), dwRetVal);
        return result;
    }
    
    result = true;
#endif
    return result;
}

void NetAdapters::InitAdapters()
{
    IP_ADAPTER_INFO *pAdapter = m_pAdaptersInfo;
    IP_ADDR_STRING *pIpAddrString = NULL;
    wxString ip, netmask, broadcast;
    
    while (pAdapter) {
        if (pAdapter->Type == MIB_IF_TYPE_ETHERNET) {
            for (pIpAddrString = &pAdapter->IpAddressList;
                 pIpAddrString; pIpAddrString = pIpAddrString->Next) {
                ip.Printf(wxT("%s"), pIpAddrString->IpAddress.String);
                netmask.Printf(wxT("%s"), pIpAddrString->IpMask.String);
                if (ip.Cmp(wxEmptyString) && ip.Cmp(wxT("0.0.0.0"))
                    && netmask.Cmp(wxEmptyString) && netmask.Cmp(wxT("0.0.0.0"))) {
                    broadcast = CalculateBroadcastAddress(ip, netmask);
                    adapters.Add(new NetAdapter(ip, netmask, broadcast));
                }
            }
        }
        pAdapter = pAdapter->Next;
    }
}

wxString NetAdapters::CalculateBroadcastAddress(const wxString& ip, const wxString& netmask)
{
    unsigned char _ip[4], _netmask[4], _inetmask[4], _broadcast[4], loop;
    unsigned long slice_num;
    wxStringTokenizer ipString(ip, wxT("."));
    wxStringTokenizer netmaskString(netmask, wxT("."));
    wxString token;

    if (ip.IsEmpty() || !ip.Cmp(wxT("0.0.0.0")) 
        || netmask.IsEmpty() || !netmask.Cmp(wxT("0.0.0.0")))
        return wxEmptyString;
        
    for (loop = 0; ipString.HasMoreTokens() && (loop < 4); loop++) {
        token = ipString.GetNextToken();
        if (token.ToULong(&slice_num)) {
            _ip[loop] = (unsigned char)(slice_num & 0xFF);
        }
    }
    
    for (loop = 0; netmaskString.HasMoreTokens() && (loop < 4); loop++) {
        token = netmaskString.GetNextToken();
        if (token.ToULong(&slice_num)) {
            _netmask[loop] = (unsigned char)(slice_num & 0xFF);
            _inetmask[loop] = ~_netmask[loop] & 0xFF;
        }
    }
    
    for (loop = 0; loop < 4; loop++)
        _broadcast[loop] = _ip[loop] | _inetmask[loop];
        
    return wxString::Format(wxT("%u.%u.%u.%u"), 
        _broadcast[0], _broadcast[1], _broadcast[2], _broadcast[3]);
}
