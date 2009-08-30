#include <wx/wx.h>
#include "DebugPane.h"

DebugPane::DebugPane()
{
    Init();
}

DebugPane::DebugPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

DebugPane::~DebugPane()
{

}

bool DebugPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void DebugPane::Init()
{
    _logTextCtrl = NULL;
}

void DebugPane::CreateControls()
{
    wxBoxSizer *allSizer = new wxBoxSizer(wxHORIZONTAL);
    _logTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2);
    allSizer->Add(_logTextCtrl, 1, wxALL|wxEXPAND, 5);
    SetSizer(allSizer);
}

