#ifndef _DEBUG_WIN_H_
#define _DEBUG_WIN_H_

#include <wx/wx.h>

class DebugPane : public wxPanel
{
public:
    // ctor
    DebugPane();
    DebugPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~DebugPane();

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

private:
    wxTextCtrl *_logTextCtrl;
    wxImage *_verboseImg;
};

#endif /* _DEBUG_WIN_H_ */

