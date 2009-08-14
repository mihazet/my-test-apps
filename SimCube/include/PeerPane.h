#ifndef _PEER_PANE_H_
#define _PEER_PANE_H_

#include <wx/wx.h>

class PeerPane : public wxPanel
{
public:
    // ctor
    PeerPane();
    PeerPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~PeerPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();


};

#endif /* _PEER_PANE_H_ */

