#ifndef _APP_PREFERENCE_PANE_H_
#define _APP_PREFERENCE_PANE_H_

#include <wx/wx.h>
#include <wx/filepicker.h>

class AppPreferencePane : public wxPanel
{
public:
    // ctors
    AppPreferencePane();
    AppPreferencePane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~AppPreferencePane();

    // public APIs
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // public data members

private:
    // helper methods
    void Init();
    void CreateControls();
    void OnUpdateSearchMethod(const int method);

    // event handlers
    void OnUpdateActivedInterface(wxCommandEvent& event);
    void OnUpdateSkipVMInterface(wxCommandEvent& event);
    void OnUseSearchMethod1(wxCommandEvent& event);
    void OnUseSearchMethod2(wxCommandEvent& event);
    void OnUpdateNumberOfSearch(wxCommandEvent& event);
    void OnUpdateReportLimitEntries(wxCommandEvent& event);
    void OnUpdateReportRotate(wxCommandEvent& event);
    void OnUpdateReportFolder(wxFileDirPickerEvent& event);
    void OnVerifyAndUpdateMAC(wxCommandEvent& event);
    void OnUpdateInvalidMAC(wxCommandEvent& event);
    
    // private data members
};

#endif /* _APP_PREFERENCE_PANE_H_ */
