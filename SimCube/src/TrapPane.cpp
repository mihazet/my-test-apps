#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "SimCubeApp.h"
#include "TrapPane.h"
#include "led.h"

/////////////////////////////////////////////////////////////////////////////
class LedStatusRadioBox : public wxRadioBox
{
public:
    typedef enum
    {
        LedType1, // used for power
        LedType2, // used for fan, lamp[ab]
    } LedType;

    LedStatusRadioBox(wxWindow *parent, wxWindowID id, const wxString &label,
        const LedType type = LedType2);
    ~LedStatusRadioBox() { }
};

LedStatusRadioBox::LedStatusRadioBox(wxWindow *parent, wxWindowID id,
                                     const wxString &label, const LedType type)
{
    wxArrayString ledStatus;
    ledStatus.push_back(wxT("Off"));
    ledStatus.push_back(wxT("Red"));
    ledStatus.push_back(wxT("Green"));
    ledStatus.push_back(wxT("Orange"));
    ledStatus.push_back(wxT("Green<->Off"));
    ledStatus.push_back(wxT("Off<->Green"));
    ledStatus.push_back(wxT("Red<->Green"));
    ledStatus.push_back(wxT("Green<->Orange"));
    Create(parent, id, label, wxDefaultPosition, wxDefaultSize, ledStatus,
        0, wxVERTICAL);
    SetItemToolTip(0, _("Led is in off state."));
    SetItemToolTip(1, _("Red light constant on."));
    SetItemToolTip(2, _("Green light constant on."));
    SetItemToolTip(3, _("Orange light constant on."));
    SetItemToolTip(4, _("Led light flashing between green and off."));
    SetItemToolTip(5, _("Led light flashing between off and green."));
    SetItemToolTip(6, _("Led light flashing between red and green."));
    SetItemToolTip(7, _("Led light flashing between green and orange."));
    if (type == LedType1)
    {
        Enable(0, false);
        SetSelection(1);
        Enable(4, false);
        Enable(5, false);
    }
    else if (type == LedType2)
    {
        Enable(1, false);
        Enable(3, false);
        Enable(6, false);
        Enable(7, false);
    }
}

/////////////////////////////////////////////////////////////////////////////

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
    wxStaticBoxSizer *ledSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Led Status"));
    allSizer->Add(ledSizer, 0, wxALL | wxEXPAND, 15);
    /* preview */
    wxStaticBoxSizer *ledPreviewSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Preview"));
    ledSizer->Add(ledPreviewSizer, 0, wxALL | wxEXPAND, 5);
    awxLed *awxLedPower = new awxLed(this, wxID_ANY, wxDefaultPosition,
        wxSize(16, 16), awxLED_RED);
    awxLed *awxLedFan = new awxLed(this, wxID_ANY, wxDefaultPosition,
        wxSize(16, 16), awxLED_GREEN);
    awxLed *awxLedLampA = new awxLed(this, wxID_ANY, wxDefaultPosition,
        wxSize(16, 16), awxLED_GREEN);
    awxLed *awxLedLampB = new awxLed(this, wxID_ANY, wxDefaultPosition,
        wxSize(16, 16), awxLED_GREEN);
    ledPreviewSizer->Add(awxLedPower, 1, wxALL, 5);
    ledPreviewSizer->Add(awxLedFan, 1, wxALL, 5);
    ledPreviewSizer->Add(awxLedLampA, 1, wxALL, 5);
    ledPreviewSizer->Add(awxLedLampB, 1, wxALL, 5);

    /* control */
    wxSizer *ledControlSizer = new wxBoxSizer(wxHORIZONTAL);
    ledSizer->Add(ledControlSizer, 0, wxALL | wxEXPAND, 0);
    LedStatusRadioBox *powerRB = new LedStatusRadioBox(this, wxID_ANY, _("Power"),
        LedStatusRadioBox::LedType1);
    LedStatusRadioBox *fanRB = new LedStatusRadioBox(this, wxID_ANY, _("Fan"),
        LedStatusRadioBox::LedType2);
    LedStatusRadioBox *lampaRB = new LedStatusRadioBox(this, wxID_ANY, _("LampA"),
        LedStatusRadioBox::LedType2);
    LedStatusRadioBox *lampbRB = new LedStatusRadioBox(this, wxID_ANY, _("LampB"),
        LedStatusRadioBox::LedType2);
    ledControlSizer->Add(powerRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(fanRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(lampaRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(lampbRB, 1, wxALL|wxEXPAND, 5);

    /* preset */
    wxSizer *ledPresetSizer = new wxBoxSizer(wxHORIZONTAL);
    ledSizer->Add(ledPresetSizer, 0, wxALL | wxEXPAND, 0);
    ledPresetSizer->Add(new wxStaticText(this, wxID_STATIC, _("Quick LED Preset")),
        0, wxALL | wxALIGN_CENTER, 5);
    wxArrayString ledPresetString;
    ledPresetString.push_back(wxT("User define"));
    ledPresetString.push_back(wxT("Standby"));
    ledPresetString.push_back(wxT("Fan Check"));
    ledPresetString.push_back(wxT("Lamp Door Check"));
    ledPresetString.push_back(wxT("..."));
    ledPresetSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, ledPresetString), 1, wxALL, 5);
    ledPresetSizer->AddStretchSpacer();
    ledPresetSizer->Add(new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition,
        wxDefaultSize, wxBU_EXACTFIT), 0, wxALL, 5);

    /* lamp */
    wxBoxSizer *lampSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(lampSizer, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 10);
    /* lamp a */
    wxStaticBoxSizer *lampASizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp A Status"));
    lampSizer->Add(lampASizer, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 5);
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
    lampSizer->Add(lampBSizer, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 5);
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


