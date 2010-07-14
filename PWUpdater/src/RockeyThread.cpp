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
#include "PWUpdater.h"
#include "RockeyThread.h"

#ifdef __WXMSW__
#include "Rockey4_ND_32.h"
#define ROCKEY  Rockey
#else
#endif

#define wxLOG_COMPONENT "PWUpdater/rockey"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
enum
{
    ROCKEY_STATE_NOT_FOUND,
    ROCKEY_STATE_FOUND,
    ROCKEY_STATE_OPENED,
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
RockeyThread::RockeyThread(wxEvtHandler *handler, const int id)
    : _pHandler(handler),
    _threadEventId(id),
    wxThread(wxTHREAD_DETACHED)
{
    _state = ROCKEY_STATE_NOT_FOUND;
}

RockeyThread::~RockeyThread()
{
    wxCriticalSection &cs = wxGetApp().m_rockeyCS;
    RockeyThread *&pRockey = wxGetApp().m_pRockeyThread;

    cs.Enter();
    pRockey = NULL;
    cs.Leave();
}

wxThread::ExitCode RockeyThread::Entry()
{
    unsigned short u16Handle, u16Result, u16Ignore;
    unsigned short u16BasicPW1, u16BasicPW2, u16AdvPW1, u16AdvPW2;
    unsigned long u32Ignore, u32HwId;
    unsigned char buf[1024];

    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    RockeyMessage msg;

    u16BasicPW1 = 0xAC31;
    u16BasicPW2 = 0x9C9C;

    while (!TestDestroy())
    {
        switch (_state)
        {
        default:
        case ROCKEY_STATE_NOT_FOUND:
            
            u16Result = ROCKEY(RY_FIND, &u16Handle, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
            if (u16Result == ERR_SUCCESS)
            {
                msg.SetEvent(ROCKEY_EVENT_KEY_INSERTED);
                event.SetPayload<RockeyMessage>(msg);
                wxQueueEvent(_pHandler, event.Clone());
                _state = ROCKEY_STATE_FOUND;
            }
            break;

        case ROCKEY_STATE_FOUND:

            u16Result = ROCKEY(RY_FIND, &u16Handle, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
            if (u16Result != ERR_SUCCESS)
            {
                msg.SetEvent(ROCKEY_EVENT_KEY_REMOVED);
                event.SetPayload<RockeyMessage>(msg);
                wxQueueEvent(_pHandler, event.Clone());
                _state = ROCKEY_STATE_NOT_FOUND;
            }
            break;
        }

        wxMilliSleep(1000);
    }

    return (wxThread::ExitCode)0;
}

