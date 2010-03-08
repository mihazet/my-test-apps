#ifndef _DOWNLOAD_PANE_H_
#define _DOWNLOAD_PANE_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/hyperlink.h>
#include <wx/infobar.h>

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
    // helper methods
    void Init();
    void CreateControls();
    void InitOptionValue();
    void SaveOptionValue();
    bool IsMACAddressInvalid(const wxString & mac_address);

    // event handlers
    void OnSearchButtonClicked(wxCommandEvent &event);
    void OnDownloadButtonClicked(wxCommandEvent &event);
    void OnModifyMACButtonClicked(wxCommandEvent &event);
    void OnUpdateDownloadButton(wxUpdateUIEvent &event);
    void OnUpdateGlobalFilePath(wxUpdateUIEvent &event);
    void OnSearchThread(wxThreadEvent &event);
    void OnUpdateThread(wxThreadEvent &event);
    void OnDeviceCheckAll(wxHyperlinkEvent &event);
    void OnDeviceUncheckAll(wxHyperlinkEvent &event);
    void OnDeviceListSelectNone(wxHyperlinkEvent &event);
    
    // data member
    int _updateThreadCount;
    wxInfoBar *_promptForModifyMAC;
    wxInfoBar *_promptForUpdateError;

    DECLARE_EVENT_TABLE()
};

#endif /* _DOWNLOAD_PANE_H_ */
