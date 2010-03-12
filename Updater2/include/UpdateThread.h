#ifndef _UPDATE_THREAD_H_
#define _UPDATE_THREAD_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>

#define UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD  wxT("|")

typedef enum
{
    UPDATE_THREAD_DOWNLOAD_FIRMWARE_COMPLETED,
    UPDATE_THREAD_DOWNLOAD_BOOTLOADER_COMPLETED,
    UPDATE_THREAD_ERASE_MANAGEMENT_DATA_COMPLETED,
    UPDATE_THREAD_MODIFY_MAC_ADDRESS_COMPLETED,
    UPDATE_THREAD_DO_NOTHING_COMPLETED,
    UPDATE_THREAD_PROGRESS,
} UTMType;

class UpdateThreadMessage
{
public:
    UpdateThreadMessage()
    {
        type = UPDATE_THREAD_DO_NOTHING_COMPLETED;
        payload = wxEmptyString;
    }
    UTMType type;
    wxString payload;
};

class UpdateThread : public wxThread
{
public:
    UpdateThread(wxEvtHandler *handler, const wxString &codedString,
        const wxString &image, const wxString &newMACAddress = wxEmptyString);
    ~UpdateThread();

private:
    void SendNotification(const UTMType type, const int data);
    virtual wxThread::ExitCode Entry();

    wxEvtHandler *_pHandler;
    wxString _localIpAddress;
    int _row;
    wxString _targetName;
    wxString _targetIpAddress;
    wxString _targetMacAddress;
    wxString _imageFilePath;
    wxString _newMACAddress;
    wxSocketClient *_tcp;
    unsigned char *_recvBuf;
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
#define UTERROR_SOCKET_CONNECT      -2
#define UTERROR_CONNECT             -3
#define UTERROR_FILE_STREAM         -4
#define UTERROR_SOCKET_WRITE        -5
#define UTERROR_SOCKET_READ         -6
#define UTERROR_BREC_SYNTAX         -7
#define UTERROR_UNKNOWN             -100

#endif /* _UPDATE_THREAD_H_ */
