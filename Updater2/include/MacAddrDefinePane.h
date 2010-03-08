#ifndef _MAC_ADDR_DEFINE_PANE_H_
#define _MAC_ADDR_DEFINE_PANE_H_

#include <wx/wx.h>

class MacAddrDefinePane : public wxPanel
{
public:
    // ctors
    MacAddrDefinePane();
    MacAddrDefinePane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~MacAddrDefinePane();

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

#endif /* _MAC_ADDR_DEFINE_PANE_H_ */
