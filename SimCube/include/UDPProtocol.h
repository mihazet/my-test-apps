#ifndef _UDP_PROTOCOL_H_
#define _UDP_PROTOCOL_H_

#include <wx/wx.h>

class UDPProtocol : public wxEvtHandler
{
public:
    UDPProtocol();
    ~UDPProtocol();

private:
    void OnSocketEvent(wxSocketEvent &event);

    DECLARE_EVENT_TABLE()
};

#endif /* _UDP_PROTOCOL_H_ */

