#ifndef _NET_ADAPTER_H_
#define _NET_ADAPTER_H_

#include <wx/wx.h>
#include <wx/socket.h>

class NetAdapter
{
public:
    NetAdapter(wxString ip, wxString netmask, wxString broadcast,
        unsigned short port = 40000)
    {
        _ip = ip;
        _netmask = netmask;
        _broadcast = broadcast;
        _port = port;
    }
    wxString& GetIp(void) { return _ip; }
    wxString& GetNetmask(void) { return _netmask; }
    wxString& GetBroadcast(void) { return _broadcast; }
    unsigned short GetPort(void) const { return _port; }
private:
    wxString _ip;
    wxString _netmask;
    wxString _broadcast;
    unsigned short _port;
};

#endif /* _NET_ADAPTER_H_ */

