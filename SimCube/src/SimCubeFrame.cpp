// wx headers
#include <wx/wx.h>
#include <wx/iconbndl.h>
// our headers
#include "ConfigPane.h"
#include "DebugPane.h"
#include "HistoryPane.h"
#include "PeerPane.h"
#include "PropertyPane.h"
#include "SimCubeFrame.h"
// resource headers
#include "img/SimCube-32.xpm"
#include "img/SimCube-48.xpm"
#include "img/SimCube-64.xpm"

enum
{
    ID_MODE_NORMAL = wxID_HIGHEST + 1,
    ID_MODE_SERVICE,
};

BEGIN_EVENT_TABLE(SimCubeFrame, wxFrame)
    EVT_ERASE_BACKGROUND(SimCubeFrame::OnEraseBackground)
    EVT_SIZE(SimCubeFrame::OnSize)
    EVT_CLOSE(SimCubeFrame::OnClose)
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
    SetSize(1000, 650);
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
    /* tell wxAuiManager to manage this frame */
    _auiManager.SetManagedWindow(this);

    /* setup icon for window title bar, taskbar and task switching bar */
    wxIconBundle icons;
    icons.AddIcon(wxIcon(SimCube_32_xpm));
    icons.AddIcon(wxIcon(SimCube_48_xpm));
    icons.AddIcon(wxIcon(SimCube_64_xpm));
    SetIcons(icons);

    /* menu bar */
    wxMenuBar *mb = new wxMenuBar;
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    wxMenu *opt_menu = new wxMenu;
    opt_menu->AppendRadioItem(ID_MODE_NORMAL, _("Normal Mode"));
    opt_menu->AppendRadioItem(ID_MODE_SERVICE, _("Service Mode"));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    mb->Append(file_menu, _("&File"));
    mb->Append(opt_menu, _("&Option"));
    mb->Append(help_menu, _("&Help"));
    SetMenuBar(mb);

    /* tool bar */

    /* panes */
    DebugPane *dbgWin = new DebugPane(this);
    wxLog *logger = new wxLogTextCtrl(dbgWin->GetLogTextCtrl());
    delete wxLog::SetActiveTarget(logger);
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S]"));
    _auiManager.AddPane(dbgWin, wxAuiPaneInfo().Name(wxT("DebugPane")).
        Caption(_("Log Output")).Bottom(). CloseButton(true).
        DestroyOnClose(false).MaximizeButton(true).MinSize(-1, 150).
        PaneBorder(false));

    _auiManager.AddPane(new PropertyPane(this), wxAuiPaneInfo().
        Name(wxT("PropertyPane")).Caption(_("Supported Property")).Center().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        PaneBorder(false));

    _auiManager.AddPane(new HistoryPane(this), wxAuiPaneInfo().
        Name(wxT("HistoryPane")).Caption(_("Transaction History")).Right().
        CloseButton(true).DestroyOnClose(false).MinSize(200, -1).
        PaneBorder(false));

    _auiManager.AddPane(new ConfigPane(this), wxAuiPaneInfo().
        Name(wxT("ConfigPane")).Caption(_("SimCube Configuration")).Right().
        CloseButton(true).DestroyOnClose(false).MinSize(50, 50).
        PaneBorder(false));

    _auiManager.AddPane(new PeerPane(this), wxAuiPaneInfo().
        Name(wxT("PeerPane")).Caption(_("Send Trap Message")).Right().
        CloseButton(true).DestroyOnClose(false).MinSize(100, 100).
        PaneBorder(false));

    /* status bar */
    SetStatusBar(CreateStatusBar(3));

    /* commit all change */
    _auiManager.Update();
}

void SimCubeFrame::OnEraseBackground(wxEraseEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnSize(wxSizeEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

