#ifndef _MAC_ADDR_UPDATE_PANE_H_
#define _MAC_ADDR_UPDATE_PANE_H_

#include <wx/wx.h>

class MacAddrUpdatePane : public wxPanel
{
public:
    // ctors
    MacAddrUpdatePane();
    MacAddrUpdatePane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString& codeString = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~MacAddrUpdatePane();

    // public APIs
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString& codeString = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // public data members

private:
    // helper methods
    void Init();
    void CreateControls();

    // event handlers
    void OnUpdateButtonClicked(wxCommandEvent& event);
    void OnCloseBttonClicked(wxCommandEvent& event);
    
    // private data members
    wxString _codeString;
    wxString _name;
    wxString _ip;
    wxString _mac;
    long _row;
    wxButton *_multiFunctionBtn;
};

#endif /* _MAC_ADDR_UPDATE_PANE_H_ */
