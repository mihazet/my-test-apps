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
#include "WidgetId.h"
#include "Version.h"
// resource headers
#include "img/SimCube-16.xpm"
#include "img/SimCube-32.xpm"
#include "img/SimCube-48.xpm"

////////////////////////////////////////////////////////////////////////////////////
class SimCubeAboutDialog : public wxDialog
{
public:
    SimCubeAboutDialog(wxWindow *parent);
    ~SimCubeAboutDialog();

private:
    void CreateControls();

    /* event handlers */
    void OnMouseLButtonUp(wxMouseEvent &event);
};

SimCubeAboutDialog::SimCubeAboutDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, _("About SimCube"))
{
    CreateControls();
    CenterOnParent();

    /* add event handler */
    Bind(wxEVT_LEFT_UP, &SimCubeAboutDialog::OnMouseLButtonUp, this, wxID_ANY);
}

SimCubeAboutDialog::~SimCubeAboutDialog()
{
}

void SimCubeAboutDialog::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);

    /* description */
    wxSizer *descSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(descSizer, 1, wxALL | wxEXPAND, 5);
    wxStaticBitmap *ico = new wxStaticBitmap(this, wxID_ANY, wxBitmap(SimCube_48_xpm));
    descSizer->Add(ico, 0, wxALL | wxEXPAND, 5);
    wxSizer *descTextSizer = new wxBoxSizer(wxVERTICAL);
    descSizer->Add(descTextSizer, 1, wxALL | wxEXPAND, 5);
    wxString msg;
    msg << _("SimCube - A Cube Simulator");
    descTextSizer->Add(new wxStaticText(this, wxID_STATIC, msg), 0, wxALL, 5);
    msg.clear();
    msg << _("Version ") 
        << wxT(SCVER_MAJOR_STRING) << wxT(".") << wxT(SCVER_MINOR_STRING) << wxT(".")
        << wxT(SCVER_RELEASE_STRING) << _(" (build ") << wxT(SCVER_BUILD_STRING)
        << _("), compiled at ") << wxT(SC_BUILDDATE_STRING);
    descTextSizer->Add(new wxStaticText(this, wxID_STATIC, msg), 0, wxALL, 5);
    msg.clear();
    msg << _("If I were a cube, I will try to response all requests from you.");
    allSizer->Add(new wxStaticText(this, wxID_STATIC, msg), 0, wxTOP|wxLEFT|wxRIGHT, 15);
    msg.clear();
    msg << _("If I were a cube, I will notify you of my change of state.");
    allSizer->Add(new wxStaticText(this, wxID_STATIC, msg), 0, wxRIGHT|wxLEFT, 15);
    msg.clear();
    msg << _("If I were a cube..., but I am not a real cube, just a simulated cube.");
    allSizer->Add(new wxStaticText(this, wxID_STATIC, msg), 0, wxBOTTOM|wxLEFT|wxRIGHT, 15);
   
    /* library info */
    wxStaticBoxSizer *lib = new wxStaticBoxSizer(wxVERTICAL, this, _("Libraries Info "));
    allSizer->Add(lib, 0, wxALL | wxEXPAND, 5);
    wxSizer *wx = new wxBoxSizer(wxHORIZONTAL);
    lib->Add(wx, 0, wxALL | wxEXPAND, 0);
    wx->Add(new wxStaticText(this, wxID_STATIC, wxVERSION_STRING), 0, wxALL, 5);
    wxSizer *sqlite = new wxBoxSizer(wxHORIZONTAL);
    lib->Add(sqlite, 0, wxALL | wxEXPAND, 0);
    sqlite->Add(new wxStaticText(this, wxID_STATIC,
        wxT("SQLite ") + wxGetApp().GetPropertyDatabase()->GetVersion()), 0, wxALL, 5);

    SetSizerAndFit(allSizer);
}

void SimCubeAboutDialog::OnMouseLButtonUp(wxMouseEvent &WXUNUSED(event))
{
    EndModal(wxID_OK);
}

////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(SimCubeFrame, wxFrame)
    EVT_MENU_RANGE(myID_VIEW_PANE_START, myID_VIEW_PANE_END, SimCubeFrame::OnViewPane)
    EVT_UPDATE_UI_RANGE(myID_VIEW_PANE_START, myID_VIEW_PANE_END, SimCubeFrame::OnUpdatePane)
    EVT_MENU(myID_VIEW_RESET_LAYOUT, SimCubeFrame::OnResetLayout)
    EVT_ERASE_BACKGROUND(SimCubeFrame::OnEraseBackground)
    EVT_SIZE(SimCubeFrame::OnSize)
    EVT_CLOSE(SimCubeFrame::OnClose)
    EVT_MENU(wxID_ABOUT, SimCubeFrame::OnAbout)
    EVT_MENU(wxID_EXIT, SimCubeFrame::OnQuit)
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
    if (1 != _cfgDB->ExecuteUpdate(sql))
    {
        wxLogError(_("Fail to save perspective!"));
    }

    _auiManager.UnInit();
}

