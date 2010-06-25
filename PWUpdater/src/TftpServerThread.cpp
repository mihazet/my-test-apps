/*
 *  TftpServerThread.cpp - A simple implementation for tftp server.
 *  Refer to RFC1350 for TFTP protocol detail.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifdef _MSC_VER
 #define _CRT_SECURE_NO_WARNINGS
#endif
// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wx/wx.h>
#include <wx/thread.h>
#include "TftpServerThread.h"
#include "PWUpdater.h"
#include "WidgetsId.h"

#define wxLOG_COMPONENT "PWUpdater/tftpserver"

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
    TFTP_OPCODE_ERROR   = 5
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
TftpServerThread::TftpServerThread(wxEvtHandler *handler)
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
    wxUint32 count;
    wxSocketError error;
    TftpServerMessage *msg = NULL;
    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_THREAD_SERVER);

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
                msg->SetRemote(remote);
                event.SetPayload<TftpServerMessage>(*msg);
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
TftpServerMessage *TftpServerThread::ProtocolParser(unsigned char *buf, unsigned int len)
{
    int opCode, errorCode, mode = TFTP_TRANSFER_MODE_INVALID;
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
                        mode = TFTP_TRANSFER_MODE_ASCII;
                    else if (!txMode.CmpNoCase(wxT("octet")))
                        mode = TFTP_TRANSFER_MODE_BINARY;

                    if (mode != TFTP_TRANSFER_MODE_INVALID)
                    {
                        return new TftpServerMessage(
                            (opCode == TFTP_OPCODE_RRQ)
                            ? TFTP_SERVER_MSG_READ_REQUEST
                            : TFTP_SERVER_MSG_WRITE_REQUEST,
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
                    return new TftpServerMessage(TFTP_SERVER_MSG_ERROR,
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

bool TftpServerThread::IsSingleNullTerminatedString(unsigned char *buf, unsigned int len)
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

bool TftpServerThread::IsTwoNullTerminatedString(unsigned char *buf, unsigned int len,
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

