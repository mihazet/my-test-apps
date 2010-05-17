#ifndef _MAC_ADDR_USAGE_PANE_H_
#define _MAC_ADDR_USAGE_PANE_H_

#include <wx/wx.h>
#include <wx/dataview.h>

class ReportDataModel;

class MacAddrUsagePane : public wxPanel
{
public:
    // ctors
    MacAddrUsagePane();
    MacAddrUsagePane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~MacAddrUsagePane();

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
    void OnReportImport(wxCommandEvent &event);
    void OnReportExport(wxCommandEvent &event);
    void OnReportClear(wxCommandEvent &event);

    // private data members
    wxDataViewCtrl *_reportView;
    ReportDataModel *_reportModel;
};

#endif /* _MAC_ADDR_USAGE_PANE_H_ */
