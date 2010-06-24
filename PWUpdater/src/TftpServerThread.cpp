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
#include <wx/thread.h>
#include "TftpServerThread.h"
#include "PWUpdater.h"

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
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpServerThread *&pServer = wxGetApp().m_pTftpServerThread;

    /* the server thread is being destroyed, make sure not to leave
       the dangling pointer around. */
    cs.Enter();
    pServer = NULL;
    cs.Leave();

    wxDELETE(_udpServerSocket);
}

wxThread::ExitCode TftpServerThread::Entry()
{
    wxIPV4address remote;
    unsigned char serverBuffer[1024];

    while (!TestDestroy())
    {
        _udpServerSocket->RecvFrom(remote, &serverBuffer[0], 1024);
        if (!_udpServerSocket->Error())
        {
            
        }

        wxLogMessage(wxT("tftp server running..."));
        wxSleep(1);
    }

    //wxLogMessage(wxT("tftp server terminated!"));
    return (wxThread::ExitCode)0;
}
