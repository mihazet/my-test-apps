#ifndef _TCP_PROTOCOL_H_
#define _TCP_PROTOCOL_H_

#include <wx/wx.h>
#include <wx/socket.h>

typedef struct
{
    unsigned char *byp_load_buffer;
    unsigned int u32_load_offset;
    unsigned int u32_total_load;
} tst_BUFFER_LOAD;

#define SOI_MAX_DOWNLOAD_SIZE   (10 * 1024 * 1024)

class TCPProtocol : public wxEvtHandler
{
public:
    TCPProtocol();
    ~TCPProtocol();

private:
    /* event handlers */
    void OnSocketEvent(wxSocketEvent &event);

    /* internal functions */
    bool ProcessDownloadModeProtocol(void *pIn, void *pOut);

    size_t numClients;
    tst_BUFFER_LOAD *pst_buffer_load;

    DECLARE_EVENT_TABLE()
};

#endif /* _TCP_PROTOCOL_H_ */

