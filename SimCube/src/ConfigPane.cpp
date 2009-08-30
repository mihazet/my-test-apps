#include <wx/wx.h>
#include "SimCubeApp.h"
#include "ConfigPane.h"

ConfigPane::ConfigPane()
{
    Init();
}

ConfigPane::ConfigPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

ConfigPane::~ConfigPane()
{

}

bool ConfigPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void ConfigPane::Init()
{
}

void ConfigPane::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);

    SetSizerAndFit(allSizer);
}

