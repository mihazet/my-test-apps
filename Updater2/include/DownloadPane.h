#ifndef _DOWNLOAD_PANE_H_
#define _DOWNLOAD_PANE_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/hyperlink.h>

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

private:
    void Init();
    void CreateControls();

    // event handlers
    void OnSearchButtonClicked(wxCommandEvent &event);
    void OnUpdateButtonClicked(wxCommandEvent &event);
    void OnSearchThread(wxThreadEvent &event);
    void OnUpdateThread(wxThreadEvent &event);
    void OnTargetCheckAll(wxHyperlinkEvent &event);
    void OnTargetUncheckAll(wxHyperlinkEvent &event);
    void OnTargetListSelectNone(wxHyperlinkEvent &event);

    int _updateThreadCount;

    DECLARE_EVENT_TABLE()
};

#endif /* _DOWNLOAD_PANE_H_ */
