#ifndef _HISTORY_PANE_H_
#define _HISTORY_PANE_H_

#include <wx/wx.h>

class HistoryPane : public wxPanel
{
public:
    // ctor
    HistoryPane();
    HistoryPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~HistoryPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();


};

#endif /* _HISTORY_PANE_H_ */

