// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif
#include "SearchThread.h"
#include "WidgetId.h"
#include "UpdaterApp.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
#define RECVBUFSIZE 128
SearchThread::SearchThread(wxEvtHandler *handler) 
    : wxThread(wxTHREAD_DETACHED), _pHandler(handler)
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxIPV4address local;

    if (!netAdapter.empty())
    {
        local.Hostname(netAdapter.at(0).GetIp());
        netAdapter.at(0).udp = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
    }

    _recvBuf = new unsigned char[RECVBUFSIZE];
}

SearchThread::~SearchThread()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    if (!netAdapter.empty())
    {
        wxDELETE(netAdapter.at(0).udp);
    }

    wxDELETE(_recvBuf);
}

wxThread::ExitCode SearchThread::Entry()
{
    int loop;
    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_SEARCH_THREAD);
    SearchThreadMessage msg;
    const unsigned char targetQueryCommand[] =
    {
        0x53, 0x51, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const unsigned char targetReplyCommand[] =
    {
        0x73, 0x71, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        /* above is string len */
    };
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxIPV4address broadcast, remote;
    broadcast.Hostname(netAdapter.at(0).GetBroadcast());
    broadcast.Service(40000);
    wxDatagramSocket *udpSocket = netAdapter.at(0).udp;

    wxLogMessage(wxT("Target search thread is running!"));
    
    for (loop = 0; (loop < 4) && !TestDestroy(); loop++)
    {
        wxLogVerbose(wxT("Broadcast target discovery command!"));
        udpSocket->SendTo(broadcast, targetQueryCommand, 16);

        wxDateTime start = wxDateTime::Now(), current;
        while (!TestDestroy())
        {
            udpSocket->RecvFrom(remote, _recvBuf, RECVBUFSIZE);
            if (!udpSocket->Error())
            {
                wxUint32 count = udpSocket->LastCount();
                if (count > 0)
                {
                    wxLogVerbose(wxT("Receive %d byte(s) data from %s"),
                        count, remote.IPAddress());
                    if ((count > 16) && (count <= 36))
                    {
                        if (!memcmp(&_recvBuf[0], &targetReplyCommand[0], 11)
                            && !memcmp(&_recvBuf[12], &targetReplyCommand[12], 4)
                            && ((_recvBuf[11] == 0) || (_recvBuf[11] == (count - 16)))
                            && ((count - 16 - 1) == strlen((const char *)&_recvBuf[16])))
                        {
                            msg.type = SEARCH_THREAD_TARGET_FOUND;
                            msg.ip = remote.IPAddress();
                            msg.name = wxString(&_recvBuf[16]);
                            msg.mac = LookupRemoteMAC(msg.ip);
                            event.SetPayload(msg);
                            wxQueueEvent(_pHandler, event.Clone());
                        }
                    }
                }                
            }
            current = wxDateTime::Now();
            wxTimeSpan diff = current.Subtract(start);
            if (diff.GetSeconds() >= 3)
                break;
        }
    }

    /* thread completed */
    msg.type = SEARCH_THREAD_COMPLETED;
    event.SetPayload(msg);
    wxQueueEvent(_pHandler, event.Clone());

    return (ExitCode)0;
}

wxString SearchThread::LookupRemoteMAC(const wxString &ipAddress)
{
#ifdef __WXMSW__
    IPAddr destIp = 0;
    unsigned char macAddr[6];
    unsigned long phyAddrLen = sizeof(macAddr);
    DWORD dwRetVal;

    memset(macAddr, 0xFF, phyAddrLen);
    destIp = inet_addr(ipAddress.ToAscii());

    dwRetVal = SendARP(destIp, 0, &macAddr[0], &phyAddrLen);
    if (dwRetVal == NO_ERROR)
    {
        return wxString::Format("%02X:%02X:%02X:%02X:%02X:%02X",
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    }
    else
    {
        switch (dwRetVal)
        {
        case ERROR_GEN_FAILURE:
            wxLogError(wxT("SendARP error = ERROR_GEN_FAILURE"));
            break;
        case ERROR_INVALID_PARAMETER:
            wxLogError(wxT("SendARP error = ERROR_INVALID_PARAMETER"));
            break;
        case ERROR_INVALID_USER_BUFFER:
            wxLogError(wxT("SendARP error = ERROR_INVALID_USER_BUFFER"));
            break;
        case ERROR_BAD_NET_NAME:
            wxLogError(wxT("SendARP error = ERROR_GEN_FAILURE"));
            break;
        case ERROR_BUFFER_OVERFLOW:
            wxLogError(wxT("SendARP error = ERROR_BUFFER_OVERFLOW"));
            break;
        case ERROR_NOT_FOUND:
            wxLogError(wxT("SendARP error = ERROR_NOT_FOUND"));
            break;
        default:
            wxLogError(wxT("SendARP error = %lu"), dwRetVal);
        }
        return wxEmptyString;
    }

#elif defined (__WXGTK__)
    return wxEmptyString;
#else
    return wxEmptyString;
#endif
}

