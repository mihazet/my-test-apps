/*
 *  PWUpdater.c - PixelWorks firmware update application for Ruby platform.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/socket.h>
#include "PWUpdater.h"
#include "TftpServerThread.h"
#include "TftpTransmissionThread.h"
#include "DownloadPane.h"
#include "LogPane.h"

#define wxLOG_COMPONENT "PWUpdater/ui/frame"

// ------------------------------------------------------------------------
// Application implementation
// ------------------------------------------------------------------------
IMPLEMENT_APP(PWUpdaterApp)

void PWUpdaterApp::Init()
{
    m_serverCS.Enter();
    m_pTftpServerThread = NULL;
    m_serverCS.Leave();

    m_transmissionCS.Enter();
    m_tftpTransmissionThreads.clear();
    m_transmissionCS.Leave();
}

bool PWUpdaterApp::OnInit()
{
    /* create main frame */
    PWUpdaterFrame *frame = new PWUpdaterFrame(NULL);
    frame->Show();
    return true;
}

// ------------------------------------------------------------------------
// Main frame implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(PWUpdaterFrame, wxFrame)
    EVT_CLOSE(PWUpdaterFrame::OnClose)
    EVT_MENU(wxID_EXIT, PWUpdaterFrame::OnQuit)
END_EVENT_TABLE()

PWUpdaterFrame::PWUpdaterFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos, const wxSize &size,
    long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

PWUpdaterFrame::~PWUpdaterFrame()
{
    _auiMgr.UnInit();
}

bool PWUpdaterFrame::Create(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos, const wxSize &size,
    long style)
{
    bool result;

    result = wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();

    return result;
}

void PWUpdaterFrame::Init()
{
}

void PWUpdaterFrame::CreateControls()
{
    /* menu bar */
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("&Quit"), _("Quit this program."));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(file_menu, _("&File"));
    SetMenuBar(menuBar);

    /* tool bar */

    /* status bar */

    /* aui manager & panes */
    _auiMgr.SetManagedWindow(this);
    _auiMgr.SetFlags(_auiMgr.GetFlags() | wxAUI_MGR_LIVE_RESIZE);

    _auiMgr.AddPane(new LogPane(this),
        wxAuiPaneInfo().Name(wxT("LogPane")).Caption(_("Log Window")).
        CloseButton(true).DestroyOnClose(false).MinSize(560, 420).
        Float());
    _auiMgr.AddPane(new DownloadPane(this),
        wxAuiPaneInfo().Name(wxT("DownloadPane")).CaptionVisible(false).
        Center().CloseButton(false).DestroyOnClose(false).
        MaximizeButton(true));

    _auiMgr.Update();
}

void PWUpdaterFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpServerThread *&pServer = wxGetApp().m_pTftpServerThread;
    wxCriticalSection &cs2 = wxGetApp().m_transmissionCS;
    wxVector<TftpTransmissionThread *> &transmissions
        = wxGetApp().m_tftpTransmissionThreads;
    wxVector<TftpTransmissionThread *>::iterator it;
    bool serverTerminated = false;
    bool allTransmissionTerminated = false;

    /* delete tftp server thread if it is still running... */
    cs.Enter();
    if (pServer)
        pServer->Delete();
    cs.Leave();

    /* delete tftp transmission threads if any are still running... */
    cs2.Enter();
    for (it = transmissions.begin(); it != transmissions.end(); ++it)
    {
        if (*it)
            (*it)->Delete();
    }
    cs2.Leave();

    /* make sure tftp server and transmissions terminated. */
    while (true)
    {
        if (!serverTerminated)
        {
            cs.Enter();
            if (!pServer)
                serverTerminated = true;
            cs.Leave();
        }

        if (!allTransmissionTerminated)
        {
            allTransmissionTerminated = true;
            cs2.Enter();
            for (it = transmissions.begin(); it != transmissions.end(); ++it)
            {
                if (*it != NULL)
                {
                    allTransmissionTerminated = false;
                    break;
                }
            }
            cs2.Leave();
        }

        if (serverTerminated && allTransmissionTerminated)
            break;

        /* give the tftp server a chance to terminated. */
        wxMilliSleep(100);
    }

    Destroy();
}

void PWUpdaterFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

