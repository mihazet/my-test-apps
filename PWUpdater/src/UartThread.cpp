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

    pQueue->SetConsumer(); // I am consumer of this queue

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
    int evt = message.GetEvent();

    switch (evt)
    {
    case UART_EVENT_CONNECT:
        break;
    case UART_EVENT_DISCONNECT:
        break;
    case UART_EVENT_QUIT:
        result = true;
        break;
    default:
        wxLogError(wxT("Undefined uart event: %d"), evt);
        break;
    }

    return result;
}

