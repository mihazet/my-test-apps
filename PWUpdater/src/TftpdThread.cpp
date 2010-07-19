/*
 *  TftpdThread.cpp - A simple implementation for tftp server.
 *  Refer to RFC1350 for TFTP protocol detail.
 *  Refer to RFC2347/RFC2348/RFC2349 for TFTP option extension detail.
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
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/stopwatch.h>
#include <wx/tokenzr.h>
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
    TFTP_OPCODE_ERROR   = 5,
    TFTP_OPCODE_OACK    = 6
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
        _rootPath = wxGetCwd();
    else
        _rootPath = rootPath;

    // remove the trailing path separator if necessary
    wxFileName dir = wxFileName::DirName(_rootPath);
    _rootPath = dir.GetPath(wxPATH_GET_VOLUME);

    /* use standard tftp port */
    local.Service(69);

    /* init socket */
    _udpServerSocket = new wxDatagramSocket(local, wxSOCKET_NOWAIT);

    /* RFC2347/2348/2349 - option negotiation for transmission */
    _optResendTimeout = _optTransferSize = _optBlockSize = false;
    _valResendTimeout = _valTransferSize = _valBlockSize = -1;

    /* record ip address for later use */
    int loop = 0;
    long longTemp = 0;
    _localIpHighNibble = _localIpLowNibble = 0;
    wxStringTokenizer tokenzr(local.IPAddress(), wxT("."));
    while (tokenzr.HasMoreTokens())
    {
        wxString token = tokenzr.GetNextToken();
        switch (loop++)
        {
        case 0:
            token.ToLong(&longTemp);
            _localIpHighNibble = (longTemp & 0xFF) << 8;
            break;
        case 1:
            token.ToLong(&longTemp);
            _localIpHighNibble += (longTemp & 0xFF);
            break;
        case 2:
            token.ToLong(&longTemp);
            _localIpLowNibble = (longTemp & 0xFF) << 8;
            break;
        case 3:
            token.ToLong(&longTemp);
            _localIpLowNibble += (longTemp & 0xFF);
            break;
        }
    }
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

    /* check socket */
    if (!_udpServerSocket->IsOk())
    {
        msg = new TftpdMessage(TFTPD_EVENT_ERROR,
            wxT("Tftp server port is unavailable!"), TFTPD_ERROR_ACCESS_VIOLATION);
        event.SetPayload<TftpdMessage>(*msg);
        wxQueueEvent(_pHandler, event.Clone());
        wxDELETE(msg);
        return (wxThread::ExitCode)0;
    }

    /* check root directory is valid */
    if (!wxFileName::DirExists(_rootPath))
    {
        msg = new TftpdMessage(TFTPD_EVENT_ERROR,
            wxT("Root directory doesn't exist!"), TFTPD_ERROR_NOT_DEFINE);
        event.SetPayload<TftpdMessage>(*msg);
        wxQueueEvent(_pHandler, event.Clone());
        wxDELETE(msg);
        return (wxThread::ExitCode)0;
    }

    /* notify main thread, we are starting... */
    msg = new TftpdMessage(TFTPD_EVENT_SERVER_STARTED, _rootPath,
        _localIpHighNibble, _localIpLowNibble);
    event.SetPayload<TftpdMessage>(*msg);
    wxQueueEvent(_pHandler, event.Clone());
    wxDELETE(msg);

    while (!TestDestroy())
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
// return NULL. Otherwise, return a pointer to a valid TftpdMessage object.
// It is the responsibility of caller to delete this object.
//
TftpdMessage *TftpdServerThread::ProtocolParser(unsigned char *buf,
                                                unsigned int len)
{
    int opCode, errorCode, mode = TFTPD_TRANSFER_MODE_INVALID;
    wxString fileName, errorMsg, txMode, optName, optValue;
    size_t nameLen, modeLen, optLen, valueLen;
    unsigned int pair, optIdx;
    bool optParseError = false;
    long longTemp = 0;

    if (len > 2)
    {
        opCode = (buf[0] << 8) + buf[1];
        switch (opCode)
        {
        case TFTP_OPCODE_RRQ:
        case TFTP_OPCODE_WRQ:
            if (len > 4)
            {
                /* file name */
                nameLen = strlen((const char *)&buf[2]);
                if (nameLen >= (len - 2))
                    break;
                else
                    fileName = wxString::FromAscii(&buf[2], nameLen);

                /* mode */
                modeLen = strlen((const char *)&buf[2 + nameLen + 1]);
                if (modeLen >= (len - 2 - nameLen - 1))
                    break;
                else
                    txMode = wxString::FromAscii(&buf[2 + nameLen + 1], modeLen);
                if (!txMode.CmpNoCase(wxT("netascii")))
                    mode = TFTPD_TRANSFER_MODE_ASCII;
                else if (!txMode.CmpNoCase(wxT("octet")))
                    mode = TFTPD_TRANSFER_MODE_BINARY;

                /* option/value pairs */
                optIdx = 2 + nameLen + 1 + modeLen + 1;
                for (pair = 1; optIdx < len; pair++)
                {
                    /* option */
                    optLen = strlen((const char *)&buf[optIdx]);
                    if (optLen >= (len - optIdx))
                    {
                        optParseError = true;
                        break;
                    }
                    else
                        optName = wxString::FromAscii(&buf[optIdx], optLen);

                    optIdx += (optLen + 1);

                    /* value */
                    valueLen = strlen((const char *)&buf[optIdx]);
                    if (valueLen >= (len - optIdx))
                    {
                        optParseError = true;
                        break;
                    }
                    else
                    {
                        optValue = wxString::FromAscii(&buf[optIdx], valueLen);
                        optValue.ToLong(&longTemp);
                    }

                    optIdx += (valueLen + 1);

                    if (!optName.CmpNoCase(wxT("timeout")))
                    {
                        _optResendTimeout = true;
                        _valResendTimeout = longTemp;
                    }
                    else if (!optName.CmpNoCase(wxT("tsize")))
                    {
                        _optTransferSize = true;
                        _valTransferSize = longTemp;
                    }
                    else if (!optName.CmpNoCase(wxT("blksize")))
                    {
                        _optBlockSize = true;
                        _valBlockSize = longTemp;
                    }
                    else
                    {
                        optParseError = true;
                        break;
                    }
                }

                if (optParseError)
                    break;

                if (mode != TFTPD_TRANSFER_MODE_INVALID)
                {
                    return new TftpdMessage(
                        (opCode == TFTP_OPCODE_RRQ)
                        ? TFTPD_EVENT_READ_REQUEST
                        : TFTPD_EVENT_WRITE_REQUEST,
                        fileName, mode);
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
            return new TftpdMessage(TFTPD_EVENT_ERROR,
                wxT("Illegal operation!"), TFTPD_ERROR_ILLEGAL_OPERATION);
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
        /* init transmission options */
        if (_optBlockSize)
        {
            pTransmission->SetBlockSize(_valBlockSize);
            _optBlockSize = false;
        }
        if (_optResendTimeout)
        {
            pTransmission->SetRetransmitInterval(_valResendTimeout * 1000);
            _optResendTimeout = false;
        }
        if (_optTransferSize)
        {
            pTransmission->SetTransferSize(_valTransferSize);
            _optTransferSize = false;
        }

        /* start transfer thread */
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

    /* internal default value */
    _dataBlock = 1;
    _ackBlock = 0;
    _rexmt = -1;
    _timeout = -1;
    _blkSize = -1;
    _transferSize = -1;
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
    unsigned char *pTransferBuffer;
    unsigned char receiveBuffer[1024];
    bool exist = wxFileName::FileExists(_file);
    wxString mode;
    bool txOk, rxOk, last;
    long rxLen;
    int tftpEvent;

    if (_read) // RRQ
    {
        mode = (_mode == TFTPD_TRANSFER_MODE_BINARY) ? wxT("rb") : wxT("r");

        if (exist)
        {
            wxFFileInputStream data(_file, mode);

            if (data.IsOk())
            {
                wxFileOffset dataTotalLen = data.GetLength();

                DoSendOptAckAndWaitAckIfNeed((long)dataTotalLen);
                pTransferBuffer = new unsigned char [_blkSize];

                int totalBlocks = (dataTotalLen / _blkSize) + 1;
                int lastBlockSize = dataTotalLen % _blkSize;

                for (int blk = 0; blk < totalBlocks; blk++)
                {
                    last = (blk == totalBlocks - 1);
                    int size = last ? lastBlockSize : _blkSize;
                    data.Read(pTransferBuffer, size);
                    txOk = DoSendOneBlockDataAndWaitAck(pTransferBuffer, size);
                    if (txOk)
                    {
                        tftpEvent = last 
                            ? TFTPD_EVENT_READ_TRANSFER_DONE
                            : TFTPD_EVENT_READ_TRANSFER_UPDATE;
                        NotifyMainThread(tftpEvent, _file, _dataBlock, totalBlocks);

                        /* advance block count */
                        _dataBlock++;
                    }
                    else
                    {
                        NotifyMainThread(TFTPD_EVENT_ERROR, wxT("Timeout!"),
                            TFTPD_ERROR_NOT_DEFINE);
                        break;
                    }
                }

                delete [] pTransferBuffer;
            }
        }
        else
        {
            DoSendError(TFTPD_ERROR_FILE_NOT_FOUND, wxT("File not found!"));
            NotifyMainThread(TFTPD_EVENT_ERROR, wxT("File not found!"), TFTPD_ERROR_FILE_NOT_FOUND);
        }
    }
    else // WRQ
    {
        mode = (_mode == TFTPD_TRANSFER_MODE_BINARY) ? wxT("wb") : wxT("w");

        if (!exist)
        {
            wxFFileOutputStream data(_file, mode);

            if (data.IsOk())
            {
                while (true)
                {
                    rxOk = DoSendAckAndWaitOneBlockData(&receiveBuffer[0], &rxLen);
                    if (rxOk)
                    {
                        wxLogMessage(wxT("rxOK, len = %d"), rxLen);
                        if (rxLen != 0)
                            data.Write(&receiveBuffer[0], rxLen);
                        last = (rxLen < 512);
                        tftpEvent = last
                            ? TFTPD_EVENT_WRITE_TRANSFER_DONE
                            : TFTPD_EVENT_WRITE_TRANSFER_UPDATE;
                        NotifyMainThread(tftpEvent, _file, _ackBlock, _ackBlock);
                        if (last)
                            break;

                        /* advance ack # */
                        _ackBlock++;
                    }
                    else
                    {
                        wxLogMessage(wxT("rxNG"));
                        NotifyMainThread(TFTPD_EVENT_ERROR, wxT("Timeout!"),
                            TFTPD_ERROR_NOT_DEFINE);
                        break;
                    }
                }
            }
        }
        else
        {
            DoSendError(TFTPD_ERROR_FILE_ALREADY_EXISTS, wxT("File alreay exists!"));
            NotifyMainThread(TFTPD_EVENT_ERROR, wxT("File already exists!"), TFTPD_ERROR_FILE_ALREADY_EXISTS);
        }
    }

    return (wxThread::ExitCode)0;
}

void TftpdTransmissionThread::DoSendOptAckAndWaitAckIfNeed(long fileLen)
{
    unsigned char txBuf[256], rxBuf[4];
    size_t idx;
    wxStopWatch stopWatch;
    wxSocketError error = wxSOCKET_NOERROR;
    int sendCnt;
    long totalTime = 0, diffTime = 0;
    wxUint32 count;
    short opcode, block;
    bool done = false, resend = false, timeout = false;
    bool optBlkSize = false, optTimeout = false, optTSize = false;

    /* init parameters per option negotiation or default value */
    if (_blkSize != -1)
        optBlkSize = true;
    else
        _blkSize = 512;
    if (_rexmt != -1)
        optTimeout = true;
    else
        _rexmt = 6 * 1000;
    _timeout = _rexmt * 5;
    if (_transferSize == 0)
        optTSize = true;

    /* if any option is used, send back option ack */
    if (optBlkSize || optTimeout || optTSize)
    {       
        memset(&txBuf[0], 0, sizeof(txBuf));
        txBuf[0] = (TFTP_OPCODE_OACK >> 8) & 0xFF;
        txBuf[1] = TFTP_OPCODE_OACK & 0xFF;
        idx = 2;
        if (optBlkSize)
        {
            sprintf((char *)&txBuf[idx], "blksize");
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
            sprintf((char *)&txBuf[idx], "%ld", _blkSize);
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
        }
        if (optTimeout)
        {
            sprintf((char *)&txBuf[idx], "timeout");
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
            sprintf((char *)&txBuf[idx], "%ld", _rexmt / 1000);
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
        }
        if (optTSize)
        {
            sprintf((char *)&txBuf[idx], "tsize");
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
            sprintf((char *)&txBuf[idx], "%ld", fileLen);
            idx = strlen((const char *)&txBuf[idx]) + idx + 1;
        }

        for (sendCnt = 1; true; sendCnt++)
        {
            _udpTransmissionSocket->SendTo(_remote, txBuf, idx);
            if (!_udpTransmissionSocket->Error())
            {
                stopWatch.Start();

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

                            if ((opcode == TFTP_OPCODE_ACK)
                                && (block == 0))
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
                            diffTime = stopWatch.Time();
                            if (diffTime > _rexmt)
                            {
                                resend = true;
                                break;
                            }
                            else
                                continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                }


            }
            else
            {
                error = _udpTransmissionSocket->LastError();
            }

            totalTime += diffTime;
            if (totalTime > _timeout)
                timeout = true;

            if (done)
                break;
            else if (timeout)
                break;
            else if (resend)
                continue;
            else
                break;
        }
    }
}

//
// Send data to tftp client, and wait for ack...
// If ack is not received within 'rexmt' second, then resend...
// If ack is still not received within 'timeout' second, then abort...
//
// Input:
//  data - pointer to data buffer
//  len - length of data buffer
// Output:
//  None
// Return:
//  true - transmission done
//  false - transmission timeout
//
bool TftpdTransmissionThread::DoSendOneBlockDataAndWaitAck(void *data, long len)
{
    unsigned char *txBuf = new unsigned char[len + 4];
    unsigned char rxBuf[4];
    wxSocketError error = wxSOCKET_NOERROR;
    wxUint32 count;
    short opcode, block;
    bool done = false, resend = false, timeout = false;
    wxStopWatch stopWatch;
    long totalTime = 0, diffTime = 0;

    txBuf[0] = (TFTP_OPCODE_DATA >> 8) & 0xFF;
    txBuf[1] = TFTP_OPCODE_DATA & 0xFF;
    txBuf[2] = (_dataBlock >> 8) & 0xFF;
    txBuf[3] = _dataBlock & 0xFF;
    if (len > 0)
        memcpy(&txBuf[4], data, len);
    
    for (int sendCount = 0; true; sendCount++)
    {
        _udpTransmissionSocket->SendTo(_remote, txBuf, len + 4);
        if (!_udpTransmissionSocket->Error())
        {
            stopWatch.Start();

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
                        if ((opcode == TFTP_OPCODE_ACK) && (block == _dataBlock))
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
                        diffTime = stopWatch.Time();
                        if (diffTime > _rexmt)
                        {
                            resend = true;
                            break;
                        }
                        else
                            continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            error = _udpTransmissionSocket->LastError();
        }

        totalTime += diffTime;
        if (totalTime > _timeout)
            timeout = true;

        if (done) // transmission successfully
            break;
        else if (timeout) // transmission timeout
            break;
        else if (resend) // re-send
            continue;
        else // socket error?
            break;
    }

    delete [] txBuf;
    return done;
}

//
// Send ack to tftp client, and wait for data...
// If data is not received within 'rexmt' second, then resend...
// If data is still not received within 'timeout' second, then abort...
//
// Input:
//  data - pointer to buffer to receive data
//  len - pointer to long integer to receive data length
// Output:
//  len - save the data length to long integer pointed by len.
//        *len < 512, this is last block data, else not last block.
// Return:
//  true - receive done
//  false - receive timeout
//
bool TftpdTransmissionThread::DoSendAckAndWaitOneBlockData(void *data, long *len)
{
    unsigned char rxBuf[1024];
    unsigned char txBuf[4];
    wxSocketError error = wxSOCKET_NOERROR;
    wxUint32 count;
    short opcode, block;
    bool done = false, resend = false, timeout = false;
    wxStopWatch stopWatch;
    long totalTime = 0, diffTime = 0;

    txBuf[0] = (TFTP_OPCODE_ACK >> 8) & 0xFF;
    txBuf[1] = TFTP_OPCODE_ACK & 0xFF;
    txBuf[2] = (_ackBlock >> 8) & 0xFF;
    txBuf[3] = _ackBlock & 0xFF;
    
    for (int sendCount = 1; true; sendCount++)
    {
        _udpTransmissionSocket->SendTo(_remote, txBuf, 4);
        if (!_udpTransmissionSocket->Error())
        {
            stopWatch.Start();

            while (!TestDestroy())
            {
                _udpTransmissionSocket->RecvFrom(_remote, rxBuf, 1024);
                if (!_udpTransmissionSocket->Error())
                {
                    count = _udpTransmissionSocket->LastCount();
                    *len = count - 4;
                    opcode = (rxBuf[0] << 8) + rxBuf[1];
                    block = (rxBuf[2] << 8) + rxBuf[3];
                    if ((opcode == TFTP_OPCODE_DATA) && (block == _dataBlock))
                    {
                        memcpy(data, &rxBuf[4], *len);
                        done = true;
                        break;
                    }
                }
                else
                {
                    error = _udpTransmissionSocket->LastError();
                    if (error == wxSOCKET_WOULDBLOCK)
                    {
                        diffTime = stopWatch.Time();
                        if (diffTime > _rexmt)
                        {
                            resend = true;
                            break;
                        }
                        else
                            continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            error = _udpTransmissionSocket->LastError();
        }

        totalTime += diffTime;
        if (totalTime > _timeout)
            timeout = true;

        if (done) // transmission successfully
            break;
        else if (timeout) // transmission timeout
            break;
        else if (resend) // re-send
            continue;
        else // socket error?
            break;
    }

    return done;
}

void TftpdTransmissionThread::DoSendError(short error, const wxString &msg)
{
    unsigned long len = msg.length();
    unsigned char *txBuf = new unsigned char[len + 5];

    txBuf[0] = (TFTP_OPCODE_ERROR >> 8) & 0xFF;
    txBuf[1] = TFTP_OPCODE_ERROR & 0xFF;
    txBuf[2] = (error >> 8) & 0xFF;
    txBuf[3] = error & 0xFF;
    memcpy(&txBuf[4], msg.To8BitData(), len);
    txBuf[len + 4] = 0;

    _udpTransmissionSocket->SendTo(_remote, &txBuf[0], len + 5);
}

void TftpdTransmissionThread::NotifyMainThread(int evt, const wxString &str,
                                               const int num1, const int num2)
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    TftpdMessage msg(evt, str, num1, num2);
    event.SetPayload<TftpdMessage>(msg);
    wxQueueEvent(_pHandler, event.Clone());
}

