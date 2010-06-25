/*
 *  TftpTransmissionThread.cpp - Transmission for the tftp request.
 *  Refer to RFC1350 for TFTP protocol detail.
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
#include <wx/socket.h>
#include "PWUpdater.h"
#include "TftpTransmissionThread.h"

#define wxLOG_COMPONENT "PWUpdater/tftptransmission"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
TftpTransmissionThread::TftpTransmissionThread(wxEvtHandler *handler,
                                               const wxIPV4address &remote,
                                               const wxString &file,
                                               bool read, int mode)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _remote(remote),
    _file(file),
    _read(read),
    _mode(mode)
{
    wxIPV4address local;

    local.AnyAddress();
    _udpTransmissionSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
}

TftpTransmissionThread::~TftpTransmissionThread()
{
    wxCriticalSection &cs = wxGetApp().m_transmissionCS;
    wxVector<TftpTransmissionThread *> &transmissions 
        = wxGetApp().m_tftpTransmissionThreads;
    wxVector<TftpTransmissionThread *>::iterator it;

    /* The transmission thread is being destroyed, make sure no to leave
       the dangling pointer around. */
    cs.Enter();
    for (it = transmissions.begin(); it != transmissions.end(); ++it)
    {

        if (*it == this)
        {
            *it = NULL;
            break;
        }
    }
    cs.Leave();

    wxDELETE(_udpTransmissionSocket);
}

wxThread::ExitCode TftpTransmissionThread::Entry()
{
    wxLogMessage(wxT("Transmission thread start!"));

    while (!TestDestroy())
    {
        wxLogMessage(wxT("Transmission thread running!"));
        wxMilliSleep(1000);
    }

    return (wxThread::ExitCode)0;
}
