#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/wxsqlite3.h>
#include "DataModel.h"

ReportDataModel::ReportDataModel()
    : wxDataViewVirtualListModel()
{
    _db = new wxSQLite3Database();
    _db->Open(wxT(":memory:"));
}

ReportDataModel::~ReportDataModel()
{
    if (_db)
    {
        if (_db->IsOpen())
            _db->Close();
        delete _db;
    }
}

unsigned int ReportDataModel::GetColumnCount() const
{
    return MAC_USAGE_MAX;
}

wxString ReportDataModel::GetColumnType(unsigned int col) const
{
    wxString type;

    switch (col)
    {
    case MAC_USAGE_ID:
    default:
        type = wxT("string");
        break;
    }

    return type;
}

unsigned int ReportDataModel::GetRowCount()
{
    unsigned int count = 0;

    if (_db->TableExists(wxT("ReportTable")))
    {
        wxSQLite3ResultSet set = _db->ExecuteQuery(wxT("SELECT count(*) from ReportTable"));
        if (set.NextRow())
        {
            count = (unsigned int)set.GetInt(0);
        }
        set.Finalize();
    }

    return count;
}

void ReportDataModel::GetValueByRow(wxVariant &variant, unsigned int row,
                                    unsigned int col) const
{
    wxString sqlQuery = wxT("SELECT ");
    wxSQLite3ResultSet set;

    switch (col)
    {
    default:
    case MAC_USAGE_ID: sqlQuery << wxT("Id"); break;
    case MAC_USAGE_DATE: sqlQuery << wxT("Date"); break;
    case MAC_USAGE_TIME: sqlQuery << wxT("Time"); break;
    case MAC_USAGE_BOARDNAME: sqlQuery << wxT("BoardName"); break;
    case MAC_USAGE_OLDMAC: sqlQuery << wxT("OldMACAddress"); break;
    case MAC_USAGE_NEWMAC: sqlQuery << wxT("NewMACAddress"); break;
    case MAC_USAGE_OPERATOR: sqlQuery << wxT("Operator"); break;
    }
    sqlQuery << wxT(" FROM ReportTable WHERE Id = ") << row + 1;

    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        variant = set.GetAsString(0);
    set.Finalize();
}

bool ReportDataModel::SetValueByRow(const wxVariant &WXUNUSED(variant),
                                    unsigned int WXUNUSED(row),
                                    unsigned int WXUNUSED(col))
{
    return false;
}

wxSQLite3Database *ReportDataModel::GetDB()
{
    return _db;
}

