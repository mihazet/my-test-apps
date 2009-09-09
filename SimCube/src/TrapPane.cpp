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

class LedPresetData : public wxObjectRefData
{
public:
    wxVector<int> m_LedStatus;
};

class LedStatusPreset : public wxChoice
{
public:
    LedStatusPreset(wxWindow *parent, wxWindowID id = wxID_ANY);
    ~LedStatusPreset() { };
};

LedStatusPreset::LedStatusPreset(wxWindow *parent, wxWindowID id)
{
    /* preset and associate data */
    wxArrayString ledPresetString;
    LedPresetData *refObj = new LedPresetData;
    ledPresetString.push_back(wxT("User define"));
    refObj->m_LedStatus.push_back(0x1000);
    ledPresetString.push_back(wxT("Standby"));
    refObj->m_LedStatus.push_back(0x1000);
    ledPresetString.push_back(wxT("Fan Detection"));
    refObj->m_LedStatus.push_back(0x2400);
    ledPresetString.push_back(wxT("LampDoor Detection"));
    refObj->m_LedStatus.push_back(0x2444);
    ledPresetString.push_back(wxT("LampOn Trying - LampA+B"));
    refObj->m_LedStatus.push_back(0x2244);
    ledPresetString.push_back(wxT("LampOn Trying - LampA"));
    refObj->m_LedStatus.push_back(0x2240);
    ledPresetString.push_back(wxT("LampOn Trying - LampB"));
    refObj->m_LedStatus.push_back(0x2204);
    ledPresetString.push_back(wxT("LampOn Idle - LampA+B"));
    refObj->m_LedStatus.push_back(0x2222);
    ledPresetString.push_back(wxT("LampOn Idle - LampA"));
    refObj->m_LedStatus.push_back(0x2220);
    ledPresetString.push_back(wxT("LampOn Idle - LampB"));
    refObj->m_LedStatus.push_back(0x2202);
    ledPresetString.push_back(wxT("LampOff - LampA+B"));
    refObj->m_LedStatus.push_back(0x2244);
    ledPresetString.push_back(wxT("LampOff - LampA"));
    refObj->m_LedStatus.push_back(0x2240);
    ledPresetString.push_back(wxT("LampOff - LampB"));
    refObj->m_LedStatus.push_back(0x2204);
    ledPresetString.push_back(wxT("LampOff Fan Cooling"));
    refObj->m_LedStatus.push_back(0x2200);
    ledPresetString.push_back(wxT("Control BD Fan Error - LampA+B"));
    refObj->m_LedStatus.push_back(0x1544);
    ledPresetString.push_back(wxT("Control BD Fan Error - LampA"));
    refObj->m_LedStatus.push_back(0x1540);
    ledPresetString.push_back(wxT("Control BD Fan Error - LampB"));
    refObj->m_LedStatus.push_back(0x1504);
    ledPresetString.push_back(wxT("DMD BD Fan Error - LampA+B"));
    refObj->m_LedStatus.push_back(0x1444);
    ledPresetString.push_back(wxT("DMD BD Fan Error - LampA"));
    refObj->m_LedStatus.push_back(0x1440);
    ledPresetString.push_back(wxT("DMD BD Fan Error - LampB"));
    refObj->m_LedStatus.push_back(0x1404);
    ledPresetString.push_back(wxT("LampDoor Error"));
    refObj->m_LedStatus.push_back(0x1245);
    ledPresetString.push_back(wxT("LampOn LampA+B Fail - LampA+B"));
    refObj->m_LedStatus.push_back(0x6444);
    ledPresetString.push_back(wxT("LampOn LampA Fail - LampA+B"));
    refObj->m_LedStatus.push_back(0x7454);
    ledPresetString.push_back(wxT("LampOn LampB Fail - LampA+B"));
    refObj->m_LedStatus.push_back(0x7445);
    ledPresetString.push_back(wxT("LampOn LampA Fail - LampA"));
    refObj->m_LedStatus.push_back(0x6440);
    ledPresetString.push_back(wxT("LampOn LampB Fail - LampB"));
    refObj->m_LedStatus.push_back(0x6404);

    /* create gui and asscoiate with data */
    Create(parent, id, wxDefaultPosition, wxDefaultSize, ledPresetString);
    SetRefData(refObj);
    SetSelection(0);
}

