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
PeerDataModel::PeerDataModel() : wxDataViewVirtualListModel(2)
{
    _db = wxGetApp().GetMemDatabase();
}

void PeerDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                     unsigned int col) const
{
    switch (col)
    {
        case 0:
            variant = wxString::Format(wxT("%lu"), row + 1);
            break;
        default:
            variant = "xx";
            break;
    }
}

bool PeerDataModel::SetValueByRow(const wxVariant &variant,
                                     unsigned int row, unsigned int col)
{
    return false;
}



