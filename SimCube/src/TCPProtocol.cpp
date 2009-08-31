#include <wx/wx.h>
#include "SimCubeApp.h"
#include "NetAdapter.h"
#include "TCPProtocol.h"

BEGIN_EVENT_TABLE(TCPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, TCPProtocol::OnSocketEvent)
END_EVENT_TABLE()

TCPProtocol::TCPProtocol()
{
    size_t socketId = 0;
    wxIPV4address local;

    /* create socket per app network adapter detect result. */
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++, socketId++)
    {
        local.Hostname(it->GetIp());
        local.Service(40000);
        it->tcp = new wxSocketServer(local, wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
        it->tcp->SetEventHandler(*this, socketId);
        it->tcp->SetNotify(wxSOCKET_CONNECTION_FLAG);
        it->tcp->Notify(true);
        if (!it->tcp->Ok())
            wxLogError(_("Fail to bind %s:%d to tcp socket"), local.IPAddress(), local.Service());
    }
}

TCPProtocol::~TCPProtocol()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++)
    {
        if (it->tcp)
        {
            it->tcp->Notify(false);
            it->tcp->Destroy();
        }
    }
}

void TCPProtocol::OnSocketEvent(wxSocketEvent &event)
{
    wxIPV4address remote;
    size_t id = event.GetId();
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxSocketServer *tcpSocket = netAdapter.at(id).tcp;

    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        wxLogError(wxT("Unexpect wxSOCKET_INPUT received!"));
        break;
    case wxSOCKET_OUTPUT:
        wxLogError(wxT("Unexpect wxSOCKET_OUTPUT received!"));
        break;
    case wxSOCKET_LOST:
        wxLogError(wxT("Unexpect wxSOCKET_LOST received!"));
        break;
    case wxSOCKET_CONNECTION:
        wxSocketBase *sock = tcpSocket->Accept();
        if (sock->GetPeer(remote))
            wxLogMessage(wxT("Got connection from %s:%d"), remote.IPAddress(), remote.Service());
        else
            wxLogError(wxT("Can not get peer info"));
        break;
    }
}

