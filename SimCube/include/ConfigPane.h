#ifndef _CONFIG_PANE_H_
#define _CONFIG_PANE_H_

#include <wx/wx.h>

class ConfigPane : public wxPanel
{
public:
    // ctor
    ConfigPane();
    ConfigPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~ConfigPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();


};

#endif /* _CONFIG_PANE_H_ */

