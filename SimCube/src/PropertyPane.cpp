#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include <wx/wxsqlite3.h>
#include "SimCubeApp.h"
#include "PropertyPane.h"

enum
{
    myID_PROPERTY_GRID = wxID_HIGHEST + 1,
    myID_RESET_BTN,
};

BEGIN_EVENT_TABLE(PropertyPane, wxPanel)
    EVT_PG_CHANGING(myID_PROPERTY_GRID, PropertyPane::OnPropertyChanging)
END_EVENT_TABLE()

PropertyPane::PropertyPane()
{
    Init();
}

PropertyPane::PropertyPane(wxWindow *parent, wxWindowID id,
    const wxPoint &pos, const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

PropertyPane::~PropertyPane()
{

}

bool PropertyPane::Create(wxWindow *parent, wxWindowID id,
    const wxPoint &pos, const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void PropertyPane::Init()
{
    _db = wxGetApp().GetMainDatabase();
    _db->SetUpdateHook(this);
}

void PropertyPane::CreateControls()
{
    wxString name, type, format, value;
    wxBoxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    _pg = new wxPropertyGrid(this, myID_PROPERTY_GRID,
        wxDefaultPosition, wxSize(250, 400),
        wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED|wxPG_TOOLTIPS);
    if (_db && _db->IsOpen())
    {
        wxSQLite3ResultSet set = _db->ExecuteQuery(wxT("SELECT * FROM PropTbl"));
        while (set.NextRow())
        {
            name = set.GetAsString(wxT("DisplayedName"));
            type = set.GetAsString(wxT("PropertyType"));
            value = set.GetAsString(wxT("CurrentValue"));
            if (type == wxT("Numeric"))
            {
                long propVal;
                _pg->Append(new wxIntProperty(name, wxPG_LABEL,
                    value.ToLong(&propVal) ? propVal : 0));
            }
            else if (type == wxT("List"))
            {
                format = set.GetAsString(wxT("PropertyFormat"));
                wxArrayString aryStr;
                wxStringTokenizer tokenizer(format, wxT(";"));
                while (tokenizer.HasMoreTokens())
                    aryStr.Add(tokenizer.GetNextToken());
                wxEnumProperty *list = new wxEnumProperty(name, wxPG_LABEL, aryStr);
                list->SetValue(value);
                _pg->Append(list);
            }
            else if (type == wxT("String"))
            {
                //int length = set.GetInt(wxT("PropertyFormat"));
                wxStringProperty *str = new wxStringProperty(name, wxPG_LABEL, value);
                //str->SetMaxLength(length);
                _pg->Append(str);
            }
        }
        set.Finalize();
    }
    allSizer->Add(_pg, 1, wxALL | wxEXPAND, 5);

    wxButton *rst = new wxButton(this, myID_RESET_BTN, _("Reset To Default"));
    wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(rst, 0, wxALL, 5);
    allSizer->Add(btnSizer);

    SetSizer(allSizer);
}

void PropertyPane::UpdateCallback(wxUpdateType type, const wxString &database,
                                  const wxString &table, wxLongLong rowid)
{
    if ((type == SQLITE_UPDATE) && (database == wxT("main"))
        && (table == wxT("PropTbl")))
    {
        wxString sqlQuery, name, value;
        wxSQLite3ResultSet set;

        /* check which property to be updated to what value */
        sqlQuery << wxT("SELECT DisplayedName, CurrentValue FROM PropTbl LIMIT 1 OFFSET ")
            << (rowid - 1).ToString();
        set = _db->ExecuteQuery(sqlQuery);
        if (set.NextRow())
        {
            name = set.GetAsString(0);
            value = set.GetAsString(1);
        }
        set.Finalize();

        /* update the property grid ui to reflect database update */
        wxPGProperty *prop = _pg->GetProperty(name);
        prop->SetValueFromString(value);
    }
}

void PropertyPane::OnPropertyChanging(wxPropertyGridEvent &event)
{
    wxPGProperty *prop = event.GetProperty();
    wxVariant value = event.GetValue();
    wxString sqlQuery, sqlUpdate, type, format, minStr, maxStr;
    long min, max, requestedValue;

    // event.GetValue() is always numeric base wxVariant.
    // Use wxPGProperty::ValueToString(wxVariant& value, int argFlags = 0) to
    // retrieve the string based property value for any kind of wxYYYProperty.
    if (prop && _db)
    {
        wxLogVerbose(wxT("OnPropertyChanging %s"), prop->ValueToString(value));
        sqlQuery << wxT("SELECT PropertyType, PropertyFormat FROM PropTbl WHERE DisplayedName = '")
            << prop->GetLabel()
            << wxT("'");
        wxSQLite3ResultSet set = _db->ExecuteQuery(sqlQuery);
        if (set.NextRow())
        {
            type = set.GetAsString(wxT("PropertyType"));
            format = set.GetAsString(wxT("PropertyFormat"));
        }
        set.Finalize();

        /* property format checker */
        if (type == wxT("Numeric"))
        {
            wxStringTokenizer tokenizer(format, wxT(";"));
            minStr = tokenizer.GetNextToken();
            maxStr = tokenizer.GetNextToken();
            if (minStr.ToLong(&min) && maxStr.ToLong(&max))
            {
                requestedValue = value.GetLong();
                if ((requestedValue < min) || (requestedValue > max))
                {
                    event.Veto();
                    wxLogError(_("Input number out of range! Max = %s, Min = %s"),
                        maxStr, minStr);
                    return;
                }
            }
        }
        else if (type == wxT("List"))
        {
            /* no check item now. */
        }
        else if (type == wxT("String"))
        {
            // TODO: check for max length
        }

        /* update database record */
        sqlUpdate << wxT("UPDATE PropTbl SET CurrentValue = '")
            << prop->ValueToString(value)
            << wxT("' WHERE DisplayedName = '")
            << prop->GetLabel()
            << wxT("'");
        if (_db->ExecuteUpdate(sqlUpdate) != 1)
        {
            wxLogError(_("Failed to update property %s"), prop->GetLabel());
            event.Veto();
        }
    }
}

