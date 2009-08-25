#include <wx/wx.h>
#include <wx/wxsqlite3.h>
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
    : wxDataViewVirtualListModel(), _db(db)
{
    wxString sqlUpdate;
    int modified_rows;

    /* create table */
    sqlUpdate << wxT("CREATE TABLE HistoryTbl (Id INTEGER PRIMARY KEY, ")
                                        << wxT("Timestamp TEXT, ")
                                        << wxT("Ip TEXT, ")
                                        << wxT("Port TEXT, ")
                                        << wxT("Length TEXT, ")
                                        << wxT("Data TEXT)");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);

    /* create trigger */
    sqlUpdate.clear();
    sqlUpdate << wxT("CREATE TRIGGER insert_timestamp AFTER INSERT ON HistoryTbl ")
              << wxT("BEGIN ")
              << wxT("UPDATE HistoryTbl SET Timestamp = DATETIME('NOW') WHERE rowid = new.rowid;")
              << wxT("END");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);
}

void HistoryDataModel::GetValueByRow(wxVariant &variant,
                                     unsigned int row,
                                     unsigned int col) const
{
    wxString sqlQuery;
    wxSQLite3ResultSet set;

    sqlQuery << wxT("SELECT ");
    switch (col)
    {
        case 0: sqlQuery << wxT("Id"); break;
        case 1: sqlQuery << wxT("Timestamp"); break;
        case 2: sqlQuery << wxT("Ip"); break;
        case 3: sqlQuery << wxT("Port"); break;
        case 4: sqlQuery << wxT("Length"); break;
        case 5: sqlQuery << wxT("Data"); break;
    }
    sqlQuery << wxT(" FROM HistoryTbl WHERE Id = ") << row + 1;

    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
    {
        variant = set.GetAsString(0);
    }
    set.Finalize();
}

bool HistoryDataModel::SetValueByRow(const wxVariant &WXUNUSED(variant),
                                     unsigned int WXUNUSED(row),
                                     unsigned int WXUNUSED(col))
{
    return false;
}

