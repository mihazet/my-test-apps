// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/wfstream.h>
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
                           const int row, const wxString &image)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _targetIpAddress(remote),
    _row(row),
    _imageFilePath(image)
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
    //
    // Load header definition  (all value are hex, and use big endian)
    // ---------------------------------------------------------------
    // [Updater -> Target]
    //   54 42 ff 01  00 00 00 00  00 0f ff f0  00 00 00 00
    //            ^^  ^^^^^ ^^^^^  ^^^^^^^^^^^  ^^^^^^^^^^^
    //             |    |     |         |            |
    //    action --+    |     |         |            |
    //    error --------+     |         |            |
    //    sequence number ----+         |            |
    //    length follow this header ----+            |
    //    check sum ---------------------------------+
    // 
    //    action:
    //     01 - download firmware image to flash
    //     05 - download bootloader image to flash
    //     06 - reset reserved area
    //    sequence number:
    //     ff ff (-1) - used in the last one payload
    //     00 00 (0)  - used in other payload
    //    check sum:
    //     sum of every byte in payload (don't count the header itself)
    //
    // [Target -> Updater] change the first byte 54 to 74.
    //   

    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_UPDATE_THREAD);
    UpdateThreadMessage msg;
    wxIPV4address remote, local;
    int error_code = 0;
    unsigned int txSize = 0;
    unsigned char *txBuf = NULL;
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
            /* send connection request and wait for reply to know the preferred resource requirement */
            _tcp->Write(&connectMessage[0], sizeof(connectMessage));
            _tcp->Read(_recvBuf, RECVBUFSIZE);
            if ((_tcp->LastError() == wxSOCKET_NOERROR) 
                && (_tcp->LastCount() == 16)
                && (_recvBuf[0] == 's')
                && (_recvBuf[1] == 'c')
                && (_recvBuf[2] == 0)
                && (_recvBuf[3] == 0))
            {
                /* the preferred transmition size */
                if (_recvBuf[7] == 0) // big endian
                    txSize = (_recvBuf[8] << 24) + (_recvBuf[9] << 16) + (_recvBuf[10] << 8) + _recvBuf[11];
                else
                    txSize = (_recvBuf[11] << 24) + (_recvBuf[10] << 16) + (_recvBuf[9] << 8) + _recvBuf[8];
                txBuf = new unsigned char [txSize];

                /* start to load image file content and transmit it */
                wxFFileInputStream imageStream(_imageFilePath);
                if (imageStream.IsOk())
                {
                    long streamLength = imageStream.GetLength();
                    long transmitCount, q, r;
                    unsigned int checkSum = 0;

                    q = streamLength / (txSize - 16);
                    r = streamLength % (txSize - 16);
                    if (r)
                        transmitCount = q + 1;
                    else
                    {
                        transmitCount = q;
                        r = txSize - 16;
                    }

                    // to here: transmitCount is the number of transmit, and r is last transmition size.

                    while (transmitCount--)
                    {
                        size_t readIndex, leftForRead, lastRead, sumIndex;

                        readIndex = 16;
                        leftForRead = transmitCount ? txSize - 16 : r;

                        /* read required data length from stream */
                        do
                        {
                            imageStream.Read(&txBuf[readIndex], leftForRead);
                            lastRead = imageStream.LastRead();
                            readIndex += lastRead;
                            leftForRead -= lastRead;
                        } while (leftForRead != 0);

                        /* update the calculated checksum */
                        for (sumIndex = 16; sumIndex < readIndex; sumIndex++)
                            checkSum += txBuf[sumIndex];

                        /* prepare the load header */
                        txBuf[0] = 'T';
                        txBuf[1] = 'B';
                        txBuf[2] = 0xff;
                        txBuf[3] = 0x01; // firmware
                        txBuf[4] = 0;
                        txBuf[5] = 0;
                        if (transmitCount == 0)
                        {
                            txBuf[6] = txBuf[7] = 0xff;
                            txBuf[8] = (r >> 24) & 0xFF;
                            txBuf[9] = (r >> 16) & 0xFF;
                            txBuf[10] = (r >> 8) & 0xFF;
                            txBuf[11] = r & 0xFF;
                            txBuf[12] = (checkSum >> 24) & 0xFF;
                            txBuf[13] = (checkSum >> 16) & 0xFF;
                            txBuf[14] = (checkSum >> 8) & 0xFF;
                            txBuf[15] = checkSum & 0xFF;
                        }
                        else
                        {
                            txBuf[6] = txBuf[7] = 0;
                            txBuf[8] = ((txSize - 16) >> 24) & 0xFF;
                            txBuf[9] = ((txSize - 16) >> 16) & 0xFF;
                            txBuf[10] = ((txSize - 16) >> 8) & 0xFF;
                            txBuf[11] = (txSize - 16) & 0xFF;
                            txBuf[12] = txBuf[13] = txBuf[14] = txBuf[15] = 0;
                        }
                        
                        /* transmit date */
                        if (transmitCount == 0)
                            _tcp->Write(&txBuf[0], r + 16);
                        else
                            _tcp->Write(&txBuf[0], txSize);
                        if (_tcp->LastError() != wxSOCKET_NOERROR)
                        {
                            error_code = UTERROR_SOCKET_WRITE;
                            break;
                        }

                        /* wait for response and verify it */
                        _tcp->Read(_recvBuf, RECVBUFSIZE);
                        if ((_tcp->LastError() == wxSOCKET_NOERROR)
                            && (_tcp->LastCount() == 16))
                        {
                            wxLogVerbose(wxT("Response: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X [%08X]"),
                                _recvBuf[0], _recvBuf[1], _recvBuf[2], _recvBuf[3], _recvBuf[4], _recvBuf[5], _recvBuf[6], _recvBuf[7],
                                _recvBuf[8], _recvBuf[9], _recvBuf[10], _recvBuf[11], _recvBuf[12], _recvBuf[13], _recvBuf[14], _recvBuf[15], checkSum);

                            if ((_recvBuf[4] != 0) || (_recvBuf[5] != 0))
                            {
                                error_code = (_recvBuf[4] << 8) + _recvBuf[5];
                                break;
                            }
                        }
                        else
                        {
                            error_code = UTERROR_SOCKET_READ;
                            break;
                        }

                        /* notify progress */
                        msg.type = UPDATE_THREAD_PROGRESS;
                        msg.progress = transmitCount ? 50 : 100;
                        msg.targetIpAddress = _targetIpAddress;
                        msg.row = _row;
                        event.SetPayload(msg);
                        wxQueueEvent(_pHandler, event.Clone());
                    }
                }
                else
                    error_code = UTERROR_FILE_STREAM;
            }
            else
                error_code = UTERROR_CONNECT;
        }
        else
            error_code = UTERROR_SOCKET_CONNECT;
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
