#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "SimCubeApp.h"
#include "SimCubeFrame.h"
#include "Rockey4_ND_32.h"

IMPLEMENT_APP(SimCubeApp)

BEGIN_EVENT_TABLE(SimCubeApp, wxApp)
#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    EVT_IDLE(SimCubeApp::OnAppIdle)
#endif
END_EVENT_TABLE()

SimCubeApp::SimCubeApp()
{
    Init();
}

void SimCubeApp::Init()
{
    wxStandardPaths &stdPaths = wxStandardPaths::Get();
    wxString dbName;

    /* database initialization */
    dbName = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
    dbName += wxT("SimCube.db");
    if (NULL != (_mainDB = new wxSQLite3Database))
        _mainDB->Open(dbName);
    if (NULL != (_memDB = new wxSQLite3Database))
        _memDB->Open(wxT(":memory:"));
}

bool SimCubeApp::OnInit()
{
#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    /* check for USB dongle */
    if (!CheckRockey())
    {
        wxLogError(_("Please insert USB dongle and restart application!"));
        return false;
    }
#endif

    /* init the main frame */
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

#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
void SimCubeApp::OnAppIdle(wxIdleEvent &WXUNUSED(event))
{
    if (!CheckRockey())
    {
        wxMessageBox(_("USB dongle is removed, application will be terminated now!"), _("Error"));
        ExitMainLoop();
    }
}

bool SimCubeApp::CheckRockey()
{
    WORD handle[16], p1, p2, p3, p4, retcode;
    DWORD lp1, lp2;
    BYTE buffer[1024];

    p1 = 0xC44C;
    p2 = 0xC8F8;
    p3 = 0x0799;
    p4 = 0xC43B;

    retcode = Rockey(RY_FIND, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
    if (retcode)
        return false;

    return true;
}
#endif

