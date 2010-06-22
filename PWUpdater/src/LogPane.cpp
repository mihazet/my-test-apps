// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/stdpaths.h>
#include "LogPane.h"

#define wxLOG_COMPONENT "PWUpdater/ui/log"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "xpm/save_16.xpm"
#include "xpm/erase_16.xpm"

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
LogPane::LogPane()
{
    Init();
}

LogPane::LogPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

LogPane::~LogPane()
{
    if (_oldLogTarget)
        delete _oldLogTarget;
}

bool LogPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();

    /* setup log target */
    if (_logTextCtrl)
    {
        _oldLogTarget = wxLog::SetActiveTarget(new wxLogTextCtrl(_logTextCtrl));
        wxLog::SetTimestamp(wxT("[%Y/%m/%d-%H:%M:%S]"));
    }
    return true;
}

//
// Helper functions
//
void LogPane::Init()
{
    _logTextCtrl = NULL;
    _oldLogTarget = NULL;
}

void LogPane::CreateControls()
{
    wxBoxSizer *allSizer = new wxBoxSizer(wxHORIZONTAL);
    /* log window */
    _logTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE|wxTE_READONLY);
    allSizer->Add(_logTextCtrl, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);

    /* ctrl buttons */
    wxBoxSizer *ctrlSizer = new wxBoxSizer(wxVERTICAL);
    allSizer->Add(ctrlSizer, 0, wxALL|wxEXPAND, 0);
    wxBitmapButton *save = new wxBitmapButton(this, wxID_ANY, wxBitmap(save_16_xpm));
    save->SetToolTip(_("Save log text to file"));
    save->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LogPane::OnSave, this);
    wxBitmapButton *erase = new wxBitmapButton(this, wxID_ANY, wxBitmap(erase_16_xpm));
    erase->SetToolTip(_("Erase all log text"));
    erase->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LogPane::OnErase, this);

    ctrlSizer->Add(save, 0, wxTOP, 5);
    ctrlSizer->Add(erase, 0, wxALL, 0);
    SetSizer(allSizer);
}

bool LogPane::IsVerbose()
{
    bool result = false;

    return result;
}

void LogPane::SetVerbose(bool verbose)
{
    wxLog::SetVerbose(verbose);
}

//
// Event handlers
//
void LogPane::OnSave(wxCommandEvent &WXUNUSED(event))
{
    wxStandardPathsBase &stdPaths = wxStandardPaths::Get();
    wxString defaultDir = stdPaths.GetDocumentsDir();
    wxFileDialog dlg(this);
    dlg.SetDirectory(defaultDir);
    wxString wildcard = _("Log files") + wxT(" (*.log)|*.log");
    dlg.SetWildcard(wildcard);
    dlg.SetWindowStyle(wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() == wxID_OK)
    {
        _logTextCtrl->SaveFile(dlg.GetPath());
    }
}

void LogPane::OnErase(wxCommandEvent &WXUNUSED(event))
{
    _logTextCtrl->Clear();
}
