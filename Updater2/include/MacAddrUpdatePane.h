#ifndef _MAC_ADDR_UPDATE_PANE_H_
#define _MAC_ADDR_UPDATE_PANE_H_

#include <wx/wx.h>

class MacAddrUpdatePane : public wxPanel
{
public:
    // ctors
    MacAddrUpdatePane();
    MacAddrUpdatePane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~MacAddrUpdatePane();

    // public APIs
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // public data members

private:
    // helper methods
    void Init();
    void CreateControls();

    // event handlers

    
    // private data members
};

#endif /* _MAC_ADDR_UPDATE_PANE_H_ */
