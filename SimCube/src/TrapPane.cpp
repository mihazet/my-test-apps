#include <wx/wx.h>
#include "TrapPane.h"

TrapPane::TrapPane()
{
    Init();
}

TrapPane::TrapPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

TrapPane::~TrapPane()
{

}

bool TrapPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void TrapPane::Init()
{
}

void TrapPane::CreateControls()
{
}


