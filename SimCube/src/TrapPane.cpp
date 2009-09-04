#include <wx/wx.h>
#include <wx/spinctrl.h>
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
    powerRB->Enable(4, false);
    powerRB->Enable(5, false);
    wxRadioBox *fanRB = new wxRadioBox(this, wxID_ANY, _("Fan"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    fanRB->Enable(1, false);
    fanRB->Enable(3, false);
    fanRB->Enable(6, false);
    fanRB->Enable(7, false);
    wxRadioBox *lampaRB = new wxRadioBox(this, wxID_ANY, _("LampA"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    lampaRB->Enable(1, false);
    lampaRB->Enable(3, false);
    lampaRB->Enable(6, false);
    lampaRB->Enable(7, false);
    wxRadioBox *lampbRB = new wxRadioBox(this, wxID_ANY, _("LampB"), wxDefaultPosition,
        wxDefaultSize, ledStatus, 0, wxVERTICAL);
    lampbRB->Enable(1, false);
    lampbRB->Enable(3, false);
    lampbRB->Enable(6, false);
    lampbRB->Enable(7, false);
    ledSizer->Add(powerRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(fanRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(lampaRB, 0, wxALL|wxEXPAND, 5);
    ledSizer->Add(lampbRB, 0, wxALL|wxEXPAND, 5);

    /* lamp */
    wxBoxSizer *lampSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(lampSizer, 1, wxALL | wxEXPAND, 0);
    /* lamp a */
    wxStaticBoxSizer *lampASizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp A Status"));
    lampSizer->Add(lampASizer, 1, wxALL | wxEXPAND, 5);
    wxBoxSizer *lampAStatusSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampAStatusSizer, 0, wxALL | wxEXPAND, 0);
    lampAStatusSizer->Add(new wxStaticText(this, wxID_STATIC, _("Status")), 0, wxALL, 5);
    wxArrayString lampStatus;
    lampStatus.push_back(_("Off"));
    lampStatus.push_back(_("In use"));
    lampStatus.push_back(_("Lit failed"));
    lampAStatusSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampStatus), 1, wxALL, 5);
    lampAStatusSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    wxBoxSizer *lampAHoursSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampAHoursSizer, 0, wxALL | wxEXPAND, 0);
    lampAHoursSizer->Add(new wxStaticText(this, wxID_STATIC, _("Hours")), 0, wxALL, 5);
    lampAHoursSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("3"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    lampAHoursSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    wxBoxSizer *lampALitCountSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampALitCountSizer, 0, wxALL | wxEXPAND, 0);
    lampALitCountSizer->Add(new wxStaticText(this, wxID_STATIC, _("Lit Count")), 0, wxALL, 5);
    lampALitCountSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("5"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    lampALitCountSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL | wxEXPAND, 5);
    wxBoxSizer *lampATempCondSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampATempCondSizer, 0, wxALL | wxEXPAND, 0);
    lampATempCondSizer->Add(new wxStaticText(this, wxID_STATIC, _("Temp Cond")), 0, wxALL, 5);
    wxArrayString lampTempCond;
    lampTempCond.push_back(_("Shutdown"));
    lampTempCond.push_back(_("Out of spec"));
    lampTempCond.push_back(_("Out of spec"));
    lampTempCond.push_back(_("Critical"));
    lampTempCond.push_back(_("Ok"));
    lampTempCond.push_back(_("Ok"));
    lampTempCond.push_back(_("Ok"));
    lampTempCond.push_back(_("Ok"));
    lampATempCondSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampTempCond), 1, wxALL, 5);
    lampATempCondSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    /* lamp b */
    wxStaticBoxSizer *lampBSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp B Status"));
    lampSizer->Add(lampBSizer, 1, wxALL | wxEXPAND, 5);
    wxBoxSizer *lampBStatusSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBStatusSizer, 0, wxALL | wxEXPAND, 0);
    lampBStatusSizer->Add(new wxStaticText(this, wxID_STATIC, _("Status")), 0, wxALL, 5);
    lampBStatusSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampStatus), 1, wxALL, 5);
    lampBStatusSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    wxBoxSizer *lampBHoursSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBHoursSizer, 0, wxALL | wxEXPAND, 0);
    lampBHoursSizer->Add(new wxStaticText(this, wxID_STATIC, _("Hours")), 0, wxALL, 5);
    lampBHoursSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("3"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    lampBHoursSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    wxBoxSizer *lampBLitCountSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBLitCountSizer, 0, wxALL | wxEXPAND, 0);
    lampBLitCountSizer->Add(new wxStaticText(this, wxID_STATIC, _("Lit Count")), 0, wxALL, 5);
    lampBLitCountSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("5"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    lampBLitCountSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);
    wxBoxSizer *lampBTempCondSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBTempCondSizer, 0, wxALL | wxEXPAND, 0);
    lampBTempCondSizer->Add(new wxStaticText(this, wxID_STATIC, _("Temp Cond")), 0, wxALL, 5);
    lampBTempCondSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampTempCond), 1, wxALL, 5);
    lampBTempCondSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);

    SetSizerAndFit(allSizer);
}


