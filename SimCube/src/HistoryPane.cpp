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
    _historyView->AssociateModel(wxGetApp().m_HistoryData);
    _historyView->AppendTextColumn(wxT("#"), 0, wxDATAVIEW_CELL_INERT, 30);
    _historyView->AppendTextColumn(_("Timestamp"), 1, wxDATAVIEW_CELL_INERT, 160);
    _historyView->AppendTextColumn(_("IP Address"), 2, wxDATAVIEW_CELL_INERT, 120);
    _historyView->AppendTextColumn(_("Port"), 3, wxDATAVIEW_CELL_INERT, 60);
    _historyView->AppendTextColumn(_("Length"), 4, wxDATAVIEW_CELL_INERT, 80);
    _historyView->AppendTextColumn(_("Message"), 5, wxDATAVIEW_CELL_INERT, 360);
    SetSizerAndFit(paneSizer);
}

////////////////////////////////////////////////////////////////////////////
HistoryDataModel::HistoryDataModel(wxSQLite3Database *db)
    : wxDataViewVirtualListModel()
{
    _db = db;

    /* create table */
}

void HistoryDataModel::GetValueByRow(wxVariant &variant,
                                     unsigned int row,
                                     unsigned int WXUNUSED(col)) const
{
    if (row)
        variant = "xxx";
    else
        variant = "x";
}

bool HistoryDataModel::SetValueByRow(const wxVariant &WXUNUSED(variant),
                                     unsigned int WXUNUSED(row),
                                     unsigned int WXUNUSED(col))
{
    return false;
}

