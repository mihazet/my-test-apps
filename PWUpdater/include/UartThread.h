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
#include <wx/ctb/serport.h>

//
// Definition of event
//
enum
{
    /* main -> uart */
    UART_EVENT_QUIT,
        // no payload
    UART_EVENT_CONNECT,
        // payload.at(0): port connect to
    UART_EVENT_DISCONNECT,
        // no payload
    UART_EVENT_PORT_SCAN,
        // no payload
    UART_EVENT_DOWNLOAD,
        // payload.at(0): ddr temp memory
        // n = 0, 1, 2, ...
        // payload.at(3n + 1): image name
        // payload.at(3n + 2): image offset
        // payload.at(3n + 3): image size

    /* uart -> main */
    UART_EVENT_PORT_DETECTION,
        // payload.at(n): available port n
    UART_EVENT_CONNECTED,
        // payload.at(0): port connected
    UART_EVENT_DISCONNECTED,
        // no payload
    UART_EVENT_DOWNLOAD_PROGRESS,
        // payload.at(0): image name
        // payload.at(1): progress
    UART_EVENT_DOWNLOAD_RESULT,
        // payload.at(0): image name
        // payload.at(1): result
    UART_EVENT_DOWNLOAD_COMPLETE,
        // no payload

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
        event = UART_EVENT_INVALID;
        payload.clear();
    }
    UartMessage(int evt)
    {
        event = evt;
        payload.clear();;
    }

    int event;
    wxVector<wxString> payload;
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
    int DetectSerialPort(bool notify = true);

    wxEvtHandler *_pHandler;
    int _threadEventId;
    wxSerialPort _comPort;
};

#endif /* _UART_THREAD_H_ */

