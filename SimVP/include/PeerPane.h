#ifndef _PEER_PANE_H_
#define _PEER_PANE_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/socket.h>
#include <wx/datetime.h>

class PeerData
{
public:
    PeerData(int id, wxIPV4address &peer, wxDateTime timestamp, bool monitor = false)
    {
        m_AdapterId = id;
        m_Peer = peer;
        m_Timestamp = timestamp;
        m_Monitor = monitor;
    }
    int m_AdapterId;
    wxIPV4address m_Peer;
    wxDateTime m_Timestamp;
    bool m_Monitor;
};

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
        return (unsigned int)_container.size();
    }

    virtual void GetValueByRow(wxVariant &variant, unsigned int row,
        unsigned int col) const;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row,
        unsigned int col);

    bool IsContain(wxIPV4address &peer);
    void AddData(PeerData &data);
    bool RemoveData(wxIPV4address &peer);
    bool SetMonitor(wxIPV4address &peer, bool monitor = true);
    bool GetMonitor(wxIPV4address &peer);
    void SendMessageToMonitors(const wxString &msg);
private:
    wxVector<PeerData> _container;
};

#endif /* _PEER_PANE_H_ */

