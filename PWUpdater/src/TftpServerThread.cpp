/*
 *  TftpServerThread.cpp - A simple implementation for tftp server.
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
#include "TftpServerThread.h"

#define wxLOG_COMPONENT "PWUpdater/tftpserver"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
TftpServerThread::TftpServerThread(wxEvtHandler *handler,
                                   const wxString &root)
    : wxThread(wxTHREAD_DETACHED), _pHandler(handler)
{
    wxIPV4address local;

    local.AnyAddress();
    local.Service(69);
    _udpServerSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
}

TftpServerThread::~TftpServerThread()
{
    wxDELETE(_udpServerSocket);
}

wxThread::ExitCode TftpServerThread::Entry()
{
    return (wxThread::ExitCode)0;
}
