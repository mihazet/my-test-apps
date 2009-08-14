#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include <wx/wxsqlite3.h>
#include "SimCubeApp.h"
#include "PropertyPane.h"

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
    wxPropertyGrid *pg = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition,
        wxSize(400, 400), wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    if (db->IsOpen())
    {
        wxSQLite3ResultSet set = db->ExecuteQuery(wxT("SELECT * FROM property"));
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

