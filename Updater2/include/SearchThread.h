#ifndef _SEARCH_THREAD_H_
#define _SEARCH_THREAD_H_

#include <wx/wx.h>

#define SEARCH_THREAD_CODEDSTRING_DELIMIT_WORD  wxT("|")

class SearchThread : public wxThread
{
public:
    SearchThread(wxEvtHandler *handler, const wxString &codedString);
    ~SearchThread();
    virtual wxThread::ExitCode Entry();

private:
    wxString LookupRemoteMAC(const wxString &ipAddress);
    wxEvtHandler *_pHandler;
    unsigned char *_recvBuf;
    int _broadcastCount;
    wxString _broadcastAddress;
};

typedef enum
{
    SEARCH_THREAD_COMPLETED,
    SEARCH_THREAD_TARGET_FOUND,
} STMType;

class SearchThreadMessage
{
public:
    SearchThreadMessage()
    {
        type = SEARCH_THREAD_COMPLETED;
        ip = name = mac = wxEmptyString;
    }
    STMType type;
    wxString ip;
    wxString name;
    wxString mac;
};

#endif /* _SEARCH_THREAD_H_ */
