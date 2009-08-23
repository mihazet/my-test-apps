#include <wx/wx.h>
#include <wx/socket.h>
#include "SimCubeApp.h"
#include "NetAdapter.h"
#include "UDPProtocol.h"

BEGIN_EVENT_TABLE(UDPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, UDPProtocol::OnSocketEvent)
END_EVENT_TABLE()

UDPProtocol::UDPProtocol(bool downloadMode) :
    _downloadMode(downloadMode)
{
    size_t socketId = 0;
    wxIPV4address local;
    /* create socket per app network adapter detect result. */

    // wxVector& operator=(const wxVector& vb)
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++, socketId++)
    {
        local.Hostname(it->GetIp());
        local.Service(40000);
        it->udp = new wxDatagramSocket(local);
        if (it->udp->Ok())
        {
            it->udp->SetEventHandler(*this, socketId);
            it->udp->SetNotify(wxSOCKET_INPUT_FLAG);
            it->udp->Notify(true);
        }
    }
}

UDPProtocol::~UDPProtocol()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++)
    {
        if (it->udp)
        {
            it->udp->Notify(false);
            it->udp->Destroy();
        }
    }
}

void UDPProtocol::SetDownloadMode(bool downloadMode)
{
    _downloadMode = downloadMode;

    /* TODO: flush socket and change internal protocol */
}

void UDPProtocol::OnSocketEvent(wxSocketEvent& event)
{
    wxIPV4address remote;
    char localBuf[1500];
    size_t id = event.GetId(), numByte;
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxDatagramSocket *udpSocket = netAdapter.at(id).udp;

    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        memset(localBuf, 0, sizeof(localBuf));
        udpSocket->RecvFrom(remote, localBuf, sizeof(localBuf));
        if (udpSocket->Error())
        {
            wxLogError(_("NetAdapter[%d]: Fail to receive something from UDP socket. Error = %d"),
                id, udpSocket->LastError());
        }
        else
        {
            numByte = udpSocket->LastCount();
            wxLogVerbose(_("NetAdapter[%d]: Received %d byte(s) from %s:%d."),
                id, numByte, remote.IPAddress(), remote.Service());
        }
    default:
        break;
    }
}

