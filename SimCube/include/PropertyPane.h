#ifndef _PROPERTY_PANE_H_
#define _PROPERTY_PANE_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wxsqlite3.h>

class PropertyPane : public wxPanel
{
public:
    // ctor
    PropertyPane();
    PropertyPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~PropertyPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();

    // event handlers
    void OnPropertySelected(wxPropertyGridEvent &event);
    void OnPropertyChanging(wxPropertyGridEvent &event);
    void OnPropertyHighlighted(wxPropertyGridEvent &event);
    void OnPropertyRightClick(wxPropertyGridEvent &event);
    void OnPropertyDoubleClick(wxPropertyGridEvent &event);
    void OnPropertyItemCollapsed(wxPropertyGridEvent &event);
    void OnPropertyItemExpanded(wxPropertyGridEvent &event);

    wxSQLite3Database *_db;

    DECLARE_EVENT_TABLE()
};

#endif /* _PROPERTY_PANE_H_ */

