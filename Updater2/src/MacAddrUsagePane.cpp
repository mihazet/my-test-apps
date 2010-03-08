// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "MacAddrUsagePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
MacAddrUsagePane::MacAddrUsagePane()
{
    Init();
}

MacAddrUsagePane::MacAddrUsagePane(wxWindow *parent, wxWindowID id,
                                   const wxPoint &pos, const wxSize &size,
                                   long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

MacAddrUsagePane::~MacAddrUsagePane()
{

}

void MacAddrUsagePane::Init()
{

}

bool MacAddrUsagePane::Create(wxWindow *parent, wxWindowID id,
                              const wxPoint &pos, const wxSize &size,
                              long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void MacAddrUsagePane::CreateControls()
{

}

// event handlers
