// wx headers
#include <wx/wx.h>
#include <wx/iconbndl.h>
// our headers
#include "DebugPane.h"
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
    _auiManager.AddPane(new PropertyPane(this), wxAuiPaneInfo().
        Name(wxT("PropertyPane")).Caption(_("Property List")).Top().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true));
    _auiManager.AddPane(new DebugPane(this), wxAuiPaneInfo().
        Name(wxT("DebugPane")).Caption(_("Log Output")).Bottom().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true));

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

