// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "UpdaterApp.h"
#include "LogPane.h"
#include "WidgetId.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/save_16.xpm"
#include "img/delete_16.xpm"
#include "img/speaker_16.xpm"
#include "img/cfg_16.xpm"

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
        wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2);
    allSizer->Add(_logTextCtrl, 1, wxTOP|wxBOTTOM|wxLEFT|wxEXPAND, 5);

    /* ctrl buttons */
    wxBoxSizer *ctrlSizer = new wxBoxSizer(wxVERTICAL);
    allSizer->Add(ctrlSizer, 0, wxALL|wxEXPAND, 0);
    wxBitmapButton *save = new wxBitmapButton(this, wxID_ANY, wxBitmap(save_16_xpm));
    save->SetToolTip(_("Save log text to file"));
    wxBitmapButton *erase = new wxBitmapButton(this, wxID_ANY, wxBitmap(delete_16_xpm));
    erase->SetToolTip(_("Erase all log text"));

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
    wxBitmapButton *config = new wxBitmapButton(this, wxID_ANY, wxBitmap(cfg_16_xpm));
    config->SetToolTip(_("Config"));
    ctrlSizer->Add(save, 0, wxTOP, 5);
    ctrlSizer->Add(erase, 0, wxALL, 0);
    ctrlSizer->Add(verbose, 0, wxTOP, 5);
    ctrlSizer->Add(config, 0, wxTOP|wxBOTTOM, 5);
    SetSizer(allSizer);
}

bool LogPane::IsVerbose()
{
    return true;
}

void LogPane::SetVerbose(bool verbose)
{
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
