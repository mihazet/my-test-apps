#ifndef _UDP_PROTOCOL_H_
#define _UDP_PROTOCOL_H_

#include <wx/wx.h>
#include <wx/socket.h>

class UDPProtocol;
class NormalHandler
{
    friend class UDPProtocol;
    bool (UDPProtocol::*handler)(const char *buf, size_t len, wxSockAddress &addr);
    wxChar keyword;
};

class UDPProtocol : public wxEvtHandler
{
    friend class NormalHandler;
public:
    UDPProtocol(bool downloadMode = false);
    ~UDPProtocol();
    void SetDownloadMode(bool downloadMode);
    bool IsDownloadMode(void) { return _downloadMode; }

private:
    /* event handlers */
    void OnSocketEvent(wxSocketEvent &event);

    /* internal functions */
    void ProcessDownloadModeProtocol(const char *buf, size_t len,
        wxSockAddress &addr);
    void ProcessNormalModeProtocol(const char *buf, size_t len,
        wxSockAddress &addr);
    NormalHandler *_normalHandler;
    bool set_request_handler(const char *buf, size_t len, wxSockAddress &addr);
    bool get_request_handler(const char *buf, size_t len, wxSockAddress &addr);
    bool null_handler(const char *buf, size_t len, wxSockAddress &addr);
    bool _downloadMode;

    DECLARE_EVENT_TABLE()
};

#endif /* _UDP_PROTOCOL_H_ */

