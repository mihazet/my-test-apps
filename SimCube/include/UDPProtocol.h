#ifndef _UDP_PROTOCOL_H_
#define _UDP_PROTOCOL_H_

#include <wx/wx.h>
#include <wx/socket.h>

class UDPProtocol;
class NormalHandler
{
    friend class UDPProtocol;
    bool (UDPProtocol::*handler)(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    wxString keyword;
};

class UDPProtocol : public wxEvtHandler
{
    friend class NormalHandler;
public:
    UDPProtocol();
    ~UDPProtocol();

private:
    /* event handlers */
    void OnSocketEvent(wxSocketEvent &event);

    /* internal functions */
    void ProcessDownloadModeProtocol(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    void ProcessNormalModeProtocol(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    void AllocateSafeBuf(char **safe, size_t *safeLen,
        const char *unsafe, size_t unsafeLen);
    void DeallocateSafeBuf(char *safe);
    bool IsBufferUnsafe(const char *buf, size_t len);
    void MakeBufferSafe(char **safe, size_t *safeLen,
        const char *unsafe, size_t unsafeLen);
    NormalHandler *_normalHandler;
    bool set_request_handler(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    bool get_request_handler(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    bool null_handler(const char *buf, size_t len,
        wxIPV4address &peer, wxDatagramSocket *local);
    bool _needDeallocateSafeBuf;

    DECLARE_EVENT_TABLE()
};

#endif /* _UDP_PROTOCOL_H_ */

