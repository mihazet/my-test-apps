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
    void Init();
    void CreateControls();

private:
    wxTextCtrl *_logTextCtrl;
    wxImage *_verboseImg;
};

#endif /* _DEBUG_WIN_H_ */

