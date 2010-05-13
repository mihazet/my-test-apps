#ifndef _MAC_ADDR_UPDATE_PANE_H_
#define _MAC_ADDR_UPDATE_PANE_H_

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/wxsqlite3.h>

class MacAddrUpdatePane : public wxPanel
{
public:
    // ctors
    MacAddrUpdatePane();
    MacAddrUpdatePane(wxWindow *parent, wxWindowID id,
        const wxString& codeString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~MacAddrUpdatePane();

    // public APIs
    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& codeString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // public data members

private:
    // helper methods
    void Init();
    void CreateControls();
    void UpdateReportFile(const wxString& macAddr);
    wxSQLite3Database *OpenReportDatabase(int reportRotateType);
    void CloseReportDatabase(wxSQLite3Database *db);

    // event handlers
    void OnUpdateButtonClicked(wxCommandEvent& event);
    void OnCloseBttonClicked(wxCommandEvent& event);
    void OnThread(wxThreadEvent& event);
    void OnTimer(wxTimerEvent& event);
    
    // private data members
    wxString _codeString;
    wxString _name;
    wxString _ip;
    wxString _mac;
    long _row;
    wxStaticBitmap *_resultBitmap;
    const static wxBitmap _resultUnknown;
    const static wxBitmap _resultPass;
    const static wxBitmap _resultFail;
    wxButton *_multiFunctionBtn;
    wxTimer *_closeSelfTimer;
    long _closeTimeout;

    DECLARE_EVENT_TABLE()
};

#endif /* _MAC_ADDR_UPDATE_PANE_H_ */
