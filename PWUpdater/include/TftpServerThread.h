#ifndef _TFTP_SERVER_THREAD_H_
#define _TFTP_SERVER_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

enum
{
    TFTP_SERVER_MSG_THREAD_COMPLETED,

    TFTP_SERVER_MSG_INVALID
};

class TftpServerMessage
{
public:
    TftpServerMessage(int event, const wxString &payload)
    {
        _event = event;
        _payload = payload;
    }
private:
    int _event;
    wxString _payload;
};

class TftpServerThread : public wxThread
{
public:
    TftpServerThread(wxEvtHandler *handler, const wxString &root = wxEmptyString);
    ~TftpServerThread();

private:
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    wxDatagramSocket *_udpServerSocket;
};


#endif /* _TFTP_SERVER_THREAD_H_ */
