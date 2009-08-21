#ifndef _PEER_PANE_H_
#define _PEER_PANE_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/wxsqlite3.h>

class PeerDataModel;

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

    wxDataViewCtrl *_peerView;
    class PeerDataModel *_peerData;
};

class PeerDataModel : public wxDataViewVirtualListModel
{
public:
    PeerDataModel();

    virtual unsigned int GetColumnCount() const
    {
        return 5;
    }

    virtual wxString GetColumnType(unsigned int WXUNUSED(col)) const
    {
        return wxT("string");
    }

    virtual unsigned int GetRowCount()
    {
        return 4;
    }

    virtual void GetValueByRow(wxVariant &variant, unsigned int row,
        unsigned int col) const;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row,
        unsigned int col);

private:
    wxSQLite3Database *_db;
};

#endif /* _PEER_PANE_H_ */

