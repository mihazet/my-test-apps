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
    EVT_PG_SELECTED(myID_PROPERTY_GRID, PropertyPane::OnPropertySelected)
    EVT_PG_CHANGING(myID_PROPERTY_GRID, PropertyPane::OnPropertyChanging)
    EVT_PG_HIGHLIGHTED(myID_PROPERTY_GRID, PropertyPane::OnPropertyHighlighted)
    EVT_PG_RIGHT_CLICK(myID_PROPERTY_GRID, PropertyPane::OnPropertyRightClick)
    EVT_PG_DOUBLE_CLICK(myID_PROPERTY_GRID, PropertyPane::OnPropertyDoubleClick)
    EVT_PG_ITEM_COLLAPSED(myID_PROPERTY_GRID, PropertyPane::OnPropertyItemCollapsed)
    EVT_PG_ITEM_EXPANDED(myID_PROPERTY_GRID, PropertyPane::OnPropertyItemExpanded)
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

}

void PropertyPane::CreateControls()
{
    wxString name, type, format, value;
    wxBoxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxPropertyGrid *pg = new wxPropertyGrid(this, myID_PROPERTY_GRID,
        wxDefaultPosition, wxSize(250, 400),
        wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    if (db->IsOpen())
    {
        wxSQLite3ResultSet set = db->ExecuteQuery(wxT("SELECT * FROM PropTbl"));
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
    wxLogMessage(wxT("OnPropertySelected"));
    event.Skip();
}

void PropertyPane::OnPropertyChanging(wxPropertyGridEvent &event)
{
    wxLogMessage(wxT("OnPropertyChanging"));
    event.Skip();
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

