/*
 *  RockeyThread.cpp - A thread to communicate with Rockey4 USE dongle.
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
#include "RockeyThread.h"

#define wxLOG_COMPONENT "PWUpdater/rockey"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
RockeyThread::RockeyThread(wxEvtHandler *handler, const int id)
    : _pHandler(handler),
    _threadEventId(id),
    wxThread(wxTHREAD_DETACHED)
{
}

RockeyThread::~RockeyThread()
{
}

wxThread::ExitCode RockeyThread::Entry()
{
    //wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);

    while (!TestDestroy())
    {
        wxMilliSleep(1000);
    }

    return (wxThread::ExitCode)0;
}

