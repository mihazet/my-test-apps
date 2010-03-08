// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/stdpaths.h>
#include "UpdaterApp.h"
#include "LogPane.h"
#include "WidgetId.h"
#include "AppOptions.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/save_16.xpm"
#include "img/delete_16.xpm"
#include "img/speaker_16.xpm"

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
    wxDELETE(_verboseImg);
}

bool LogPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

//
// Helper functions
//
void LogPane::Init()
{
    _logTextCtrl = NULL;
    _verboseImg = new wxImage(speaker_16_xpm);
}

void LogPane::CreateControls()
{
    wxBoxSizer *allSizer = new wxBoxSizer(wxHORIZONTAL);
    /* log text ctrl */
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
    wxBitmapButton *erase = new wxBitmapButton(this, wxID_ANY, wxBitmap(delete_16_xpm));
    erase->SetToolTip(_("Erase all log text"));
    erase->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LogPane::OnErase, this);

    wxBitmapButton *verbose;
    if (IsVerbose())
    {
        verbose = new wxBitmapButton(this, wxID_ANY, wxBitmap(*_verboseImg));
        verbose->SetToolTip(_("Disable verbose message"));
        wxLog::SetVerbose(true);
    }
    else
    {
        verbose = new wxBitmapButton(this, wxID_ANY, wxBitmap(_verboseImg->ConvertToGreyscale()));
        verbose->SetToolTip(_("Enable verbose message"));
        wxLog::SetVerbose(false);
    }
    verbose->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LogPane::OnVerbose, this);
    ctrlSizer->Add(save, 0, wxTOP, 5);
    ctrlSizer->Add(erase, 0, wxALL, 0);
    ctrlSizer->Add(verbose, 0, wxTOP, 5);
    SetSizer(allSizer);
}

bool LogPane::IsVerbose()
{
    bool result = false;
    wxString strValue;
    long longValue;
    AppOptions *pOpt = wxGetApp().m_pAppOptions;

    if (pOpt && pOpt->GetOption(wxT("LogVerbose"), strValue))
    {
        if (strValue.ToLong(&longValue))
            result = (longValue == 1);
    }

    return result;
}

void LogPane::SetVerbose(bool verbose)
{
    wxString strValue = verbose ? wxT("1") : wxT("0");
    AppOptions *pOpt = wxGetApp().m_pAppOptions;

    if (pOpt && pOpt->SetOption(wxT("LogVerbose"), strValue))
        wxLog::SetVerbose(verbose);
}

//
// Event handlers
//
void LogPane::OnVerbose(wxCommandEvent &event)
{
    wxBitmapButton *btn = wxDynamicCast(event.GetEventObject(), wxBitmapButton);
    if (IsVerbose())
    {
        SetVerbose(false);
        btn->SetBitmap(wxBitmap(_verboseImg->ConvertToGreyscale()));
        btn->SetToolTip(_("Enable verbose message"));
    }
    else
    {
        SetVerbose(true);
        btn->SetBitmap(wxBitmap(*_verboseImg));
        btn->SetToolTip(_("Disable verbose message"));
    }
}

void LogPane::OnSave(wxCommandEvent &WXUNUSED(event))
{
    wxStandardPathsBase &stdPaths = wxStandardPaths::Get();
    wxString defaultDir = stdPaths.GetDocumentsDir();
    wxFileDialog dlg(this);
    dlg.SetDirectory(defaultDir);
    dlg.SetWildcard(_("Log files (*.log)|*.log"));
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
