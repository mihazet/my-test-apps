#include <wx/wx.h>
#include "DebugPane.h"
#include "WidgetId.h"

#include "img/save_16.xpm"
#include "img/delete_16.xpm"
#include "img/speaker_16.xpm"
#include "img/cfg_16.xpm"

DebugPane::DebugPane()
{
    Init();
}

DebugPane::DebugPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

DebugPane::~DebugPane()
{
    wxDELETE(_verboseImg);
}

bool DebugPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void DebugPane::Init()
{
    _logTextCtrl = NULL;
    _verboseImg = new wxImage(speaker_16_xpm);
}

void DebugPane::CreateControls()
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
    wxBitmapButton *verbose = new wxBitmapButton(this, wxID_ANY, wxBitmap(*_verboseImg));
    verbose->SetToolTip(_("Verbose"));
    wxBitmapButton *config = new wxBitmapButton(this, wxID_ANY, wxBitmap(cfg_16_xpm));
    config->SetToolTip(_("Config"));
    ctrlSizer->Add(save, 0, wxTOP, 5);
    ctrlSizer->Add(erase, 0, wxALL, 0);
    ctrlSizer->Add(verbose, 0, wxTOP, 5);
    ctrlSizer->Add(config, 0, wxTOP|wxBOTTOM, 5);
    SetSizer(allSizer);
}

