#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#include <wx/wx.h>
#include <wx/socket.h>
#include "HistoryPane.h"

class UDPSocket : public wxDatagramSocket
{
public:
    UDPSocket(const wxSockAddress &addr,
        wxSocketFlags flags = wxSOCKET_NONE);
    UDPSocket& SendToAndRecord(const wxIPV4address &addr,
        const char *buf, wxUint32 nBytes);

private:
    HistoryDataModel *_history;
};

#endif /* _UDP_SOCKET_H_ */

