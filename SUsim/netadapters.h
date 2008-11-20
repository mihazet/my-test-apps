#ifndef _NET_ADAPTERS_H_
#define _NET_ADAPTERS_H_

#include <wx/wx.h>
#include <wx/dynarray.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif

class NetAdapter
{
public:
    NetAdapter(const wxString& ip, const wxString& netmask, const wxString& broadcast)
        : ipAddress(ip), netmaskAddress(netmask), broadcastAddress(broadcast) {}
    wxString ipAddress;
    wxString netmaskAddress;
    wxString broadcastAddress;
};
WX_DECLARE_OBJARRAY(NetAdapter, ArrayOfNetAdapter);

class NetAdapters
{
public:
    NetAdapters();
    ~NetAdapters();
    size_t GetAdapterCount() { return adapters.GetCount(); }
    NetAdapter& GetAdapter(size_t index) { return adapters.Item(index); }
    wxString& GetAdapterIpAddress(size_t index) { return adapters.Item(index).ipAddress; }
    wxString& GetAdapterNetmaskAddress(size_t index) { return adapters.Item(index).netmaskAddress; }
    wxString& GetAdapterBroadcastAddress(size_t index) { return adapters.Item(index).broadcastAddress; }
private:
    bool DetectAdapters(void);
    void InitAdapters(void);
    IP_ADAPTER_INFO *m_pAdaptersInfo;
    wxString CalculateBroadcastAddress(const wxString& ip, const wxString& netmask);
    ArrayOfNetAdapter adapters;
};

#endif /* _NET_ADAPTERS_H_ */