/////////////////////////////////////////////////////////////////////////////

enum
{
    myID_LED_POWER = wxID_HIGHEST + 200,
    myID_LED_FAN,
    myID_LED_LAMPA,
    myID_LED_LAMPB,
    myID_RB_LED_POWER,
    myID_RB_LED_FAN,
    myID_RB_LED_LAMPA,
    myID_RB_LED_LAMPB,
    myID_CHOICE_LED_PRESET,
    myID_BTN_LEDSTATUS_SEND,
    myID_BTN_LAMPASTATE_SEND,
    myID_BTN_LAMPBSTATE_SEND,
    myID_BTN_LAMPAHOURS_SEND,
    myID_BTN_LAMPBHOURS_SEND,
    myID_BTN_LAMPALITCNT_SEND,
    myID_BTN_LAMPBLITCNT_SEND,
    myID_BTN_LAMPATEMPCOND_SEND,
    myID_BTN_LAMPBTEMPCOND_SEND,
};

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
    wxString sqlQuery;
    wxSQLite3ResultSet set;
    _db = wxGetApp().GetMainDatabase();
    sqlQuery << wxT("SELECT CurrentValue FROM TrapTbl WHERE ProtocolName = 'LEDSTATUS'");
    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        _ledStatus = set.GetInt(0);
    set.Finalize();
}

