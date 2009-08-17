#include <wx/wx.h>
#include "SimCubeApp.h"
#include "HistoryPane.h"

HistoryPane::HistoryPane()
{
    Init();
}

HistoryPane::HistoryPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

HistoryPane::~HistoryPane()
{

}

bool HistoryPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void HistoryPane::Init()
{
}

void HistoryPane::CreateControls()
{
    wxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    _historyView = new wxDataViewCtrl(this, wxID_ANY);
    paneSizer->Add(_historyView, 1, wxALL | wxEXPAND, 5);
    _historyData = new HistoryDataModel();
    _historyView->AssociateModel(_historyData);
    _historyView->AppendTextColumn(wxT("#"), 0, wxDATAVIEW_CELL_INERT, 40);
    _historyView->AppendTextColumn(_("Remote IP Addr"), 1, wxDATAVIEW_CELL_INERT, 120);
    _historyView->AppendTextColumn(_("Port"), 2, wxDATAVIEW_CELL_INERT, 80);
    _historyView->AppendTextColumn(_("Content"), 3, wxDATAVIEW_CELL_INERT, 360);
    _historyView->AppendTextColumn(_("Length"), 4, wxDATAVIEW_CELL_INERT, 80);
    SetSizerAndFit(paneSizer);
}

////////////////////////////////////////////////////////////////////////////
HistoryDataModel::HistoryDataModel() : wxDataViewVirtualListModel(2)
{
    _db = wxGetApp().GetMemDatabase();
}

void HistoryDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                     unsigned int col) const
{
    if (row)
        variant = "xxx";
    else
        variant = "x";
}

bool HistoryDataModel::SetValueByRow(const wxVariant &variant,
                                     unsigned int row, unsigned int col)
{
    return false;
}

