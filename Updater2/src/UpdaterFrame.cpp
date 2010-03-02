// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/iconbndl.h>
#include "UpdaterFrame.h"
#include "UpdaterApp.h"
#include "LogPane.h"
#include "DownloadPane.h"
#include "WidgetId.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/update_16.xpm"
#include "img/update_32.xpm"
#include "img/update_48.xpm"

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(UpdaterFrame, wxFrame)
    EVT_ERASE_BACKGROUND(UpdaterFrame::OnEraseBackground)
    EVT_SIZE(UpdaterFrame::OnSize)
    EVT_CLOSE(UpdaterFrame::OnClose)
    EVT_MENU(wxID_ABOUT, UpdaterFrame::OnAbout)
    EVT_MENU(wxID_EXIT, UpdaterFrame::OnQuit)
END_EVENT_TABLE()

UpdaterFrame::UpdaterFrame()
{
    Init();
}

UpdaterFrame::UpdaterFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool UpdaterFrame::Create(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    int x, y, w, h;

    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();

    /* update frame size and position */
    RetrieveFrameSizeAndPosition(&x, &y, &w, &h);
    SetSize(x, y, w, h);

    return true;
}

UpdaterFrame::~UpdaterFrame()
{
    _auiManager.UnInit();
}

void UpdaterFrame::Init()
{
}

void UpdaterFrame::CreateControls()
{
    /* tell wxAuiManager to manage this frame */
    _auiManager.SetManagedWindow(this);

    /* setup icon for window title bar, taskbar and task switching bar */
    wxIconBundle icons;
    icons.AddIcon(wxIcon(update_16_xpm));
    icons.AddIcon(wxIcon(update_32_xpm));
    icons.AddIcon(wxIcon(update_48_xpm));
    SetIcons(icons);

    /* menu bar */
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("&Quit"), _("Quit this program"));
    wxMenu *view_menu = new wxMenu;
    view_menu->AppendCheckItem(myID_VIEW_MAC_SETUP_PANE, _("MAC Setup\tCTRL+F1"),
        _("Show or hide the MAC address setup pane."));
    view_menu->AppendCheckItem(myID_VIEW_MAC_USAGE_PANE, _("MAC Usage\tCTRL+F2"),
        _("Show or hide the MAC usage pane."));
    view_menu->AppendCheckItem(myID_VIEW_MAC_UPDATE_PANE, _("MAC Update\tCTRL+F3"),
        _("Show or hide the MAC update pane."));
    view_menu->AppendCheckItem(myID_VIEW_LOG_PANE, _("Log Window\tCTRL+F4"),
        _("Show or hide the log window."));
    view_menu->AppendCheckItem(myID_VIEW_OPTION_PANE, _("Preference\tCTRL+F5"),
        _("Show or hide the preference."));
    view_menu->AppendSeparator();
    view_menu->Append(myID_VIEW_RESET_LAYOUT, _("Reset layout"),
        _("Reset all panes to the default position and size."));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(myID_HELP_DOC, _("Help file"), _("Show html help file."));
    help_menu->Append(wxID_ABOUT, _("About..."), _("Show about dialog."));
    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(file_menu, _("&File"));
    menubar->Append(view_menu, _("&View"));
    menubar->Append(help_menu, _("&Help"));
    SetMenuBar(menubar);

    /* tool bar */

    /* panes */
    LogPane *dbgWin = new LogPane(this);
    wxLog *logger = new wxLogTextCtrl(dbgWin->GetLogTextCtrl());
    delete wxLog::SetActiveTarget(logger);
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S]"));
    _auiManager.AddPane(dbgWin, wxAuiPaneInfo().Name(wxT("LogPane")).
        Caption(_("Log Window")).Bottom().CloseButton(true).
        DestroyOnClose(false).MaximizeButton(true).MinSize(-1, 150));

    _auiManager.AddPane(new DownloadPane(this), wxAuiPaneInfo().
        Name(wxT("DownloadPane")).Caption(_("Download Window")).Center().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(300, -1));

    /* status bar */

    _auiManager.Update();
}

void UpdaterFrame::RetrieveFrameSizeAndPosition(int *x, int *y, int *w, int *h)
{
    int _x = -1, _y = -1, _w = -1, _h = -1;

    if ((_x == -1) || (_y == -1) || (_w == -1) || (_h == -1))
    {
        wxSize screen = wxGetDisplaySize();
        float hRatio = 0.85f, vRatio = 0.85f;

        if (screen.x <= 1024)
            hRatio = 0.95f;
        if (screen.y <= 768)
            vRatio = 0.90f;

        _w = (int)(screen.x * hRatio);
        _h = (int)(screen.y * vRatio);
        _x = (int)((screen.x - _w) * 0.5);
        _y = (int)((screen.y - _h) * 0.5);
    }

    *x = _x;
    *y = _y;
    *w = _w;
    *h = _h;
}

//////////////////////////////////////////////////////////////////////////////
// Event handlers
//////////////////////////////////////////////////////////////////////////////
void UpdaterFrame::OnViewPane(wxCommandEvent &event)
{

}

void UpdaterFrame::OnUpdatePane(wxUpdateUIEvent &event)
{
}

void UpdaterFrame::OnResetLayout(wxCommandEvent &WXUNUSED(event))
{
}

void UpdaterFrame::OnEraseBackground(wxEraseEvent &event)
{
    event.Skip();
}

void UpdaterFrame::OnSize(wxSizeEvent &event)
{
    //event.Skip();
}

void UpdaterFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    Destroy();
}

void UpdaterFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
}

void UpdaterFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

