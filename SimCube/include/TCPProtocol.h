#ifndef _TCP_PROTOCOL_H_
#define _TCP_PROTOCOL_H_

#include <wx/wx.h>
#include <wx/socket.h>

class TCPProtocol : public wxEvtHandler
{
public:
    TCPProtocol();
    ~TCPProtocol();

private:
    /* event handlers */
    void OnSocketEvent(wxSocketEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif /* _TCP_PROTOCOL_H_ */

