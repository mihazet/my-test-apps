// wx headers
#include <wx/wx.h>
#include <wx/iconbndl.h>
#include <wx/wxsqlite3.h>
// our headers
#include "ConfigPane.h"
#include "DebugPane.h"
#include "HistoryPane.h"
#include "PeerPane.h"
#include "PropertyPane.h"
#include "SimCubeFrame.h"
#include "TrapPane.h"
#include "SimCubeApp.h"
// resource headers
#include "img/SimCube-32.xpm"
#include "img/SimCube-48.xpm"
#include "img/SimCube-64.xpm"

enum
{
    ID_VIEW_PANE_START = wxID_HIGHEST + 1,
    ID_VIEW_PROPERTY_PANE = ID_VIEW_PANE_START,
    ID_VIEW_TRAP_PANE,
    ID_VIEW_HISTORY_PANE,
    ID_VIEW_PEER_PANE,
    ID_VIEW_CONFIG_PANE,
    ID_VIEW_DOWNLOAD_PANE,
    ID_VIEW_LOG_PANE,
    ID_VIEW_PANE_END = ID_VIEW_LOG_PANE,
    ID_VIEW_RESET_LAYOUT,
    ID_HELP_DOC,
};

BEGIN_EVENT_TABLE(SimCubeFrame, wxFrame)
    EVT_MENU_RANGE(ID_VIEW_PANE_START, ID_VIEW_PANE_END, SimCubeFrame::OnViewPane)
    EVT_UPDATE_UI_RANGE(ID_VIEW_PANE_START, ID_VIEW_PANE_END, SimCubeFrame::OnUpdatePane)
    EVT_MENU(ID_VIEW_RESET_LAYOUT, SimCubeFrame::OnResetLayout)
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
    int x, y, w, h;
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();

    /* update frame size and position */
    RetrieveFrameSizeAndPosition(&x, &y, &w, &h);
    SetSize(x, y, w, h);
    return true;
}

SimCubeFrame::~SimCubeFrame()
{
    wxString sql, perspective;
    wxSQLite3ResultSet set;

    perspective = _auiManager.SavePerspective();
    sql << wxT("UPDATE CfgTbl SET ConfigValue = '")
        << perspective
        << wxT("' WHERE ConfigName = 'Perspective'");
    if (1 != _db->ExecuteUpdate(sql))
        wxLogError(_("Fail to save perspective!"));

    _auiManager.UnInit();
}

void SimCubeFrame::Init()
{
    _db = wxGetApp().GetMainDatabase();
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
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    wxMenu *view_menu = new wxMenu;
    view_menu->AppendCheckItem(ID_VIEW_PROPERTY_PANE, _("Property Pane\tCTRL+F1"),
        _("Show or hide the property pane."));
    view_menu->AppendCheckItem(ID_VIEW_TRAP_PANE, _("Trap Pane\tCTRL+F2"),
        _("Show or hid the trap pane."));
    view_menu->AppendCheckItem(ID_VIEW_HISTORY_PANE, _("History Pane\tCTRL+F3"),
        _("Show or hide the history pane."));
    view_menu->AppendCheckItem(ID_VIEW_PEER_PANE, _("Remote Pane\tCTRL+F4"),
        _("Show or hide the remote peer pane."));
    view_menu->AppendCheckItem(ID_VIEW_CONFIG_PANE, _("Configuration Pane\tCTRL+F5"),
        _("Show or hide the configuration pane."));
    view_menu->AppendCheckItem(ID_VIEW_DOWNLOAD_PANE, _("Download Pane\tCTRL+F6"),
        _("Show or hide the download pane."));
    view_menu->AppendCheckItem(ID_VIEW_LOG_PANE, _("Log Widnow\tCTRL+F7"),
        _("Show or hide the logging window."));
    view_menu->AppendSeparator();
    view_menu->Append(ID_VIEW_RESET_LAYOUT, _("Reset all panes"),
        _("Reset all panes to its default position and size."));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(ID_HELP_DOC, _("Help file"),
        _("Show help file."));
    help_menu->Append(wxID_ABOUT);

    wxMenuBar *mb = new wxMenuBar;
    mb->Append(file_menu, _("File"));
    mb->Append(view_menu, _("View"));
    mb->Append(help_menu, _("Help"));
    SetMenuBar(mb);

    /* tool bar */

    /* panes */
    DebugPane *dbgWin = new DebugPane(this);
    wxLog *logger = new wxLogTextCtrl(dbgWin->GetLogTextCtrl());
    delete wxLog::SetActiveTarget(logger);
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S]"));
    _auiManager.AddPane(dbgWin, wxAuiPaneInfo().Name(wxT("DebugPane")).
        Caption(_("Log Output")).Bottom().CloseButton(true).
        DestroyOnClose(false).MaximizeButton(true).MinSize(-1, 150).
        Hide());

    _auiManager.AddPane(new PropertyPane(this), wxAuiPaneInfo().
        Name(wxT("PropertyPane")).Caption(_("Supported Property")).Left().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(300, -1));

    _auiManager.AddPane(new HistoryPane(this), wxAuiPaneInfo().
        Name(wxT("HistoryPane")).Caption(_("Transaction History")).Center().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(500, -1));

    _auiManager.AddPane(new ConfigPane(this), wxAuiPaneInfo().
        Name(wxT("ConfigPane")).Caption(_("SimCube Configuration")).
        Dockable(false).Float().Hide());

    _auiManager.AddPane(new TrapPane(this), wxAuiPaneInfo().
        Name(wxT("TrapPane")).Caption(_("Send Trap Message")).
        Dockable(false).Float().Hide());

    _auiManager.AddPane(new PeerPane(this), wxAuiPaneInfo().
        Name(wxT("PeerPane")).Caption(_("Remote Status")).Bottom().
        CloseButton(true).DestroyOnClose(false).MinSize(200, 150));

    /* status bar */
    SimCubeStatusBar *sb = wxGetApp().m_StatusBar;
    sb->Create(this);
    SetStatusBar(sb);

    /* update default perspective (use layout code above) */
    wxString sql, perspective;
    wxSQLite3ResultSet set;

    sql << wxT("UPDATE CfgTbl SET ConfigValue = '")
        << _auiManager.SavePerspective()
        << wxT("' WHERE ConfigName = 'DefaultPerspective'");
    if (1 != _db->ExecuteUpdate(sql))
        wxLogError(_("Fail to save software default perspective!"));

    /* restore previous perspective or just use current perspective */
    sql.clear();
    sql << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Perspective'");
    set = _db->ExecuteQuery(sql);
    if (set.NextRow())
        perspective = set.GetAsString(0);
    set.Finalize();

    if (!perspective.empty())
        _auiManager.LoadPerspective(perspective);
    else
        _auiManager.Update();
}

