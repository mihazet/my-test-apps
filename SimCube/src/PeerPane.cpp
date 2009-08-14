#include <wx/wx.h>
#include "PeerPane.h"

PeerPane::PeerPane()
{
    Init();
}

PeerPane::PeerPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

PeerPane::~PeerPane()
{

}

bool PeerPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void PeerPane::Init()
{
}

void PeerPane::CreateControls()
{
}

