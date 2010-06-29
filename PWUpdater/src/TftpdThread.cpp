/*
 *  TftpdThread.cpp - A simple implementation for tftp server.
 *  Refer to RFC1350 for TFTP protocol detail.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifdef _MSC_VER
 #define _CRT_SECURE_NO_WARNINGS /* disable security warning in string.h */
#endif
// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/thread.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include "TftpdThread.h"
#include "PWUpdater.h"

#define wxLOG_COMPONENT "PWUpdater/tftpd"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
enum
{
    TFTP_OPCODE_RRQ     = 1,
    TFTP_OPCODE_WRQ     = 2,
    TFTP_OPCODE_DATA    = 3,
    TFTP_OPCODE_ACK     = 4,
    TFTP_OPCODE_ERROR   = 5
};

// ------------------------------------------------------------------------
// Implementation (Server Thread)
// ------------------------------------------------------------------------
TftpdServerThread::TftpdServerThread(wxEvtHandler *handler, const int id,
                                     wxIPV4address &local,
                                     const wxString &rootPath)
    : _pHandler(handler),
    _threadEventId(id),
    wxThread(wxTHREAD_DETACHED)
{
    /*
       Setup root directory for the tftpd.
       Make sure the path ends without the trailing path separator.
     */
    if (rootPath.empty())
    {
        // use current working directory as root directory...
        wxStandardPaths &stdPaths = wxStandardPaths::Get();
        wxFileName exec = wxFileName(stdPaths.GetExecutablePath());
        _rootPath = exec.GetPath(wxPATH_GET_VOLUME);
    }
    else
    {
        // rootPath may have the trailing path separator, remove it...
        wxFileName dir = wxFileName::DirName(rootPath);
        _rootPath = dir.GetPath(wxPATH_GET_VOLUME);
    }

    /* use standard tftp port */
    local.Service(69);

    /* init socket */
    _udpServerSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
}

TftpdServerThread::~TftpdServerThread()
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;

    /* the server thread is being destroyed, make sure not to leave
       the dangling pointer around. */
    cs.Enter();
    pServer = NULL;
    cs.Leave();

    wxDELETE(_udpServerSocket);
}

wxThread::ExitCode TftpdServerThread::Entry()
{
    wxIPV4address remote;
    unsigned char serverBuffer[1024];
    wxUint32 count;
    wxSocketError error;
    TftpdMessage *msg = NULL;
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    wxString file;
    int mode;
    bool rootPathExist = wxFileName::DirExists(_rootPath);

    /* check root directory is valid */
    if (!rootPathExist)
    {
        msg = new TftpdMessage(TFTPD_EVENT_ERROR,
            wxT("Root directory doesn't exist!"), TFTPD_ERROR_NOT_DEFINE);
        event.SetPayload<TftpdMessage>(*msg);
        wxQueueEvent(_pHandler, event.Clone());
        wxDELETE(msg);
    }

    while (!TestDestroy() && rootPathExist)
    {
        /*
         * When we read from a non-blocking socket and there is no data 
         * available immediately, wxSOCKET_WOULDBLOCK will be retruned.
         * In MSW, it means WSAGetLastError() = WSAEWOULDBLOCK
         * (resource temporarily unavailable)
         */
        memset(&serverBuffer[0], 0, sizeof(serverBuffer));
        _udpServerSocket->RecvFrom(remote, &serverBuffer[0], 1024);
        if (!_udpServerSocket->Error())
        {
            count = _udpServerSocket->LastCount();
            msg = ProtocolParser(&serverBuffer[0], count);
            if (msg)
            {
                /* create transfer thread for RRQ/WRQ */
                if (msg->GetEvent() == TFTPD_EVENT_READ_REQUEST)
                {
                    file = msg->GetString();
                    mode = msg->GetNum1();
                    DoStartTransmissionThread(remote, file, mode, true);
                }
                else if (msg->GetEvent() == TFTPD_EVENT_WRITE_REQUEST)
                {
                    file = msg->GetString();
                    mode = msg->GetNum1();
                    DoStartTransmissionThread(remote, file, mode, false);
                }

                /* post event to main thread */
                event.SetPayload<TftpdMessage>(*msg);
                wxQueueEvent(_pHandler, event.Clone());
                wxDELETE(msg);
            }
        }
        else
        {
            error = _udpServerSocket->LastError();
            if (error != wxSOCKET_WOULDBLOCK)
                wxLogMessage(wxT("Socket read error = %d"), error);
        }

        //wxLogMessage(wxT("tftp server running..."));
        wxMilliSleep(50);
    }

    //wxLogMessage(wxT("tftp server terminated!"));
    return (wxThread::ExitCode)0;
}

