#ifndef _TRAP_PANE_H_
#define _TRAP_PANE_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class TrapPane : public wxPanel
{
public:
    // ctor
    TrapPane();
    TrapPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~TrapPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();

    wxSQLite3Database *_db;
};

#endif /* _TRAP_PANE_H_ */

