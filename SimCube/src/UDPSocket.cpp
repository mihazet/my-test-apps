#include <wx/wx.h>
#include "SimCubeApp.h"
#include "UDPSocket.h"

UDPSocket::UDPSocket(const wxSockAddress &addr, wxSocketFlags flags)
    : wxDatagramSocket(addr, flags)
{
    _history = wxGetApp().m_HistoryData;
    m_enabled = false;
    m_tempContextMenuItemId = wxID_ANY;
}

UDPSocket& UDPSocket::SendToWithRecord(const wxIPV4address &addr,
                                       const char *buf, wxUint32 nBytes)
{
    /* don't sent if socket is disabled */
    if (!m_enabled)
        return (*this);

    SendTo(addr, buf, nBytes);
    if (!LastError() && _history)
    {
        /* add into history record */
        HistoryData data;
        data.m_ip = addr.IPAddress();
        data.m_host = addr.Hostname();
        data.m_port = addr.Service();
        data.m_len = LastCount();
        data.m_msg = wxString::FromAscii(buf, nBytes);
        data.m_direction = _("SendTo");
        _history->AddData(data);

        /* add into socket statistics */
        DoTxStatistics(nBytes);
    }

    return (*this);
}

UDPSocket& UDPSocket::SendToWithoutRecord(const wxIPV4address &addr,
                                          const char *buf, wxUint32 nBytes)
{
    /* don't sent if socket is disabled */
    if (!m_enabled)
        return (*this);

    SendTo(addr, buf, nBytes);
    if (!LastError())
    {
        /* add into socket statistics */
        DoTxStatistics(nBytes);
    }

    return (*this);
}

bool UDPSocket::DoTxStatistics(wxUint32 nBytes)
{
    int id = 0;
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;

    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++, id++)
    {
        if (it->udp == this)
        {
            it->m_udpStatus[NetAdapter::TX_COUNT]++;
            it->m_udpStatus[NetAdapter::TX_BYTE] += nBytes;
            return true;
        }
    }

    return false;
}

