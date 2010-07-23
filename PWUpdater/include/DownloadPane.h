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
    void SearchImageFiles();
    void DoStartTftpServerThread(const wxString &ipAddr = wxEmptyString,
        const wxString &root = wxEmptyString);
    void DoStopTftpServerThread();
    void DoStartUartThread();
    void DoSearchLocalImageFiles();
    int DoSearchFreeSerialPort(bool update = true);

    // event handlers
    void OnThreadTftpd(wxThreadEvent &event);
    void OnButtonSerialPortRefresh(wxCommandEvent &event);
    void OnButtonDownload(wxCommandEvent &event);

    // data member
    wxVector<int> _serialPort;

    DECLARE_EVENT_TABLE()
};

#endif /* _DOWNLOAD_PANE_H_ */