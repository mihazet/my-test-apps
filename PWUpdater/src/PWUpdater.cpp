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
#include <wx/socket.h>
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "LogPane.h"

#define wxLOG_COMPONENT "PWUpdater/ui/frame"

// ------------------------------------------------------------------------
// Application implementation
// ------------------------------------------------------------------------
IMPLEMENT_APP(PWUpdaterApp)

void PWUpdaterApp::Init()
{
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
    EVT_MENU(wxID_EXIT, PWUpdaterFrame::OnQuit)
END_EVENT_TABLE()

PWUpdaterFrame::PWUpdaterFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos, const wxSize &size,
    long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
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
    file_menu->Append(wxID_EXIT, _("&Quuit"), _("Quit this program."));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(file_menu, _("&File"));
    SetMenuBar(menuBar);

    /* tool bar */

    /* status bar */

    /* aui */
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

void PWUpdaterFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

