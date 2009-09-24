#ifndef _PROPERTY_PANE_H_
#define _PROPERTY_PANE_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wxsqlite3.h>

class PropertyPane : public wxPanel, public wxSQLite3Hook
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

    virtual void UpdateCallback(wxUpdateType, const wxString &,
        const wxString &, wxLongLong);

    // event handlers
    void OnPropertyChanging(wxPropertyGridEvent &event);
    void OnPropertyChanged(wxPropertyGridEvent &event);
    void OnPropertyReset(wxCommandEvent &event);

    wxSQLite3Database *_propDB;
    wxPropertyGrid *_pg;
    bool _pgUpdatedFromUI;

    DECLARE_EVENT_TABLE()
};

#endif /* _PROPERTY_PANE_H_ */

