#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
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
    wxBoxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxPropertyGrid *pg = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition,
        wxSize(400, 400), wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    pg->Append(new wxStringProperty("String Property", wxPG_LABEL));
    pg->Append(new wxIntProperty("Int Property", wxPG_LABEL));
    pg->Append(new wxBoolProperty("Bool Property", wxPG_LABEL));
    allSizer->Add(pg, 1, wxALL | wxEXPAND, 5);
    SetSizer(allSizer);
}

