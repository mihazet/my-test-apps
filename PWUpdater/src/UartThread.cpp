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
    int evt;

    pQueue->SetConsumer(); // I am consumer of this queue

    wxLogMessage(wxT("Uart thread started..."));

    while (1)
    {
        pQueue->Wait();
        //wxLogMessage(wxT("wake up from wait"));
        message = pQueue->DeQueue();
        //wxLogMessage(wxT("Something in queue"));

        /* process message */
        evt = message.GetEvent();
        if (evt == UART_EVENT_QUIT)
        {
            //if (TestDestroy())
                break;
        }
        else
        {
            if (evt == UART_EVENT_CONNECT)
                wxLogMessage(wxT("Connect event"));
            else if (evt == UART_EVENT_DISCONNECT)
                wxLogMessage(wxT("Disconnect event"));
        }

        while (1)
        {
            if (!pQueue->Empty())
            {
                message = pQueue->DeQueue();

                /* process message */
            }
            else
                break;
        }
    }

    return (wxThread::ExitCode)0;
}

