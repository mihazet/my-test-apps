// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "SimCL200App.h"
#include "SimCL200Frame.h"

IMPLEMENT_APP(SimCL200App)
IMPLEMENT_CLASS(SimCL200App, wxApp)

BEGIN_EVENT_TABLE(SimCL200App, wxApp)
END_EVENT_TABLE()

SimCL200App::SimCL200App()
{
    Init();
}

void SimCL200App::Init()
{
}

bool SimCL200App::OnInit()
{    
	SimCL200Frame *mainWindow = new SimCL200Frame(NULL);
	mainWindow->Show();

    return true;
}

int SimCL200App::OnExit()
{    
	return wxApp::OnExit();
}

