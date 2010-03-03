// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "UpdateThread.h"
#include "WidgetId.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
UpdateThread::UpdateThread(wxEvtHandler *handler, const wxString &remote,
                           const int row)
    : wxThread(wxTHREAD_DETACHED)
{
    _pHandler = handler;
    _targetIpAddress = remote;
    _row = row;
}

UpdateThread::~UpdateThread()
{

}

wxThread::ExitCode UpdateThread::Entry()
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_UPDATE_THREAD);
    UpdateThreadMessage msg;
    int loop = 5;
    
    wxLogMessage(wxT("TCP udpate thread connect to %s"), _targetIpAddress);

    wxSleep(5);

    while (loop--)
    {
        msg.type = UPDATE_THREAD_PROGRESS;
        msg.progress = 100 - loop * 20;
        msg.targetIpAddress = _targetIpAddress;
        msg.row = _row;
        event.SetPayload(msg);
        wxQueueEvent(_pHandler, event.Clone());

        wxSleep(2);
    }

    msg.type = UPDATE_THREAD_COMPLETED;
    msg.error = 0;
    event.SetPayload(msg);
    wxQueueEvent(_pHandler, event.Clone());

    return (ExitCode)0;
}