void TrapPane::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer *ledSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Led Status"));
    allSizer->Add(ledSizer, 0, wxALL | wxEXPAND, 15);
    /* preview */
    wxStaticBoxSizer *ledPreviewSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Preview"));
    ledSizer->Add(ledPreviewSizer, 0, wxALL | wxEXPAND, 5);
    awxLed *awxLedPower = new awxLed(this, myID_LED_POWER, (_ledStatus >> 12) & 0x7);
    awxLed *awxLedFan = new awxLed(this, myID_LED_FAN, (_ledStatus >> 8) & 0x7);
    awxLed *awxLedLampA = new awxLed(this, myID_LED_LAMPA, (_ledStatus >> 4) & 0x7);
    awxLed *awxLedLampB = new awxLed(this, myID_LED_LAMPB, (_ledStatus >> 0) & 0x7);
    ledPreviewSizer->Add(awxLedPower, 1, wxALL, 10);
    ledPreviewSizer->Add(awxLedFan, 1, wxALL, 10);
    ledPreviewSizer->Add(awxLedLampA, 1, wxALL, 10);
    ledPreviewSizer->Add(awxLedLampB, 1, wxALL, 10);

    /* control */
    wxSizer *ledControlSizer = new wxBoxSizer(wxHORIZONTAL);
    ledSizer->Add(ledControlSizer, 0, wxALL | wxEXPAND, 0);
    LedStatusRadioBox *powerRB = new LedStatusRadioBox(this, myID_RB_LED_POWER,
        _("Power"), LedStatusRadioBox::LedType1);
    powerRB->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &TrapPane::OnLedStatusChosen, this);
    powerRB->SetSelection((_ledStatus >> 12) & 0x7);
    LedStatusRadioBox *fanRB = new LedStatusRadioBox(this, myID_RB_LED_FAN,
        _("Fan"), LedStatusRadioBox::LedType2);
    fanRB->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &TrapPane::OnLedStatusChosen, this);
    fanRB->SetSelection((_ledStatus >> 8) & 0x7);
    LedStatusRadioBox *lampaRB = new LedStatusRadioBox(this, myID_RB_LED_LAMPA,
        ("LampA"), LedStatusRadioBox::LedType2);
    lampaRB->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &TrapPane::OnLedStatusChosen, this);
    lampaRB->SetSelection((_ledStatus >> 4) & 0x7);
    LedStatusRadioBox *lampbRB = new LedStatusRadioBox(this, myID_RB_LED_LAMPB,
        _("LampB"), LedStatusRadioBox::LedType2);
    lampbRB->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED, &TrapPane::OnLedStatusChosen, this);
    lampbRB->SetSelection((_ledStatus >> 0) & 0x7);
    ledControlSizer->Add(powerRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(fanRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(lampaRB, 1, wxALL|wxEXPAND, 5);
    ledControlSizer->Add(lampbRB, 1, wxALL|wxEXPAND, 5);

    /* preset */
    wxSizer *ledPresetSizer = new wxBoxSizer(wxHORIZONTAL);
    ledSizer->Add(ledPresetSizer, 0, wxALL | wxEXPAND, 0);
    ledPresetSizer->Add(new wxStaticText(this, wxID_STATIC, _("Quick LED Preset")),
        0, wxALL | wxALIGN_CENTER, 5);
    LedStatusPreset *ledPreset = new LedStatusPreset(this, myID_CHOICE_LED_PRESET);
    ledPreset->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &TrapPane::OnLedPresetChosen, this);
    ledPresetSizer->Add(ledPreset, 1, wxALL, 5);

    ledPresetSizer->AddStretchSpacer();
    wxButton *ledStatusBtn = new wxButton(this, myID_BTN_LEDSTATUS_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    ledStatusBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLedStatusSend, this);
    ledPresetSizer->Add(ledStatusBtn, 0, wxALL, 5);

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
    lampStatus.push_back(wxT("OFF"));
    lampStatus.push_back(wxT("IN_USE"));
    lampStatus.push_back(wxT("LIGHT_FAILED"));
    lampAStatusSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampStatus), 1, wxALL, 5);
    wxButton *lampAStateBtn = new wxButton(this, myID_BTN_LAMPASTATE_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampAStateBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampAStateSend, this);
    lampAStatusSizer->Add(lampAStateBtn, 0, wxALL, 5);
    wxBoxSizer *lampAHoursSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampAHoursSizer, 0, wxALL | wxEXPAND, 0);
    lampAHoursSizer->Add(new wxStaticText(this, wxID_STATIC, _("Hours")), 0, wxALL, 5);
    lampAHoursSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("3"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    wxButton *lampAHoursBtn = new wxButton(this, myID_BTN_LAMPAHOURS_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampAHoursBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampAHoursSend, this);
    lampAHoursSizer->Add(lampAHoursBtn, 0, wxALL, 5);
    wxBoxSizer *lampALitCountSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampALitCountSizer, 0, wxALL | wxEXPAND, 0);
    lampALitCountSizer->Add(new wxStaticText(this, wxID_STATIC, _("Lit Count")), 0, wxALL, 5);
    lampALitCountSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("5"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    wxButton *lampALitCntBtn = new wxButton(this, myID_BTN_LAMPALITCNT_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampALitCntBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampALitCntSend, this);
    lampALitCountSizer->Add(lampALitCntBtn, 0, wxALL | wxEXPAND, 5);
    wxBoxSizer *lampATempCondSizer = new wxBoxSizer(wxHORIZONTAL);
    lampASizer->Add(lampATempCondSizer, 0, wxALL | wxEXPAND, 0);
    lampATempCondSizer->Add(new wxStaticText(this, wxID_STATIC, _("Temp Cond")), 0, wxALL, 5);
    wxArrayString lampTempCond;
    lampTempCond.push_back(wxT("SHUTDOWN"));
    lampTempCond.push_back(wxT("OUT_OF_SPEC"));
    lampTempCond.push_back(wxT("OUT_OF_SPEC"));
    lampTempCond.push_back(wxT("CRITICAL"));
    lampTempCond.push_back(wxT("OK"));
    lampTempCond.push_back(wxT("OK"));
    lampTempCond.push_back(wxT("OK"));
    lampTempCond.push_back(wxT("OK"));
    lampATempCondSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampTempCond), 1, wxALL, 5);
    wxButton *lampATempCondBtn = new wxButton(this, myID_BTN_LAMPATEMPCOND_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampATempCondBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampATempCondSend, this);
    lampATempCondSizer->Add(lampATempCondBtn, 0, wxALL, 5);
    /* lamp b */
    wxStaticBoxSizer *lampBSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lamp B Status"));
    lampSizer->Add(lampBSizer, 1, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 5);
    wxBoxSizer *lampBStatusSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBStatusSizer, 0, wxALL | wxEXPAND, 0);
    lampBStatusSizer->Add(new wxStaticText(this, wxID_STATIC, _("Status")), 0, wxALL, 5);
    lampBStatusSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampStatus), 1, wxALL, 5);
    wxButton *lampBStateBtn = new wxButton(this, myID_BTN_LAMPBSTATE_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampBStateBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampBStateSend, this);
    lampBStatusSizer->Add(lampBStateBtn, 0, wxALL, 5);
    wxBoxSizer *lampBHoursSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBHoursSizer, 0, wxALL | wxEXPAND, 0);
    lampBHoursSizer->Add(new wxStaticText(this, wxID_STATIC, _("Hours")), 0, wxALL, 5);
    lampBHoursSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("3"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    wxButton *lampBHoursBtn = new wxButton(this, myID_BTN_LAMPBHOURS_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampBHoursBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampBHoursSend, this);
    lampBHoursSizer->Add(lampBHoursBtn, 0, wxALL, 5);
    wxBoxSizer *lampBLitCountSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBLitCountSizer, 0, wxALL | wxEXPAND, 0);
    lampBLitCountSizer->Add(new wxStaticText(this, wxID_STATIC, _("Lit Count")), 0, wxALL, 5);
    lampBLitCountSizer->Add(new wxSpinCtrl(this, wxID_ANY, wxT("5"), wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000), 1, wxALL, 5);
    wxButton *lampBLitCntBtn = new wxButton(this, myID_BTN_LAMPBLITCNT_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampBLitCntBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampBLitCntSend, this);
    lampBLitCountSizer->Add(lampBLitCntBtn, 0, wxALL, 5);
    wxBoxSizer *lampBTempCondSizer = new wxBoxSizer(wxHORIZONTAL);
    lampBSizer->Add(lampBTempCondSizer, 0, wxALL | wxEXPAND, 0);
    lampBTempCondSizer->Add(new wxStaticText(this, wxID_STATIC, _("Temp Cond")), 0, wxALL, 5);
    lampBTempCondSizer->Add(new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, lampTempCond), 1, wxALL, 5);
    wxButton *lampBTempCondBtn = new wxButton(this, myID_BTN_LAMPBTEMPCOND_SEND, _("Send"),
        wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    lampBTempCondBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &TrapPane::OnLampBTempCondSend, this);
    lampBTempCondSizer->Add(lampBTempCondBtn, 0, wxALL, 5);

    SetSizerAndFit(allSizer);
}

