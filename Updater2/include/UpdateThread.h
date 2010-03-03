#ifndef _UPDATE_THREAD_H_
#define _UPDATE_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>

class UpdateThread : public wxThread
{
public:
    UpdateThread(wxEvtHandler *handler, const wxString &remote, const int row);
    ~UpdateThread();
    virtual wxThread::ExitCode Entry();

private:
    wxEvtHandler *_pHandler;
    wxString _targetIpAddress;
    int _row;
};

typedef enum
{
    UPDATE_THREAD_COMPLETED,
    UPDATE_THREAD_PROGRESS,
} UTMType;

class UpdateThreadMessage
{
public:
    UpdateThreadMessage()
    {
        type = UPDATE_THREAD_COMPLETED;
        progress = 0;
    }
    UTMType type;
    wxString targetIpAddress;
    int progress;
    int row;
    int error;
};


#endif /* _UPDATE_THREAD_H_ */
