/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _ROCKEY_THREAD_H_
#define _ROCKEY_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>

//
// Definition of event
//
enum
{
    ROCKEY_EVENT_KEY_INSERTED,
    ROCKEY_EVENT_KEY_REMOVED,
    ROCKEY_EVENT_INVALID
};

//
// Communication object between main thread and Rockey thread
//
class RockeyMessage
{
public:
    // default ctor is required by wx internal storage, wxAny...
    RockeyMessage() { _event = ROCKEY_EVENT_INVALID; }
    RockeyMessage(int event) { _event = event; }
    int GetEvent() { return _event; }
    void SetEvent(int event) { _event = event; };
private:
    int _event;
};

//
// Rockey Thread
//
class RockeyThread : public wxThread
{
public:
    RockeyThread(wxEvtHandler *handler, const int id);
    ~RockeyThread();

private:
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    int _threadEventId;
    int _state;
};

#endif /* _ROCKEY_THREAD_H_ */
