/*
 *  TftpTransmissionThread.cpp - Transmission for the tftp request.
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
                                               const wxString &root)
    : wxThread(wxTHREAD_DETACHED), _pHandler(handler)
{
    wxIPV4address local;

    local.AnyAddress();
    _udpTransmissionSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
}

TftpTransmissionThread::~TftpTransmissionThread()
{
    wxDELETE(_udpTransmissionSocket);
}

wxThread::ExitCode TftpTransmissionThread::Entry()
{
    return (wxThread::ExitCode)0;
}
