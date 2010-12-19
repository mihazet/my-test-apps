#include <wx/wx.h>
#include "PeerPane.h"
#include "SimCubeApp.h"

PeerPane::PeerPane()
{
    Init();
}

PeerPane::PeerPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

PeerPane::~PeerPane()
{

}

bool PeerPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void PeerPane::Init()
{
}

void PeerPane::CreateControls()
{
    wxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    _peerView = new wxDataViewCtrl(this, wxID_ANY);
    paneSizer->Add(_peerView, 1, wxALL | wxEXPAND, 5);
    _peerView->AssociateModel(wxGetApp().m_PeerData);
    _peerView->AppendTextColumn(wxT("#"), 0, wxDATAVIEW_CELL_INERT, 25, wxALIGN_CENTER);
    _peerView->AppendTextColumn(_("IP Address"), 1, wxDATAVIEW_CELL_INERT, 120);
    _peerView->AppendTextColumn(_("Port"), 2, wxDATAVIEW_CELL_INERT, 60, wxALIGN_CENTER);
    _peerView->AppendTextColumn(_("Established Time"), 3, wxDATAVIEW_CELL_INERT, 160);
    _peerView->AppendTextColumn(_("Monitored"), 4, wxDATAVIEW_CELL_INERT, 80, wxALIGN_CENTER);
    SetSizerAndFit(paneSizer);
}

////////////////////////////////////////////////////////////////////////////
PeerDataModel::PeerDataModel() : wxDataViewVirtualListModel(0)
{
}

void PeerDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                     unsigned int col) const
{
    size_t count = _container.size();

    if (row >= count)
    {
        variant = "OutOfRange";
        return;
    }

    const PeerData &data = _container.at(row);
    switch (col)
    {
        case 0: /* row # */
            variant = wxString::Format(wxT("%lu"), row + 1);
            break;
        case 1: /* peer ip */
            variant = data.m_Peer.IPAddress();
            break;
        case 2: /* peer port */
            variant = wxString::Format(wxT("%lu"), data.m_Peer.Service());
            break;
        case 3: /* timestamp */
            variant = data.m_Timestamp;
            break;
        case 4: /* monitor */
            variant = data.m_Monitor ? wxT("Yes") : wxT("No");
            break;
        default:
            variant = "NonExistCol";
            break;
    }
}

bool PeerDataModel::SetValueByRow(const wxVariant &WXUNUSED(variant),
                                  unsigned int WXUNUSED(row),
                                  unsigned int WXUNUSED(col))
{
    return false;
}

bool PeerDataModel::IsContain(wxIPV4address &peer)
{
    bool contained = false;

    for (wxVector<PeerData>::iterator it = _container.begin();
        it != _container.end();
        it++)
    {
        if ((it->m_Peer.IPAddress() == peer.IPAddress())
            && (it->m_Peer.Service() == peer.Service()))
        {
            contained = true;
            break;
        }
    }

    return contained;
}

void PeerDataModel::AddData(PeerData &data)
{
    _container.push_back(data);
    Reset((unsigned int)_container.size());
}

bool PeerDataModel::RemoveData(wxIPV4address &peer)
{
    bool remove_done = false;

    for (wxVector<PeerData>::iterator it = _container.begin();
        it != _container.end();
        ++it)
    {
        if ((it->m_Peer.IPAddress() == peer.IPAddress())
            && (it->m_Peer.Service() == peer.Service()))
        {
            _container.erase(it);
            remove_done = true;
            break;
        }
    }

    if (remove_done)
        Reset((unsigned int)_container.size());

    return remove_done;
}

bool PeerDataModel::SetMonitor(wxIPV4address &peer, bool monitor)
{
    bool exist = false, update = false;
    wxVector<PeerData>::iterator it;
    unsigned int row = 0;

    for (it = _container.begin(); it != _container.end(); ++it, ++row)
    {
        if ((it->m_Peer.IPAddress() == peer.IPAddress())
            && (it->m_Peer.Service() == peer.Service()))
        {
            exist = true;
            break;
        }
    }

    if (exist)
    {
        if (it->m_Monitor != monitor)
            update = true;
        it->m_Monitor = monitor;
    }

    if (update)
        RowChanged(row);

    return exist;
}

bool PeerDataModel::GetMonitor(wxIPV4address &peer)
{
    wxVector<PeerData>::iterator it;

    for (it = _container.begin(); it != _container.end(); ++it)
    {
        if ((it->m_Peer.IPAddress() == peer.IPAddress())
            && (it->m_Peer.Service() == peer.Service()))
            return it->m_Monitor;
    }

    return false;
}

void PeerDataModel::SendMessageToMonitors(const wxString &msg)
{
    wxVector<NetAdapter> &adapter = wxGetApp().m_Adapters;
    wxVector<PeerData>::iterator it;

    for (it = _container.begin(); it != _container.end(); ++it)
    {
        if (it->m_Monitor)
        {
            adapter.at(it->m_AdapterId).udp->SendToWithRecord(
                it->m_Peer, msg.ToAscii(), msg.length() + 1);
        }
    }
}

