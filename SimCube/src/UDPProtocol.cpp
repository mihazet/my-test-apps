#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/socket.h>
#include "SimCubeApp.h"
#include "NetAdapter.h"
#include "UDPProtocol.h"

#define MSG_KEYWORD_SET     '='
#define MSG_KEYWORD_GET     '?'
#define MSG_KEYWORD_INVALID ' '

BEGIN_EVENT_TABLE(UDPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, UDPProtocol::OnSocketEvent)
END_EVENT_TABLE()

UDPProtocol::UDPProtocol(bool downloadMode) :
    _downloadMode(downloadMode)
{
    size_t socketId = 0;
    wxIPV4address local;

    /* init function pointer */
    _normalHandler = new NormalHandler[3];
    _normalHandler[0].handler = &UDPProtocol::set_request_handler;
    _normalHandler[0].keyword = MSG_KEYWORD_SET;
    _normalHandler[1].handler = &UDPProtocol::get_request_handler;
    _normalHandler[1].keyword = MSG_KEYWORD_GET;
    _normalHandler[2].handler = &UDPProtocol::null_handler;
    _normalHandler[2].keyword = MSG_KEYWORD_INVALID;

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

    delete [] _normalHandler;
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
            if (_downloadMode)
                ProcessDownloadModeProtocol(&localBuf[0], numByte, remote);
            else
                ProcessNormalModeProtocol(&localBuf[0], numByte, remote);
        }
    default:
        break;
    }
}

void UDPProtocol::ProcessDownloadModeProtocol(const char *buf, size_t len,
                                              wxSockAddress &addr)
{

}

void UDPProtocol::ProcessNormalModeProtocol(const char *buf, size_t len,
                                            wxSockAddress &addr)
{
    bool handled = false;
    int msg_keyword_checker, handler, rIdx, lIdx;
    wxString token;
    wxStringTokenizer tkz(wxString::FromAscii(buf), wxT(";"));
    while (tkz.HasMoreTokens())
    {
        token = tkz.GetNextToken();
        if (token.IsEmpty())
            continue;
        msg_keyword_checker = 0;
        for (handler = 0; 
            _normalHandler[handler].keyword != MSG_KEYWORD_INVALID;
            handler++)
        {
            lIdx = token.Find(_normalHandler[handler].keyword);
            if (lIdx != wxNOT_FOUND)
            {
                /* found keyword for current handler */
                msg_keyword_checker |= 0x01 << (2 * handler);
                rIdx = token.Find(_normalHandler[handler].keyword, true);
                if (lIdx != rIdx)
                {
                    /* found multiple keywords for handler in current token */
                    msg_keyword_checker |= 0x02 << (2 * handler);
                }
            }
        }

        /* skip if no keyword */
        if (msg_keyword_checker == 0)
            continue;

        /* process token if only single keyword */
        if (!(msg_keyword_checker & (msg_keyword_checker - 1)))
        {
            /* find handler type */
            for (handler = 0; msg_keyword_checker != 0x01; msg_keyword_checker >>= 2);
            /* process token by handler type */
            handled = (this->*_normalHandler[handler].handler)(buf, len, addr);

            if (!handled)
                wxLogVerbose(_("Token (%s) didn't handle by handler"), token);
        }
    }
}

bool UDPProtocol::set_request_handler(const char *buf, size_t len,
                                      wxSockAddress &addr)
{
    bool handled = false;
    return handled;
}

bool UDPProtocol::get_request_handler(const char *buf, size_t len,
                                      wxSockAddress &addr)
{
    bool handled = false;
    return handled;
}

bool UDPProtocol::null_handler(const char *buf, size_t len, wxSockAddress &addr)
{
    bool handled = false;
    return handled;
}

