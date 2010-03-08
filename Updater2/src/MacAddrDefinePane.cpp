// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "MacAddrDefinePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
MacAddrDefinePane::MacAddrDefinePane()
{
    Init();
}

MacAddrDefinePane::MacAddrDefinePane(wxWindow *parent, wxWindowID id,
                                     const wxPoint &pos, const wxSize &size,
                                     long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

MacAddrDefinePane::~MacAddrDefinePane()
{

}

void MacAddrDefinePane::Init()
{

}

bool MacAddrDefinePane::Create(wxWindow *parent, wxWindowID id,
                               const wxPoint &pos, const wxSize &size,
                               long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void MacAddrDefinePane::CreateControls()
{

}

// event handlers
