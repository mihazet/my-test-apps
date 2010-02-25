#ifndef _TARGETS_PANE_H_
#define _TARGETS_PANE_H_

#include <wx/wx.h>

class TargetsPane : public wxPanel
{
public:
    // ctor
    TargetsPane();
    TargetsPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~TargetsPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();

    // event handlers

    DECLARE_EVENT_TABLE()
};

#endif /* _TARGETS_PANE_H_ */
