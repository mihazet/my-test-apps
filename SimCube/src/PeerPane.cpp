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
    _peerData = new PeerDataModel();
    _peerView->AssociateModel(_peerData);
    _peerView->AppendTextColumn(wxT("#"), 0, wxDATAVIEW_CELL_INERT, 40);
    _peerView->AppendTextColumn(_("Remote IP Addr"), 1, wxDATAVIEW_CELL_INERT, 120);
    _peerView->AppendTextColumn(_("Port"), 2, wxDATAVIEW_CELL_INERT, 80);
    _peerView->AppendTextColumn(_("Established Time"), 3, wxDATAVIEW_CELL_INERT, 120);
    _peerView->AppendTextColumn(_("Monitored"), 4, wxDATAVIEW_CELL_INERT, 80);
    SetSizerAndFit(paneSizer);
}

////////////////////////////////////////////////////////////////////////////
PeerDataModel::PeerDataModel() : _peers(wxGetApp().m_Peers), wxDataViewVirtualListModel()
{
}

void PeerDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                     unsigned int col) const
{
    size_t numPeer = _peers.size();

    if (row >= numPeer)
    {
        variant = "OutOfRange";
        return;
    }

    PeerData &peer = _peers.at(row);
    switch (col)
    {
        case 0: /* row # */
            variant = wxString::Format(wxT("%lu"), row + 1);
            break;
        case 1: /* peer ip */
            variant = peer.m_Peer.IPAddress();
            break;
        case 2: /* peer port */
            variant = wxString::Format(wxT("%lu"), peer.m_Peer.Service());
            break;
        case 3: /* timestamp */
            variant = peer.m_Timestamp;
            break;
        case 4: /* monitor */
            variant = peer.m_Monitor ? wxT("Yes") : wxT("No");
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

