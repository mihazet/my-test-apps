#ifndef _PROPERTY_PANE_H_
#define _PROPERTY_PANE_H_

#include <wx/wx.h>

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


};

#endif /* _PROPERTY_PANE_H_ */

