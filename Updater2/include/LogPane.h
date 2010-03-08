#ifndef _LOG_PANE_H_
#define _LOG_PANE_H_

#include <wx/wx.h>

class LogPane : public wxPanel
{
public:
    // ctor
    LogPane();
    LogPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~LogPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    wxTextCtrl *GetLogTextCtrl() { return _logTextCtrl; }

private:
    // helper functions
    void Init();
    void CreateControls();
    bool IsVerbose();
    void SetVerbose(bool verbose = true);

    // event handlers
    void OnVerbose(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnErase(wxCommandEvent &event);

private:
    wxTextCtrl *_logTextCtrl;
    wxImage *_verboseImg;
};

#endif /* _DEBUG_WIN_H_ */

