#ifndef _HISTORY_PANE_H_
#define _HISTORY_PANE_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>
#include <wx/dataview.h>

class HistoryDataModel;
class HistoryPane : public wxPanel, public wxSQLite3Hook
{
public:
    // ctor
    HistoryPane();
    HistoryPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~HistoryPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();

    virtual void UpdateCallback(wxUpdateType type, const wxString &database,
        const wxString &table, wxLongLong rowid);

    // event handlers
    void OnSaveHistory(wxCommandEvent &event);
    void OnDeleteHistory(wxCommandEvent &event);
    void OnAutoscroll(wxCommandEvent &event);

    // helper functions
    bool IsAutoSaveEnabled();

    wxDataViewCtrl *_historyView;
    bool _historyAutoScroll;
    wxImage *_autoScrollBtnImg;

    DECLARE_EVENT_TABLE()
};

class HistoryDataModel : public wxDataViewVirtualListModel
{
public:
    HistoryDataModel(wxSQLite3Database *db);

    virtual unsigned int GetColumnCount() const
    {
        return 6;
    }

    virtual wxString GetColumnType(unsigned int WXUNUSED(col)) const
    {
        return wxT("string");
    }

    virtual unsigned int GetRowCount()
    {
        return (unsigned int)_db->GetLastRowId().ToLong();
    }

    virtual void GetValueByRow(wxVariant &variant, unsigned int row,
        unsigned int col) const;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row,
        unsigned int col);

    bool AddData(const wxString &ip, unsigned short port, const wxString &data, size_t len);

private:
    wxSQLite3Database *_db;
};

#endif /* _HISTORY_PANE_H_ */

