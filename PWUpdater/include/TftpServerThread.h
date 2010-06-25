/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _TFTP_SERVER_THREAD_H_
#define _TFTP_SERVER_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

/* Definition of event */
enum
{
    TFTP_SERVER_MSG_READ_REQUEST,
    TFTP_SERVER_MSG_WRITE_REQUEST,
    TFTP_SERVER_MSG_ERROR,

    TFTP_SERVER_MSG_INVALID
};

/* Definition for TFTP_SERVER_MSG_{READ,WRITE}_REQUEST */
enum
{
    TFTP_TRANSFER_MODE_ASCII,
    TFTP_TRANSFER_MODE_BINARY,

    TFTP_TRANSFER_MODE_INVALID
};

/* Definition for TFTP_SERVER_MSG_ERROR */
enum
{
    TFTP_ERROR_CODE_NOT_DEFINE,
    TFTP_ERROR_CODE_FILE_NOT_FOUND,
    TFTP_ERROR_CODE_ACCESS_VIOLATION,
    TFTP_ERROR_CODE_DISK_FULL,
    TFTP_ERROR_CODE_ILLEGAL_OPERATION,
    TFTP_ERROR_CODE_UNKNOWN_TRANSFER_ID,
    TFTP_ERROR_CODE_FILE_ALREADY_EXISTS,
    TFTP_ERROR_CODE_NO_SUCH_USER,

    TFTP_ERROR_CODE_INVALID
};

class TftpServerMessage
{
public:
    // default ctor must required...
    TftpServerMessage()
    {
        _event = TFTP_SERVER_MSG_INVALID;
        _str = wxEmptyString;
        _num = -1;
    }
    TftpServerMessage(int event,
        const wxString &string,
        const int num)
    {
        _event = event;
        _num = num;
        _str = string;
    }
    void SetRemote(const wxIPV4address &remote) { _remote = remote; }
    wxIPV4address GetRemote() { return _remote; }
    int GetEvent() { return _event; }
    void GetValue(int &num) { num = _num; }
    void GetValue(wxString &str) { str = _str; }
private:
    int _event;
    int _num;
    wxString _str;
    wxIPV4address _remote;
};

class TftpServerThread : public wxThread
{
public:
    TftpServerThread(wxEvtHandler *handler);
    ~TftpServerThread();

private:
    virtual wxThread::ExitCode Entry();
    TftpServerMessage *ProtocolParser(unsigned char *buf, unsigned int len);
    bool IsSingleNullTerminatedString(unsigned char *buf, unsigned int len);
    bool IsTwoNullTerminatedString(unsigned char *buf, unsigned int len, unsigned char **str2);

    wxEvtHandler *_pHandler;
    wxDatagramSocket *_udpServerSocket;
};


#endif /* _TFTP_SERVER_THREAD_H_ */
