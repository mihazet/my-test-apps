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
    return (wxThread::ExitCode)0;
}