void TrapPane::OnLedStatusChosen(wxCommandEvent &event)
{
    int mask, shift;
    wxWindowID id;
    wxString sqlUpdate;

    switch (event.GetId())
    {
    case myID_RB_LED_POWER:
        id = myID_LED_POWER;
        mask = 0x0FFF; shift = 12;
        break;
    case myID_RB_LED_FAN:
        id = myID_LED_FAN;
        mask = 0xF0FF; shift = 8;
        break;
    case myID_RB_LED_LAMPA:
        id = myID_LED_LAMPA;
        mask = 0xFF0F; shift = 4;
        break;
    case myID_RB_LED_LAMPB:
        id = myID_LED_LAMPB;
        mask = 0xFFF0; shift = 0;
        break;
    default:
        return;
    }

    _ledStatus &= mask;
    _ledStatus |= (event.GetSelection() << shift);

    /* update database */
    sqlUpdate << wxT("UPDATE TrapTbl SET CurrentValue = '") << _ledStatus
        << wxT("' WHERE ProtocolName = 'LEDSTATUS'");
    if (1 != _db->ExecuteUpdate(sqlUpdate))
    {
        wxLogError(_("Fail to update LEDSTATUS."));
    }

    /* update led preview */
    awxLed *preview = wxDynamicCast(FindWindow(id), awxLed);
    if (preview)
        preview->UpdateStateAndColors(event.GetSelection());

    /* update preset */
    wxChoice *preset = wxDynamicCast(FindWindow(myID_CHOICE_LED_PRESET), wxChoice);
    if (preset)
        preset->SetSelection(0);
}

