#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "SimCubeApp.h"
#include "SimCubeFrame.h"

IMPLEMENT_APP(SimCubeApp)

SimCubeApp::SimCubeApp()
{
    Init();
}

void SimCubeApp::Init()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString dbName;

    dbName = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
    dbName += wxT("SimCube.db");
    if (NULL != (_mainDB = new wxSQLite3Database))
        _mainDB->Open(dbName);
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
