#include <wx/wx.h>
#include "SimCubeApp.h"
#include "UDPSocket.h"

UDPSocket::UDPSocket(const wxSockAddress &addr, wxSocketFlags flags)
    : wxDatagramSocket(addr, flags)
{
    _history = wxGetApp().m_HistoryData;
}

UDPSocket& UDPSocket::SendToAndRecord(const wxIPV4address &addr,
                                      const char *buf, wxUint32 nBytes)
{
    SendTo(addr, buf, nBytes);
    if (!LastError() && _history)
    {
        HistoryData data;
        data.m_ip = addr.IPAddress();
        data.m_host = addr.Hostname();
        data.m_port = addr.Service();
        data.m_len = LastCount();
        data.m_msg = wxString::FromAscii(buf, nBytes);
        data.m_direction = _("SendTo");
        _history->AddData(data);
    }

    return (*this);
}

