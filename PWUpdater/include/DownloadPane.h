#ifndef _DOWNLOAD_PANE_H_
#define _DOWNLOAD_PANE_H_

#include <wx/wx.h>
#include <wx/socket.h>

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
    void DoSearchLocalImageFiles();

    // event handlers
    void OnButtonStartTftp(wxCommandEvent &event);
    void OnButtonStopTftp(wxCommandEvent &event);
    void OnThreadTftpd(wxThreadEvent &event);

    // data member

    DECLARE_EVENT_TABLE()
};

#endif /* _DOWNLOAD_PANE_H_ */
