// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "MacAddrUpdatePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
MacAddrUpdatePane::MacAddrUpdatePane()
{
    Init();
}

MacAddrUpdatePane::MacAddrUpdatePane(wxWindow *parent, wxWindowID id,
                                     const wxPoint &pos, const wxSize &size,
                                     long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

MacAddrUpdatePane::~MacAddrUpdatePane()
{

}

void MacAddrUpdatePane::Init()
{

}

bool MacAddrUpdatePane::Create(wxWindow *parent, wxWindowID id,
                               const wxPoint &pos, const wxSize &size,
                               long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void MacAddrUpdatePane::CreateControls()
{

}

// event handlers
