#ifndef _UPDATE_THREAD_H_
#define _UPDATE_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

class UpdateThread : public wxThread
{
public:
    UpdateThread(wxEvtHandler *handler, const wxString &remote, const int row);
    ~UpdateThread();
    virtual wxThread::ExitCode Entry();

private:
    wxEvtHandler *_pHandler;
    wxString _targetIpAddress;
    wxString _localIpAddress;
    int _row;
    wxSocketClient *_tcp;
    unsigned char *_recvBuf;
};

typedef enum
{
    UPDATE_THREAD_COMPLETED,
    UPDATE_THREAD_PROGRESS,
} UTMType;

class UpdateThreadMessage
{
public:
    UpdateThreadMessage()
    {
        type = UPDATE_THREAD_COMPLETED;
        progress = 0;
    }
    UTMType type;
    wxString targetIpAddress;
    int progress;
    int row;
    int error;
};

// -------------------------------------------------------------------------
// Thread error code definition
// The SOI official updater uses postive number as error code, and we follow
// its error number, and use negative number for internal error code
// -------------------------------------------------------------------------
#define UTERROR_NOERROR             0

/* error code compatible with SOI updater */
#define UTERROR_BAD_IMAGE_SYNTAX    2
#define UTERROR_RAM_CHECKSUM        3
#define UTERROR_BREC_CHECKSUM       4
#define UTERROR_LOAD_MODE           5
#define UTERROR_VER                 6
#define UTERROR_MALLOC              7
#define UTERROR_ERASH_FLASH         8
#define UTERROR_READ_FLASH          9
#define UTERROR_WRITE_FLASH         10
#define UTERROR_FW_SIZE             11
#define UTERROR_BTL_SIZE            12

/* error code use by ourself */
#define UTERROR_SOCKET_INIT         -1
#define UTERROR_CONNECT             -2


#endif /* _UPDATE_THREAD_H_ */
