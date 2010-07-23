/*
 *  UartThread.cpp - A thread to communicate with U-Boot on target board.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "PWUpdater.h"
#include "UartThread.h"

#define wxLOG_COMPONENT "PWUpdater/uart"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
UartThread::UartThread(wxEvtHandler *handler, const int id)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _threadEventId(id)
{
}

UartThread::~UartThread()
{
    wxCriticalSection &cs = wxGetApp().m_uartCS;
    UartThread *&pUart = wxGetApp().m_pUartThread;

    cs.Enter();
    pUart = NULL;
    cs.Leave();
}

wxThread::ExitCode UartThread::Entry()
{
    ThreadSafeQueue<UartMessage> *&pQueue = wxGetApp().m_pUartQueue;
    UartMessage message;
    bool quit = false;

    /* register as queue consumer */
    pQueue->SetConsumer();

    /* detect serial and notify main thread the result */
    DetectSerialPort(true);

    while (!quit)
    {
        pQueue->Wait();
        message = pQueue->DeQueue();

        quit = ProcessMessage(message);

        while (!quit)
        {
            if (!pQueue->Empty())
            {
                message = pQueue->DeQueue();

                quit = ProcessMessage(message);
            }
            else
                break;
        }
    }

    return (wxThread::ExitCode)0;
}

bool UartThread::ProcessMessage(const UartMessage &message)
{
    bool result = false;
    int evt = message.event;

    switch (evt)
    {
    case UART_EVENT_QUIT:
        result = true;
        break;
    case UART_EVENT_CONNECT:
        break;
    case UART_EVENT_DISCONNECT:
        break;
    case UART_EVENT_PORT_SCAN:
        DetectSerialPort(true);
        break;
    default:
        wxLogError(wxT("Undefined uart event: %d"), evt);
        break;
    }

    return result;
}

//
// Detect free serial port and send the result to main thread
//
int UartThread::DetectSerialPort(bool notify)
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    UartMessage message(UART_EVENT_PORT_DETECTION);
    char port[16];
    wxSerialPort com;
    int id;

#if defined (__WXMSW__)
    for (id = 0; id < 100; id++)
    {
        COMMCONFIG cc;
        DWORD dwSize = sizeof(cc);

        sprintf(&port[0], "COM%d", id);
        if (::GetDefaultCommConfigA(port, &cc, &dwSize))
        {
            if (cc.dwProviderSubType == PST_RS232)
            {
                if (id >= 10)
                    sprintf(&port[0], "\\\\.\\COM%d", id);
                if (com.Open(port) < 0)
                    continue;
                com.Close();

                message.payload.push_back(wxString::Format(wxT("COM%d"), id));
            }
        }
    }
#elif defined (__WXGTK__)
    glob_t globbuf;

    // search standard serial port
    strcpy(&port[0], "/dev/ttyS*");
    if (0 == glob(port, GLOB_ERR, NULL, &globbuf))
    {
        // no error, glob was successful
        for (id = 0; id < globbuf.gl_pathc; id++)
        {
            if (com.Open(globbuf.gl_pathv[id]) < 0)
                continue;
            com.Close();

            message.payload.push_back(wxString::Format(wxT("ttyS%d"), id));
        }
    }
    globfree(&globbuf);

    // search for USB to RS232 converters
    strcpy(&port[0], "/dev/ttyUSB*");
    if (0 == glob(port, GLOB_ERR, NULL, &globbuf))
    {
        for (id = 0; id < globbuf.gl_pathc; id++)
        {
            if (com.Open(globbuf.gl_pathv[id]) < 0)
                continue;
            com.Close();

            message.payload.push_back(wxString::Format(wxT("ttyUSB%d"), id));
        }
    }
    globfree(&globbuf);
#endif

    /* notify the main thread */
    if (notify)
    {
        event.SetPayload<UartMessage>(message);
        wxQueueEvent(_pHandler, event.Clone());
    }

    return message.payload.size();
}
