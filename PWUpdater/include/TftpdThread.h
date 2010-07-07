/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _TFTPD_THREAD_H_
#define _TFTPD_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

//
// Definition of event
//
enum
{
    TFTPD_EVENT_READ_REQUEST,
        // _num1 = file tranfer mode
        // _str = transfer file name
    TFTPD_EVENT_READ_TRANSFER_UPDATE,
        // _str = transfer file name
        // _num1 = current transfer block #
        // _num2 = total transfer block #
    TFTPD_EVENT_READ_TRANSFER_DONE,
        // _str = transfer file name
        // _num1 = total transfer block #
        // _num2 = total transfer block #
    TFTPD_EVENT_WRITE_REQUEST,
        // _num1 = file tranfer mode
        // _str = transfer file name
    TFTPD_EVENT_WRITE_TRANSFER_UPDATE,
        // _str = transfer file name
        // _num1 = current transfer block #
        // _num2 = total transfer block #
    TFTPD_EVENT_WRITE_TRANSFER_DONE,
        // _str = transfer file name
        // _num1 = total transfer block #
        // _num2 = total transfer block #
    TFTPD_EVENT_ERROR,
        // _num1 = error code
        // _str = error message from tftp client

    TFTPD_EVENT_INVALID
};

/* Definition for TFTPD_EVENT_{READ,WRITE}_REQUEST */
enum
{
    TFTPD_TRANSFER_MODE_ASCII,
    TFTPD_TRANSFER_MODE_BINARY,

    TFTPD_TRANSFER_MODE_INVALID
};

/* Definition for TFTPD_EVENT_ERROR */
enum
{
    TFTPD_ERROR_NOT_DEFINE              = 0,
    TFTPD_ERROR_FILE_NOT_FOUND          = 1,
    TFTPD_ERROR_ACCESS_VIOLATION        = 2,
    TFTPD_ERROR_DISK_FULL               = 3,
    TFTPD_ERROR_ILLEGAL_OPERATION       = 4,
    TFTPD_ERROR_UNKNOWN_TRANSFER_ID     = 5,
    TFTPD_ERROR_FILE_ALREADY_EXISTS     = 6,
    TFTPD_ERROR_NO_SUCH_USER            = 7,

    TFTPD_ERROR_INVALID
};

//
// Communication object between main thread and tftpd thread
//
class TftpdMessage
{
public:
    // default ctor is required by wx internal storage, wxAny...
    TftpdMessage()
    {
        _event = TFTPD_EVENT_INVALID;
        _str = wxEmptyString;
        _num1 = _num2 = -1;
    }
    TftpdMessage(int event,
        const wxString &str = wxEmptyString,
        const int num1 = -1, const int num2 = -1)
    {
        _event = event;
        _num1 = num1;
        _num2 = num2;
        _str = str;
    }
    int GetEvent() { return _event; }
    int GetNum1(int *num = NULL) { if (num) *num = _num1; return _num1; }
    int GetNum2(int *num = NULL) { if (num) *num = _num2; return _num2; }
    wxString GetString() { return _str; }
private:
    int _event;
    int _num1;
    int _num2;
    wxString _str;
};

//
// Server Thread
// Response to client initial connection request
//
class TftpdServerThread : public wxThread
{
public:
    TftpdServerThread(wxEvtHandler *handler, const int id,
        wxIPV4address &local, const wxString &rootPath = wxEmptyString);
    ~TftpdServerThread();

private:
    virtual wxThread::ExitCode Entry();
    TftpdMessage *ProtocolParser(unsigned char *buf, unsigned int len);
    bool IsSingleNullTerminatedString(unsigned char *buf, unsigned int len);
    bool IsTwoNullTerminatedString(unsigned char *buf, unsigned int len,
        unsigned char **str2);
    void DoStartTransmissionThread(const wxIPV4address &remote,
        const wxString &file, const int mode, bool rrq);

    wxEvtHandler *_pHandler;
    int _threadEventId;
    wxString _rootPath;
    wxDatagramSocket *_udpServerSocket;
};


//
// Transmission Thread
// Transfer data to/from tftp client
//
class TftpdTransmissionThread : public wxThread
{
public:
    TftpdTransmissionThread(wxEvtHandler *handler,
        const int threadEventId,
        const wxIPV4address &remote,
        const wxString &file = wxEmptyString,
        bool read = true, int mode = TFTPD_TRANSFER_MODE_BINARY);
    ~TftpdTransmissionThread();
    void SetRetransmitInterval(long ms);
    void SetTotalTimeout(long ms);

private:
    virtual wxThread::ExitCode Entry();
    bool DoSendOneBlockDataAndWaitAck(void *data, long len);
    bool DoSendAckAndWaitOneBlockData(void *data, long *len);
    void DoSendError(short error, const wxString &msg = wxEmptyString);
    void NotifyMainThread(int evt, const wxString &str,
        const int num1, const int num2 = -1);

    wxEvtHandler *_pHandler;
    wxDatagramSocket *_udpTransmissionSocket;
    wxIPV4address _remote;
    wxString _file;
    bool _read;
    int _mode;
    int _threadEventId;
    short _dataBlock;
    short _ackBlock;
    long _rexmt;
    long _timeout;
};

#endif /* _TFTPD_THREAD_H_ */

