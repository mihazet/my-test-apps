#ifndef _DOWNLOAD_PANE_H_
#define _DOWNLOAD_PANE_H_

#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/ctb/serport.h>

class DownloadPane : public wxPanel
{
public:
    // ctor
    DownloadPane();
    DownloadPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~DownloadPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    void RescanImageFiles();

private:
    // helper methods
    void Init();
    void CreateControls();
    bool StartInternalTftpIfNeed();
    void SearchImageFiles(int mode);
    void DoStartTftpServerThread(const wxString &ipAddr = wxEmptyString,
        const wxString &root = wxEmptyString);
    void DoStopTftpServerThread();
    wxString GetTftpServerIpAddress();
    void StartUartThread();
    void DoSearchLocalImageFiles(int mode);
    wxString GetNextDownloadFile(const wxString &currentFile = wxEmptyString);
    void GetFileInfo(const wxString &file, unsigned long *offset,
        unsigned long *end, unsigned long *size);

    // event handlers
    void OnThreadTftpd(wxThreadEvent &event);
    void OnThreadUart(wxThreadEvent &event);
    void OnButtonSerialPortRefresh(wxCommandEvent &event);
    void OnButtonDownload(wxCommandEvent &event);
    void OnUpdateUIButtonDownload(wxUpdateUIEvent &event);

    // data member
    wxVector<int> _serialPort;
    bool _downloading;

    DECLARE_EVENT_TABLE()
};

#endif /* _DOWNLOAD_PANE_H_ */
