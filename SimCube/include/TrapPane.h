#ifndef _TRAP_PANE_H_
#define _TRAP_PANE_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class TrapPane : public wxPanel
{
public:
    // ctor
    TrapPane();
    TrapPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~TrapPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    // helper functions
    void Init();
    void CreateControls();
    void TrapString(const wxString &str);
    void TrapNumeric(const wxString &str);
    void UpdateString(const wxString &dbString, const wxString &str);
    void UpdateNumeric(const wxString &dbString, const int num);

    // event handlers
    void OnLedStatusChosen(wxCommandEvent &event);
    void OnLedPresetChosen(wxCommandEvent &event);
    void OnLedStatusSend(wxCommandEvent &event);
    void OnLampAStateSend(wxCommandEvent &event);
    void OnLampBStateSend(wxCommandEvent &event);
    void OnLampAHoursSend(wxCommandEvent &event);
    void OnLampBHoursSend(wxCommandEvent &event);
    void OnLampALitCntSend(wxCommandEvent &event);
    void OnLampBLitCntSend(wxCommandEvent &event);
    void OnLampATempCondSend(wxCommandEvent &event);
    void OnLampBTempCondSend(wxCommandEvent &event);
    void OnLampStatusChosen(wxCommandEvent &event);
    void OnLampTempCondChosen(wxCommandEvent &event);
    void OnLampHoursUpdated(wxCommandEvent &event);
    void OnLampLitCountUpdated(wxCommandEvent &event);

    wxSQLite3Database *_trapDB;
    wxSQLite3Database *_propDB;
    int _ledStatus;
};

#endif /* _TRAP_PANE_H_ */

