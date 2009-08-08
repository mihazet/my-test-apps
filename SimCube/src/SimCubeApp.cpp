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
    return wxApp::OnExit();
}

