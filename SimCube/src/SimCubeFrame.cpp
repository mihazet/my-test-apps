#include <wx/wx.h>
#include "SimCubeFrame.h"

BEGIN_EVENT_TABLE(SimCubeFrame, wxFrame)
END_EVENT_TABLE()

SimCubeFrame::SimCubeFrame()
{
    Init();
}

SimCubeFrame::SimCubeFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool SimCubeFrame::Create(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();
    return true;
}

SimCubeFrame::~SimCubeFrame()
{
    _auiManager.UnInit();
}

void SimCubeFrame::Init()
{

}

void SimCubeFrame::CreateControls()
{
    _auiManager.SetManagedWindow(this);
    
    wxStatusBar *statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP|wxNO_BORDER);
    statusBar->SetFieldsCount(3);
    SetStatusBar(statusBar);
    
    _auiManager.Update();
}