//
// If data in the buf can not be recognized as valid tftp protocol, then
// return NULL. Otherwise, return a pointer to a valid TftpServerMessage
// object. It is the responsibility of caller to delete this object.
//
TftpdMessage *TftpdServerThread::ProtocolParser(unsigned char *buf,
                                                unsigned int len)
{
    int opCode, errorCode, mode = TFTPD_TRANSFER_MODE_INVALID;
    wxString fileName, errorMsg, txMode;
    unsigned char *pStr2 = NULL;

    if (len > 2)
    {
        opCode = (buf[0] << 8) + buf[1];
        switch (opCode)
        {
        case TFTP_OPCODE_RRQ:
        case TFTP_OPCODE_WRQ:
            if (len > 4)
            {
                if (IsTwoNullTerminatedString(&buf[2], len - 2, &pStr2))
                {
                    fileName = wxString::FromAscii(&buf[2]);
                    txMode = wxString::FromAscii(pStr2);
                    if (!txMode.CmpNoCase(wxT("netascii")))
                        mode = TFTPD_TRANSFER_MODE_ASCII;
                    else if (!txMode.CmpNoCase(wxT("octet")))
                        mode = TFTPD_TRANSFER_MODE_BINARY;

                    if (mode != TFTPD_TRANSFER_MODE_INVALID)
                    {
                        return new TftpdMessage(
                            (opCode == TFTP_OPCODE_RRQ)
                            ? TFTPD_EVENT_READ_REQUEST
                            : TFTPD_EVENT_WRITE_REQUEST,
                            fileName, mode);
                    }
                }
            }
            break;
        case TFTP_OPCODE_ERROR:
            if (len > 5)
            {
                errorCode = (buf[2] << 8) + buf[3];
                if (IsSingleNullTerminatedString(&buf[4], len - 4))
                {
                    errorMsg = wxString::FromAscii(&buf[4]);
                    return new TftpdMessage(TFTPD_EVENT_ERROR,
                        errorMsg, errorCode);
                }
            }
            break;
        default:
            break;
        }
    }

    return NULL;
}

bool TftpdServerThread::IsSingleNullTerminatedString(unsigned char *buf,
                                                     unsigned int len)
{
    unsigned int index;

    for (index = 0; index < (len - 1); index++)
    {
        if (!isprint((int)buf[index]))
            return false;
    }

    if (buf[len - 1] != 0)
        return false;

    return true;
}

bool TftpdServerThread::IsTwoNullTerminatedString(unsigned char *buf,
                                                  unsigned int len,
                                                  unsigned char **str2)
{
    unsigned int str1Len, str2Len;

    if (str2 == NULL)
        return false;
    else
        *str2 = NULL;

    if (len <= 2)
        return false;

    str1Len = strlen((const char *)&buf[0]);
    if (str1Len >= (len - 2))
        return false;

    str2Len = strlen((const char *)&buf[str1Len + 1]);
    if ((str1Len + str2Len) != (len - 2))
        return false;
    else
        *str2 = &buf[str1Len + 1];

    return true;
}

void TftpdServerThread::DoStartTransmissionThread(
    const wxIPV4address &remote, const wxString &file,
    const int mode, bool rrq)
{
    wxCriticalSection &cs = wxGetApp().m_transmissionCS;
    wxVector<TftpdTransmissionThread *> &transmissions
        = wxGetApp().m_tftpdTransmissionThreads;
    TftpdTransmissionThread *pTransmission;
    bool done = true;
    wxFileName rp = wxFileName::DirName(_rootPath);
    wxString fullName 
        = rp.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + file;

    cs.Enter();

    pTransmission = new TftpdTransmissionThread(_pHandler,
        _threadEventId, remote, fullName, rrq, mode);

    if (pTransmission->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create tftp transmission thread!"));
        wxDELETE(pTransmission);
        done = false;
    }
    else
    {
        if (pTransmission->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the tftp transmission thread!"));
            wxDELETE(pTransmission);
            done = false;
        }
    }

    if (done)
        transmissions.push_back(pTransmission);

    cs.Leave();
}

// ------------------------------------------------------------------------
// Implementation (Transmission Thread)
// ------------------------------------------------------------------------
TftpdTransmissionThread::TftpdTransmissionThread(wxEvtHandler *handler,
                                                 const int threadEventId,
                                                 const wxIPV4address &remote,
                                                 const wxString &file,
                                                 bool read, int mode)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _threadEventId(threadEventId),
    _remote(remote),
    _file(file),
    _read(read),
    _mode(mode)
{
    wxIPV4address local;

    /* use any interface */
    local.AnyAddress();

    /* init socket */
    _udpTransmissionSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);

    /* internal value */
    _txBlock = 1;
}

