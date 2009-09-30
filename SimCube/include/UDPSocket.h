#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#include <wx/wx.h>
#include <wx/socket.h>
#include "HistoryPane.h"
#include "NetAdapter.h"

class UDPSocket : public wxDatagramSocket
{
public:
    UDPSocket(const wxSockAddress &addr,
        wxSocketFlags flags = wxSOCKET_NONE);
    UDPSocket& SendToWithRecord(const wxIPV4address &addr,
        const char *buf, wxUint32 nBytes);
    UDPSocket& SendToWithoutRecord(const wxIPV4address &addr,
        const char *buf, wxUint32 nBytes);

private:
    bool DoTxStatistics(wxUint32 nBytes);

    HistoryDataModel *_history;
};

#endif /* _UDP_SOCKET_H_ */

