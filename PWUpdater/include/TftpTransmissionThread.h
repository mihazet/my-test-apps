/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _TFTP_TRANSMISSION_THREAD_H_
#define _TFTP_TRANSMISSION_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>
#include "TftpCommon.h"

/* Definition of event */
enum
{
    TFTP_TRANSMISSION_MSG_READ_TRANSFER_UPDATE,
        // _str = transfer file name
        // _num1 = current transfer block #
        // _num2 = total transfer block #
    TFTP_TRANSMISSION_MSG_READ_TRANSFER_DONE,
        // _str = transfer file name
        // _num1 = total transfer block #
        // _num2 = total transfer block #
    TFTP_TRANSMISSION_MSG_WRITE_TRANSFER_UPDATE,
        // _str = transfer file name
        // _num1 = current transfer block #
        // _num2 = total transfer block #
    TFTP_TRANSMISSION_MSG_WRITE_TRANSFER_DONE,
        // _str = transfer file name
        // _num1 = total transfer block #
        // _num2 = total transfer block #
    TFTP_TRANSMISSION_MSG_ERROR,
        // _str = error message for tftp client
        // _num1 = error code
        // _num2 = error code

    TFTP_TRANSMISSION_MSG_INVALID
};

class TftpTransmissionMessage
{
public:
    TftpTransmissionMessage()
    {
        _event = TFTP_TRANSMISSION_MSG_INVALID;
        _str = wxEmptyString;
        _num1 = _num2 = -1;
    }
    TftpTransmissionMessage(int event, const wxString& string,
        int num1, int num2)
    {
        _event = event;
        _str = string;
        _num1 = num1;
        _num2 = num2;
    }
    void SetRemote(const wxIPV4address &remote) { _remote = remote; }
    wxIPV4address GetRemote() { return _remote; }
    int GetEvent() { return _event; }
    void GetValue(int &num1, int &num2) { num1 = _num1; num2 = _num2; }
    void GetValue(wxString &str) { str = _str; }
private:
    int _event;
    int _num1;
    int _num2;
    wxString _str;
    wxIPV4address _remote;
};

class TftpTransmissionThread : public wxThread
{
public:
    TftpTransmissionThread(wxEvtHandler *handler,
        const wxIPV4address &remote,
        const wxString &file = wxEmptyString,
        bool read = true, int mode = TFTP_TRANSFER_MODE_BINARY);
    ~TftpTransmissionThread();

private:
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    wxDatagramSocket *_udpTransmissionSocket;
    wxIPV4address _remote;
    wxString _file;
    bool _read;
    int _mode;
};


#endif /* _TFTP_TRANSMISSION_THREAD_H_ */