TftpdTransmissionThread::~TftpdTransmissionThread()
{
    wxCriticalSection &cs = wxGetApp().m_transmissionCS;
    wxVector<TftpdTransmissionThread *> &transmissions 
        = wxGetApp().m_tftpdTransmissionThreads;
    wxVector<TftpdTransmissionThread *>::iterator it;

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

wxThread::ExitCode TftpdTransmissionThread::Entry()
{
    unsigned char transferBuffer[1024];
    bool exist = wxFileName::FileExists(_file);
    wxString mode;
    bool txOk;

    if (_read) // RRQ
    {
        mode = (_mode == TFTPD_TRANSFER_MODE_BINARY) ? wxT("rb") : wxT("r");

        if (exist)
        {
            wxFFileInputStream data(_file, mode);

            if (data.IsOk())
            {
                wxFileOffset dataTotalLen = data.GetLength();
                int totalBlocks = (dataTotalLen / 512) + 1;
                int lastBlockSize = dataTotalLen % 512;

                for (int blk = 0; blk < totalBlocks; blk++)
                {
                    bool last = (blk == totalBlocks - 1);
                    int size = last ? lastBlockSize : 512;
                    data.Read(&transferBuffer[0], size);
                    txOk = DoSendOneBlockData(&transferBuffer[0], size);
                    if (txOk)
                    {
                        int tftpEvent = last 
                            ? TFTPD_EVENT_READ_TRANSFER_DONE
                            : TFTPD_EVENT_READ_TRANSFER_UPDATE;
                        NotifyMainThread(tftpEvent, _file, _txBlock, totalBlocks);

                        /* advance block count */
                        _txBlock++;
                    }
                }
            }
        }
        else
        {
            /* response error to tftp client */

            NotifyMainThread(TFTPD_EVENT_ERROR, wxT("File not found!"), TFTPD_ERROR_FILE_NOT_FOUND);
        }
    }
    else // WRQ
    {
        mode = (_mode == TFTPD_TRANSFER_MODE_BINARY) ? wxT("wb") : wxT("w");

        if (!exist)
        {
            /* read data from tftp client */

            /* response acknowledgement to tftp client */

            /* notify the main thread */
        }
        else
        {
            /* response error to tftp client */

            NotifyMainThread(TFTPD_EVENT_ERROR, wxT("File already exists!"), TFTPD_ERROR_FILE_ALREADY_EXISTS);
        }
    }

    return (wxThread::ExitCode)0;
}

//
// Send data to tftp client, and wait for ack...
// If ack is not received within 'rexmt' second, then resend...
// If ack is still not received within 'timeout' second, then abort...
// Return true - transmission done
// Return false - transmission timeout
//
bool TftpdTransmissionThread::DoSendOneBlockData(void *data, long len)
{
    unsigned char *txBuf = new unsigned char[len + 4];
    unsigned char rxBuf[4];
    wxSocketError error;
    wxUint32 count;
    short opcode, block;
    bool done = false;

    txBuf[0] = (TFTP_OPCODE_DATA >> 8) & 0xFF;
    txBuf[1] = TFTP_OPCODE_DATA & 0xFF;
    txBuf[2] = (_txBlock >> 8) & 0xFF;
    txBuf[3] = _txBlock & 0xFF;
    if (len > 0)
        memcpy(&txBuf[4], data, len);
    
    _udpTransmissionSocket->SendTo(_remote, txBuf, len + 4);
    if (!_udpTransmissionSocket->Error())
    {
        while (!TestDestroy())
        {
            _udpTransmissionSocket->RecvFrom(_remote, rxBuf, 4);
            if (!_udpTransmissionSocket->Error())
            {
                count = _udpTransmissionSocket->LastCount();
                if (count == 4)
                {
                    opcode = (rxBuf[0] << 8) + rxBuf[1];
                    block = (rxBuf[2] << 8) + rxBuf[3];
                    if ((opcode == TFTP_OPCODE_ACK) && (block == _txBlock))
                    {
                        done = true;
                        break;
                    }
                }
            }
            else
            {
                error = _udpTransmissionSocket->LastError();
                if (error == wxSOCKET_WOULDBLOCK)
                {
                    continue; // not real error, check timeout
                }
                else
                {
                    // real error, do something
                }
            }
        }
    }
    else
    {
        error = _udpTransmissionSocket->LastError();
    }

    delete [] txBuf;
    return done;
}

void TftpdTransmissionThread::NotifyMainThread(int evt, const wxString &str,
                                               const int num1, const int num2)
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    TftpdMessage msg(evt, str, num1, num2);
    event.SetPayload<TftpdMessage>(msg);
    wxQueueEvent(_pHandler, event.Clone());
}