void SimCubeFrame::RetrieveFrameSizeAndPosition(int *x, int *y, int *w, int *h)
{
    int _x = -1, _y = -1, _w = -1, _h = -1;
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    wxSQLite3ResultSet set;

    wxASSERT_MSG(db, wxT("db is null pointer"));
    wxASSERT_MSG(x, wxT("x is null pointer"));
    wxASSERT_MSG(y, wxT("y is null pointer"));
    wxASSERT_MSG(w, wxT("w is null pointer"));
    wxASSERT_MSG(h, wxT("h is null pointer"));

    if (db->IsOpen())
    {
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameX'"));
        if (set.NextRow())
            _x = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameY'"));
        if (set.NextRow())
            _y = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameW'"));
        if (set.NextRow())
            _w = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameH'"));
        if (set.NextRow())
            _h = set.GetInt(0);
        set.Finalize();
    }

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
void SimCubeFrame::OnViewPane(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case ID_VIEW_PROPERTY_PANE:
        _auiManager.GetPane(wxT("PropertyPane")).Show(event.IsChecked());
        break;
    case ID_VIEW_TRAP_PANE:
        _auiManager.GetPane(wxT("TrapPane")).Show(event.IsChecked());
        break;
    case ID_VIEW_HISTORY_PANE:
        _auiManager.GetPane(wxT("HistoryPane")).Show(event.IsChecked());
        break;
    case ID_VIEW_PEER_PANE:
        _auiManager.GetPane(wxT("PeerPane")).Show(event.IsChecked());
        break;
    case ID_VIEW_CONFIG_PANE:
        _auiManager.GetPane(wxT("ConfigPane")).Show(event.IsChecked());
        break;
    case ID_VIEW_DOWNLOAD_PANE:
        break;
    case ID_VIEW_LOG_PANE:
        _auiManager.GetPane(wxT("DebugPane")).Show(event.IsChecked());
        break;
    }
    _auiManager.Update();
}

void SimCubeFrame::OnUpdatePane(wxUpdateUIEvent &event)
{
    switch (event.GetId())
    {
    case ID_VIEW_PROPERTY_PANE:
        event.Check(_auiManager.GetPane(wxT("PropertyPane")).IsShown());
        break;
    case ID_VIEW_TRAP_PANE:
        event.Check(_auiManager.GetPane(wxT("TrapPane")).IsShown());
        break;
    case ID_VIEW_HISTORY_PANE:
        event.Check(_auiManager.GetPane(wxT("HistoryPane")).IsShown());
        break;
    case ID_VIEW_PEER_PANE:
        event.Check(_auiManager.GetPane(wxT("PeerPane")).IsShown());
        break;
    case ID_VIEW_CONFIG_PANE:
        event.Check(_auiManager.GetPane(wxT("ConfigPane")).IsShown());
        break;
    case ID_VIEW_DOWNLOAD_PANE:
        break;
    case ID_VIEW_LOG_PANE:
        event.Check(_auiManager.GetPane(wxT("DebugPane")).IsShown());
        break;
    }
}

void SimCubeFrame::OnResetLayout(wxCommandEvent &WXUNUSED(event))
{
    wxString sql, defaultLayout;
    wxSQLite3ResultSet set;

    sql << wxT("UPDATE CfgTbl SET ConfigValue = '' WHERE ConfigName = 'Perspective'");
    if (1 != _db->ExecuteUpdate(sql))
        wxLogError(_("Fail to reset current perspective value"));

    sql.clear();
    sql << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'DefaultPerspective'");
    set = _db->ExecuteQuery(sql);
    if (set.NextRow())
        defaultLayout = set.GetAsString(0);
    set.Finalize();

    if (!defaultLayout.empty())
        _auiManager.LoadPerspective(defaultLayout);
}

void SimCubeFrame::OnEraseBackground(wxEraseEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnSize(wxSizeEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    Destroy();
}