void TrapPane::OnLedPresetChosen(wxCommandEvent &event)
{
    LedStatusPreset *obj = wxDynamicCast(event.GetEventObject(), LedStatusPreset);
    LedPresetData *refData = (LedPresetData *)obj->GetRefData();
    int select = event.GetSelection();
    wxString sqlUpdate;

    if ((select != 0) && (select != wxNOT_FOUND))
    {
        /* update radiobox */
        _ledStatus = refData->m_LedStatus.at(select);
        LedStatusRadioBox *power = wxDynamicCast(FindWindow(myID_RB_LED_POWER), LedStatusRadioBox);
        LedStatusRadioBox *fan = wxDynamicCast(FindWindow(myID_RB_LED_FAN), LedStatusRadioBox);
        LedStatusRadioBox *lampa = wxDynamicCast(FindWindow(myID_RB_LED_LAMPA), LedStatusRadioBox);
        LedStatusRadioBox *lampb = wxDynamicCast(FindWindow(myID_RB_LED_LAMPB), LedStatusRadioBox);
        power->SetSelection((_ledStatus >> 12) & 0x7);
        fan->SetSelection((_ledStatus >> 8) & 0x7);
        lampa->SetSelection((_ledStatus >> 4) & 0x7);
        lampb->SetSelection((_ledStatus >> 0) & 0x7);

        /* update preview */
        awxLed *powerPreview = wxDynamicCast(FindWindow(myID_LED_POWER), awxLed);
        awxLed *fanPreview = wxDynamicCast(FindWindow(myID_LED_FAN), awxLed);
        awxLed *lampaPreview = wxDynamicCast(FindWindow(myID_LED_LAMPA), awxLed);
        awxLed *lampbPreview = wxDynamicCast(FindWindow(myID_LED_LAMPB), awxLed);
        powerPreview->UpdateStateAndColors((_ledStatus >> 12) & 0x7);
        fanPreview->UpdateStateAndColors((_ledStatus >> 8) & 0x7);
        lampaPreview->UpdateStateAndColors((_ledStatus >> 4) & 0x7);
        lampbPreview->UpdateStateAndColors((_ledStatus >> 0) & 0x7);

        /* update database */
        sqlUpdate << wxT("UPDATE TrapTbl SET CurrentValue = '") << _ledStatus
            << wxT("' WHERE ProtocolName = 'LEDSTATUS'");
        if (1 != _db->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update LEDSTATUS."));
        }
    }
}

void TrapPane::OnLedStatusSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("led state"));
}

void TrapPane::OnLampAStateSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp A state"));
}

void TrapPane::OnLampBStateSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp B state"));
}

void TrapPane::OnLampAHoursSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp A hours"));
}

void TrapPane::OnLampBHoursSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp B hours"));
}

void TrapPane::OnLampALitCntSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp A lit cnt"));
}

void TrapPane::OnLampBLitCntSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp B lit cnt"));
}

void TrapPane::OnLampATempCondSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp A temp cond"));
}

void TrapPane::OnLampBTempCondSend(wxCommandEvent &event)
{
    wxLogMessage(wxT("Lamp B temp cond"));
}

