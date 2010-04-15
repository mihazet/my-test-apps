// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/iconbndl.h>
#include <wx/aboutdlg.h>
#include "UpdaterFrame.h"
#include "UpdaterApp.h"
#include "LogPane.h"
#include "DownloadPane.h"
#include "MacAddrUsagePane.h"
#include "AppPreferencePane.h"
#include "WidgetId.h"
#include "Version.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/update_16.xpm"
#include "img/update_32.xpm"
#include "img/update_48.xpm"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
#define PANE_NAME_LOG               wxT("LogPane")
#define PANE_NAME_IMAGE_UPDATE      wxT("DownloadPane")
#define PANE_NAME_MAC_USAGE         wxT("MacAddrUsagePane")
#define PANE_NAME_PREFERENCE        wxT("PreferencePane")

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(UpdaterFrame, wxFrame)
    EVT_MENU_RANGE(myID_VIEW_PANE_START, myID_VIEW_PANE_END, UpdaterFrame::OnViewPane)
    EVT_UPDATE_UI_RANGE(myID_VIEW_PANE_START, myID_VIEW_PANE_END, UpdaterFrame::OnUpdatePane)
    EVT_MENU(myID_VIEW_RESET_LAYOUT, UpdaterFrame::OnResetLayout)
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
    AppOptions *pOpt = wxGetApp().m_pAppOptions;
    wxString perspective;

    if (pOpt)
        pOpt->SetOption(wxT("ActivePerspective"), _auiManager.SavePerspective());

    _auiManager.UnInit();
}

void UpdaterFrame::Init()
{
}

