#ifndef _APP_PREFERENCE_PANE_H_
#define _APP_PREFERENCE_PANE_H_

#include <wx/wx.h>

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

    // event handlers
    void OnUpdateActivedInterface(wxCommandEvent& event);
    void OnUpdateSkipVMInterface(wxCommandEvent& event);
    
    // private data members
};

#endif /* _APP_PREFERENCE_PANE_H_ */
