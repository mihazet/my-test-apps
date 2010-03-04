// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "UpdateThread.h"
#include "WidgetId.h"
#include "UpdaterApp.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
#define RECVBUFSIZE                     128
#define CONNECTION_TIMEOUT              5


// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
UpdateThread::UpdateThread(wxEvtHandler *handler, const wxString &remote,
                           const int row)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _targetIpAddress(remote),
    _row(row)
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxIPV4address local;

    if (!netAdapter.empty())
    {
        _localIpAddress = netAdapter.at(0).GetIp();
        local.Hostname(_localIpAddress);
        _tcp = new wxSocketClient(wxSOCKET_BLOCK);
    }
    else
    {
        _localIpAddress = wxEmptyString;
        _tcp = NULL;
    }

    _recvBuf = new unsigned char[RECVBUFSIZE];
}

UpdateThread::~UpdateThread()
{
    delete _tcp;
    delete [] _recvBuf;
}

wxThread::ExitCode UpdateThread::Entry()
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_UPDATE_THREAD);
    UpdateThreadMessage msg;
    wxIPV4address remote, local;
    int error_code = 0;
    unsigned int txSize = 0;
    unsigned char *txBuf = NULL;
    int loop = 5;

    unsigned char connectMessage[] =
    {
        0x53, 0x43, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    wxLogMessage(wxT("TCP udpate thread connect to %s"), _targetIpAddress);

    if (_tcp)
    {
        remote.Hostname(_targetIpAddress);
        remote.Service(40000);
        local.Hostname(_localIpAddress);

        wxLogVerbose(wxT("Connect to %s:40000..."), _targetIpAddress);

        if (_tcp->Connect(remote, local, false) || _tcp->WaitOnConnect(CONNECTION_TIMEOUT))
        {
            _tcp->Write(&connectMessage[0], sizeof(connectMessage));
            _tcp->Read(_recvBuf, RECVBUFSIZE);
            if ((_tcp->LastError() == wxSOCKET_NOERROR) 
                && (_tcp->LastCount() == 16)
                && (_recvBuf[0] == 's')
                && (_recvBuf[1] == 'c')
                && (_recvBuf[2] == 0)
                && (_recvBuf[3] == 0))
            {
                if (_recvBuf[7] == 0) // big endian
                    txSize = (_recvBuf[8] << 24) + (_recvBuf[9] << 16) + (_recvBuf[10] << 8) + _recvBuf[11];
                else
                    txSize = (_recvBuf[11] << 24) + (_recvBuf[10] << 16) + (_recvBuf[9] << 8) + _recvBuf[8];

                txBuf = new unsigned char [txSize];
            }

            wxSleep(2);

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
        }
        else
            error_code = UTERROR_CONNECT;
    }
    else
        error_code = UTERROR_SOCKET_INIT;

    msg.type = UPDATE_THREAD_COMPLETED;
    msg.error = error_code;
    event.SetPayload(msg);
    wxQueueEvent(_pHandler, event.Clone());

    //
    // clean up
    //
    delete [] txBuf;

    return (ExitCode)error_code;
}
