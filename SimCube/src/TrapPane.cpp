#include <wx/wx.h>
#include "SimCubeApp.h"
#include "TrapPane.h"

TrapPane::TrapPane()
{
    Init();
}

TrapPane::TrapPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

TrapPane::~TrapPane()
{

}

bool TrapPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void TrapPane::Init()
{
    _db = wxGetApp().GetMainDatabase();
}

void TrapPane::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer *ledSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Led Status"));
    allSizer->Add(ledSizer, 0, wxALL | wxEXPAND, 5);
    wxArrayString ledStatus;
    ledStatus.push_back(_("Off"));
    ledStatus.push_back(_("Red"));
    ledStatus.push_back(_("Green"));
    ledStatus.push_back(_("Orange"));
    ledStatus.push_back(_("Green<->Off"));
    ledStatus.push_back(_("Off<->Green"));
    ledStatus.push_back(_("Red<->Green"));
    ledStatus.push_back(_("Green<->Orange"));
    wxRadioBox *powerRB = new wxRadioBox(this, wxID_ANY, _("Power"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    wxRadioBox *fanRB = new wxRadioBox(this, wxID_ANY, _("Fan"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    wxRadioBox *lampaRB = new wxRadioBox(this, wxID_ANY, _("LampA"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    wxRadioBox *lampbRB = new wxRadioBox(this, wxID_ANY, _("LampB"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    ledSizer->Add(powerRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(fanRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(lampaRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(lampbRB, 0, wxALL|wxEXPAND, 5);

    wxBoxSizer *lampSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(lampSizer, 1, wxALL | wxEXPAND, 5);
    wxStaticBoxSizer *lampASizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp A Status"));
    lampSizer->Add(lampASizer, 1, wxALL | wxEXPAND, 0);
    wxStaticBoxSizer *lampBSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp B Status"));
    lampSizer->Add(lampBSizer, 1, wxALL | wxEXPAND, 0);
    
    SetSizerAndFit(allSizer);
}


