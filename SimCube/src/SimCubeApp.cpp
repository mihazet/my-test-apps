#include <wx/wx.h>
#include "SimCubeApp.h"
#include "SimCubeFrame.h"

IMPLEMENT_APP(SimCubeApp)

SimCubeApp::SimCubeApp()
{
    Init();
}

void SimCubeApp::Init()
{
    if (NULL != (_mainDB = new wxSQLite3Database))
        _mainDB->Open(wxT("SimCube.db"));
    if (NULL != (_memDB = new wxSQLite3Database))
        _memDB->Open(wxT(":memory:"));
}

bool SimCubeApp::OnInit()
{
    SimCubeFrame *frame = new SimCubeFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int SimCubeApp::OnExit()
{
    if (_mainDB)
        _mainDB->Close();
    if (_memDB)
        _memDB->Close();

    return wxApp::OnExit();
}
