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
    /* main -> uart */
    UART_EVENT_CONNECT,
    UART_EVENT_DISCONNECT,
    UART_EVENT_QUIT,

    /* uart -> main */
    UART_EVENT_PORT_DETECTION,

    /* last */
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
        _payload.clear();
    }
    UartMessage(int event, const wxString &payload = wxEmptyString)
    {
        _event = event;
        _payload = payload;
    }
    int GetEvent() const { return _event; }
    void SetEvent(int event) { _event = event; }
    wxString GetPayload() const { return _payload; }
    void SetPayload(const wxString &payload) { _payload = payload; }

private:
    int _event;
    wxString _payload;
};

//
// Thread safe queue
//
template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
    {
        _pCondition = new wxCondition(_mutexCond);
    }

    ~ThreadSafeQueue()
    {
        wxDELETE(_pCondition);
    }

    void SetConsumer()
    {
        // TODO: check only one consumer
        _mutexCond.Lock();
    }

    void EnQueue(const T &data)
    {
        /* push data into queue */
        _mutexQ.Lock();
        _Q.push(data);
        _mutexQ.Unlock();
        /* signal condition variable */
        _mutexCond.Lock();
        _pCondition->Signal();
        _mutexCond.Unlock();
    }

    T DeQueue()
    {
        /* pop data out of queue */
        _mutexQ.Lock();
        T elem(_Q.front());
        _Q.pop();
        _mutexQ.Unlock();
        return elem;
    }

    bool Empty()
    {
        _mutexQ.Lock();
        bool result = _Q.empty();
        _mutexQ.Unlock();

        return result;
    }

    void Wait()
    {
        _pCondition->Wait();
    }

private:
    std::queue<T> _Q;
    wxMutex _mutexQ;
    wxMutex _mutexCond;
    wxCondition *_pCondition;
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
    bool ProcessMessage(const UartMessage &message);

    wxEvtHandler *_pHandler;
    int _threadEventId;
};

#endif /* _UART_THREAD_H_ */

