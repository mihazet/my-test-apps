#ifndef _NET_ADAPTER_H_
#define _NET_ADAPTER_H_

#include <wx/wx.h>
#include <wx/socket.h>
#include "UDPSocket.h"

class NetAdapter
{
public:
    NetAdapter(wxString name, wxString ip, wxString netmask,
        wxString broadcast)
    {
        _name = name;
        _ip = ip;
        _netmask = netmask;
        _broadcast = broadcast;
        udp = NULL;
        tcp = NULL;
        for (int idx = 0; idx < RXTX_STATUS_MAX; idx++)
        {
            m_udpStatus[idx] = 0;
        }
    }
    wxString& GetName(void) { return _name; }
    wxString& GetIp(void) { return _ip; }
    wxString& GetNetmask(void) { return _netmask; }
    wxString& GetBroadcast(void) { return _broadcast; }
    UDPSocket *udp;
    wxSocketServer *tcp;

    /* socket rx/tx statistics for application layer */
    enum
    {
        RX_COUNT,
        RX_BYTE,
        TX_COUNT,
        TX_BYTE,
        RXTX_STATUS_MAX
    };
    long m_udpStatus[RXTX_STATUS_MAX];

private:
    wxString _name;
    wxString _ip;
    wxString _netmask;
    wxString _broadcast;
};

#endif /* _NET_ADAPTER_H_ */

