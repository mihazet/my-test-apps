#ifndef _TFTP_TRANSMISSION_THREAD_H_
#define _TFTP_TRANSMISSION_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

enum
{
    TFTP_TRANSMISSION_MSG_INVALID
};

class TftpTransmissionMessage
{
public:
    TftpTransmissionMessage(int event, const wxString &payload)
    {
        _event = event;
        _payload = payload;
    }
private:
    int _event;
    wxString _payload;
};

class TftpTransmissionThread : public wxThread
{
public:
    TftpTransmissionThread(wxEvtHandler *handler, const wxString &root = wxEmptyString);
    ~TftpTransmissionThread();

private:
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    wxDatagramSocket *_udpTransmissionSocket;
};


#endif /* _TFTP_SERVER_THREAD_H_ */
