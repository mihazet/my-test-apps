#ifndef _UDP_PROTOCOL_H_
#define _UDP_PROTOCOL_H_

#include <wx/wx.h>

class UDPProtocol : public wxEvtHandler
{
public:
    UDPProtocol(bool downloadMode = false);
    ~UDPProtocol();
    void SetDownloadMode(bool downloadMode);
    bool IsDownloadMode(void) { return _downloadMode; }

private:
    void OnSocketEvent(wxSocketEvent &event);

    bool _downloadMode;

    DECLARE_EVENT_TABLE()
};

#endif /* _UDP_PROTOCOL_H_ */

