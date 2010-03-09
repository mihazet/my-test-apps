// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "AppPreferencePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
AppPreferencePane::AppPreferencePane()
{
    Init();
}

AppPreferencePane::AppPreferencePane(wxWindow *parent, wxWindowID id,
                                     const wxPoint &pos, const wxSize &size,
                                     long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

AppPreferencePane::~AppPreferencePane()
{

}

void AppPreferencePane::Init()
{

}

bool AppPreferencePane::Create(wxWindow *parent, wxWindowID id,
                               const wxPoint &pos, const wxSize &size,
                               long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void AppPreferencePane::CreateControls()
{

}

// event handlers