void UpdaterFrame::CreateControls()
{
    /* tell wxAuiManager to manage this frame */
    _auiManager.SetManagedWindow(this);
    _auiManager.SetFlags(_auiManager.GetFlags() | wxAUI_MGR_LIVE_RESIZE);

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
    view_menu->AppendRadioItem(myID_VIEW_MAC_ADDR_USAGE_PANE, _("MAC Address Usage Window\tCTRL+U"),
        _("Show or hide the MAC usage pane."));
    view_menu->AppendRadioItem(myID_VIEW_DOWNLOAD_PANE, _("Device Download Window\tCTRL+D"),
        _("Show or hide the MAC update pane."));
    view_menu->AppendSeparator();
    view_menu->AppendCheckItem(myID_VIEW_LOG_PANE, _("Log Window\tCTRL+L"),
        _("Show or hide the log window."));
    view_menu->AppendCheckItem(myID_VIEW_OPTION_PANE, _("Preference\tCTRL+P"),
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
    _auiManager.AddPane(dbgWin, wxAuiPaneInfo().Name(PANE_NAME_LOG).
        Caption(_("Log Window")).Bottom().CloseButton(true).
        DestroyOnClose(false).MaximizeButton(false).MinSize(300, 150));

    _auiManager.AddPane(new DownloadPane(this), wxAuiPaneInfo().
        Name(PANE_NAME_IMAGE_UPDATE).Caption(_("Device Download Window")).Center().
        CloseButton(false).DestroyOnClose(false).MaximizeButton(true).
        MinSize(300, -1));

    _auiManager.AddPane(new MacAddrUsagePane(this), wxAuiPaneInfo().
        Name(PANE_NAME_MAC_USAGE).Caption(_("MAC Address Usage Window")).Center().
        CloseButton(false).DestroyOnClose(false).MaximizeButton(true).
        MinSize(300, 300).Hide());

    _auiManager.AddPane(new AppPreferencePane(this), wxAuiPaneInfo().
        Name(PANE_NAME_PREFERENCE).Caption(_("Preference")).Float().Dockable(false).
        CloseButton(true).DestroyOnClose(false).MaximizeButton(false).
        MinSize(450, 400).Hide());

    /* status bar */
    wxStatusBar *sBar = new wxStatusBar(this, myID_SBAR);
    int fieldWidth[SBAR_FIELD_MAX], field;
    for (field = 0; field < SBAR_FIELD_MAX; field++)
        fieldWidth[field] = -1;
    fieldWidth[SBAR_FIELD_ACTIVE_INTERFACE] = 200;
    sBar->SetFieldsCount(SBAR_FIELD_MAX, &fieldWidth[0]);
    SetStatusBar(sBar);

    /* update default perspective (use above layout code) */
    wxString perspective;
    bool useLastLayout = false;
    AppOptions *pOpt = wxGetApp().m_pAppOptions;
    if (pOpt)
    {
        pOpt->SetOption(wxT("DefaultPerspective"), _auiManager.SavePerspective());
        if (pOpt->GetOption(wxT("ActivePerspective"), perspective))
        {
            if (perspective.empty())
                useLastLayout = false;
            else
                useLastLayout = true;
        }
    }

    if (useLastLayout)
        _auiManager.LoadPerspective(perspective);
    else
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
    int evtId = event.GetId(), paneId;
    wxString paneName;

    if ((evtId > myID_VIEW_RADIO_PANE_START) && (evtId < myID_VIEW_RADIO_PANE_END))
    {
        for (paneId = myID_VIEW_RADIO_PANE_START;
            paneId < myID_VIEW_RADIO_PANE_END;
            paneId++)
        {
            switch (paneId)
            {
            case myID_VIEW_MAC_ADDR_USAGE_PANE:
                paneName = PANE_NAME_MAC_USAGE;
                break;
            case myID_VIEW_DOWNLOAD_PANE:
                paneName = PANE_NAME_IMAGE_UPDATE;
                break;
            default:
                continue;
            }
            _auiManager.GetPane(paneName).Show((evtId == paneId) ? true : false);
        }
    }
    else if ((evtId > myID_VIEW_CHECK_PANE_START) && (evtId < myID_VIEW_CHECK_PANE_END))
    {
        switch (evtId)
        {
        case myID_VIEW_LOG_PANE:
            paneName = PANE_NAME_LOG;
            break;
        case myID_VIEW_OPTION_PANE:
            paneName = PANE_NAME_PREFERENCE;
            break;
        default:
            return;
        }
        _auiManager.GetPane(paneName).Show(event.IsChecked());
    }
    _auiManager.Update();
}

void UpdaterFrame::OnUpdatePane(wxUpdateUIEvent &event)
{
    int evtId = event.GetId();
    wxString paneName;

    switch (evtId)
    {
    case myID_VIEW_MAC_ADDR_USAGE_PANE:
        paneName = PANE_NAME_MAC_USAGE;
        break;
    case myID_VIEW_DOWNLOAD_PANE:
        paneName = PANE_NAME_IMAGE_UPDATE;
        break;
    case myID_VIEW_LOG_PANE:
        paneName = PANE_NAME_LOG;
        break;
    case myID_VIEW_OPTION_PANE:
        paneName = PANE_NAME_PREFERENCE;
        break;
    default:
        return;
    }
    event.Check(_auiManager.GetPane(paneName).IsShown());
}

void UpdaterFrame::OnResetLayout(wxCommandEvent &WXUNUSED(event))
{
    wxString defaultLayout;
    AppOptions *pOpt = wxGetApp().m_pAppOptions;

    if (pOpt)
    {
        pOpt->SetOption(wxT("ActivePerspective"), wxEmptyString);
        if (pOpt->GetOption(wxT("DefaultPerspective"), defaultLayout)
            && !defaultLayout.empty())
            _auiManager.LoadPerspective(defaultLayout);
    }
}

void UpdaterFrame::OnEraseBackground(wxEraseEvent &event)
{
    event.Skip();
}

void UpdaterFrame::OnSize(wxSizeEvent &WXUNUSED(event))
{
    //event.Skip();
}

void UpdaterFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    Destroy();
}

void UpdaterFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxAboutDialogInfo info;
    wxString copyright, description, version, longVersion;

    copyright
        << wxT("Copyright 2010 (c), Delta Electronics, Inc. All Rights Reserved.");
    description
        << wxT("This program is used to download the firmware and/or bootloader\n")
        << wxT("into flash memory in parallel for the RELTA platform.\n")
        << wxT("And it also helps user to manage the MAC address management.\n");
    version
        << VER_MAJOR_STRING << "." << VER_MINOR_STRING << "." << VER_RELEASE_STRING;
    longVersion = version + "." + VER_BUILD_STRING;

    info.SetDescription(description);
    info.SetCopyright(copyright);
    info.SetIcon(wxIcon(update_48_xpm));
    info.SetName(wxT("Updater"));
    info.SetVersion(version, longVersion);

    wxAboutBox(info, this);
}

void UpdaterFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

