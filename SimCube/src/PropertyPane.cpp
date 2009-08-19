#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include <wx/wxsqlite3.h>
#include "SimCubeApp.h"
#include "PropertyPane.h"

enum
{
    myID_PROPERTY_GRID = wxID_HIGHEST + 1,
};

BEGIN_EVENT_TABLE(PropertyPane, wxPanel)
    //EVT_PG_SELECTED(myID_PROPERTY_GRID, PropertyPane::OnPropertySelected)
    EVT_PG_CHANGING(myID_PROPERTY_GRID, PropertyPane::OnPropertyChanging)
    //EVT_PG_HIGHLIGHTED(myID_PROPERTY_GRID, PropertyPane::OnPropertyHighlighted)
    //EVT_PG_RIGHT_CLICK(myID_PROPERTY_GRID, PropertyPane::OnPropertyRightClick)
    //EVT_PG_DOUBLE_CLICK(myID_PROPERTY_GRID, PropertyPane::OnPropertyDoubleClick)
    //EVT_PG_ITEM_COLLAPSED(myID_PROPERTY_GRID, PropertyPane::OnPropertyItemCollapsed)
    //EVT_PG_ITEM_EXPANDED(myID_PROPERTY_GRID, PropertyPane::OnPropertyItemExpanded)
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
}

void PropertyPane::CreateControls()
{
    wxString name, type, format, value;
    wxBoxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxPropertyGrid *pg = new wxPropertyGrid(this, myID_PROPERTY_GRID,
        wxDefaultPosition, wxSize(250, 400),
        wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED|wxPG_TOOLTIPS);
    if (_db && _db->IsOpen())
    {
        wxSQLite3ResultSet set = _db->ExecuteQuery(wxT("SELECT * FROM PropTbl"));
        while (set.NextRow())
        {
            name = set.GetAsString(wxT("DisplayedName"));
            type = set.GetAsString(wxT("PropertyType"));
            value = set.GetAsString(wxT("PropertyValue"));
            if (type == wxT("Numeric"))
            {
                long propVal;
                pg->Append(new wxIntProperty(name, wxPG_LABEL,
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
                pg->Append(list);
            }
        }
        set.Finalize();
    }
    allSizer->Add(pg, 1, wxALL | wxEXPAND, 5);
    SetSizer(allSizer);
}

void PropertyPane::OnPropertySelected(wxPropertyGridEvent &event)
{
    wxPGProperty *prop = event.GetProperty();

    if (prop) // when TLW close, PG will send wxEVT_PG_SELECTED with NULL prop.
    {
        wxLogMessage(wxT("OnPropertySelected %s"), prop->GetLabel());
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

        /* update database record */
        sqlUpdate << wxT("UPDATE PropTbl SET PropertyValue = '")
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

void PropertyPane::OnPropertyHighlighted(wxPropertyGridEvent &event)
{
    wxLogVerbose(wxT("OnPropertyHighlighted"));
    event.Skip();
}

void PropertyPane::OnPropertyRightClick(wxPropertyGridEvent &event)
{
    wxLogMessage(wxT("OnPropertyRightClick"));
    event.Skip();
}

void PropertyPane::OnPropertyDoubleClick(wxPropertyGridEvent &event)
{
    wxLogMessage(wxT("OnPropertyDoubleClick"));
    event.Skip();
}

void PropertyPane::OnPropertyItemCollapsed(wxPropertyGridEvent &event)
{
    wxLogMessage(wxT("OnPropertyItemCollapsed"));
    event.Skip();
}

void PropertyPane::OnPropertyItemExpanded(wxPropertyGridEvent &event)
{
    wxLogMessage(wxT("OnPropertyItemExpanded"));
    event.Skip();
}

