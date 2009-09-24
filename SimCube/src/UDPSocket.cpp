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
        _history->AddData(addr.IPAddress(), addr.Service(),
            wxString::FromAscii(buf, nBytes), LastCount());
    }

    return (*this);
}

