/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _UART_THREAD_H_
#define _UART_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>

//
// Definition of event
//
enum
{
    UART_EVENT_PORT_DETECTION,
    UART_EVENT_INVALID
};

//
// Communication object between main thread and Uart thread
//
class UartMessage
{
public:
    // default ctor is required by wx internal storage, wxAny...
    UartMessage()
    {
        _event = UART_EVENT_INVALID;
    }
    UartMessage(int event)
    {
        _event = event;
    }
    int GetEvent() { return _event; }
    void SetEvent(int event) { _event = event; }

private:
    int _event;
};

//
// Uart Thread
//
class UartThread : public wxThread
{
public:
    UartThread(wxEvtHandler *handler, const int id);
    ~UartThread();

private:
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    int _threadEventId;
};

#endif /* _UART_THREAD_H_ */