void SimCubeFrame::Init()
{
    _cfgDB = wxGetApp().GetConfigDatabase();
}

void SimCubeFrame::CreateControls()
{
    /* tell wxAuiManager to manage this frame */
    _auiManager.SetManagedWindow(this);

    /* setup icon for window title bar, taskbar and task switching bar */
    wxIconBundle icons;
    icons.AddIcon(wxIcon(SimCube_16_xpm));
    icons.AddIcon(wxIcon(SimCube_32_xpm));
    icons.AddIcon(wxIcon(SimCube_48_xpm));
    SetIcons(icons);

    /* menu bar */
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("Quit"), _("Quit this program"));
    wxMenu *view_menu = new wxMenu;
    view_menu->AppendCheckItem(myID_VIEW_PROPERTY_PANE, _("Property Pane\tCTRL+F1"),
        _("Show or hide the property pane."));
    view_menu->AppendCheckItem(myID_VIEW_TRAP_PANE, _("Trap Pane\tCTRL+F2"),
        _("Show or hid the trap pane."));
    view_menu->AppendCheckItem(myID_VIEW_PEER_PANE, _("Remote Pane\tCTRL+F3"),
        _("Show or hide the remote peer pane."));
    view_menu->AppendCheckItem(myID_VIEW_CONFIG_PANE, _("Configuration Pane\tCTRL+F4"),
        _("Show or hide the configuration pane."));
    view_menu->AppendCheckItem(myID_VIEW_LOG_PANE, _("Log Widnow\tCTRL+F5"),
        _("Show or hide the logging window."));
    view_menu->AppendSeparator();
    view_menu->Append(myID_VIEW_RESET_LAYOUT, _("Reset all panes"),
        _("Reset all panes to its default position and size."));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(myID_HELP_DOC, _("Help file"),
        _("Show help file."));
    help_menu->Append(wxID_ABOUT, _("About..."), _("Show about dialog"));

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
        CloseButton(false).DestroyOnClose(false).MaximizeButton(true).
        MinSize(500, 300));

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
    if (1 != _cfgDB->ExecuteUpdate(sql))
    {
        wxLogError(_("Fail to save software default perspective!"));
    }

    /* restore previous perspective or just use current perspective */
    sql.clear();
    sql << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Perspective'");
    set = _cfgDB->ExecuteQuery(sql);
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
    wxSQLite3Database *db = wxGetApp().GetConfigDatabase();
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
    case myID_VIEW_PROPERTY_PANE:
        _auiManager.GetPane(wxT("PropertyPane")).Show(event.IsChecked());
        break;
    case myID_VIEW_TRAP_PANE:
        _auiManager.GetPane(wxT("TrapPane")).Show(event.IsChecked());
        break;
    case myID_VIEW_PEER_PANE:
        _auiManager.GetPane(wxT("PeerPane")).Show(event.IsChecked());
        break;
    case myID_VIEW_CONFIG_PANE:
        _auiManager.GetPane(wxT("ConfigPane")).Show(event.IsChecked());
        break;
    case myID_VIEW_LOG_PANE:
        _auiManager.GetPane(wxT("DebugPane")).Show(event.IsChecked());
        break;
    }
    _auiManager.Update();
}

void SimCubeFrame::OnUpdatePane(wxUpdateUIEvent &event)
{
    switch (event.GetId())
    {
    case myID_VIEW_PROPERTY_PANE:
        event.Check(_auiManager.GetPane(wxT("PropertyPane")).IsShown());
        break;
    case myID_VIEW_TRAP_PANE:
        event.Check(_auiManager.GetPane(wxT("TrapPane")).IsShown());
        break;
    case myID_VIEW_PEER_PANE:
        event.Check(_auiManager.GetPane(wxT("PeerPane")).IsShown());
        break;
    case myID_VIEW_CONFIG_PANE:
        event.Check(_auiManager.GetPane(wxT("ConfigPane")).IsShown());
        break;
    case myID_VIEW_LOG_PANE:
        event.Check(_auiManager.GetPane(wxT("DebugPane")).IsShown());
        break;
    }
}

void SimCubeFrame::OnResetLayout(wxCommandEvent &WXUNUSED(event))
{
    wxString sql, defaultLayout;
    wxSQLite3ResultSet set;

    sql << wxT("UPDATE CfgTbl SET ConfigValue = '' WHERE ConfigName = 'Perspective'");
    if (1 != _cfgDB->ExecuteUpdate(sql))
    {
        wxLogError(_("Fail to reset current perspective value"));
    }

    sql.clear();
    sql << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'DefaultPerspective'");
    set = _cfgDB->ExecuteQuery(sql);
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

void SimCubeFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    SimCubeAboutDialog dlg(this);
    dlg.ShowModal();
}

void SimCubeFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

