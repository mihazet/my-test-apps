#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "SimCubeApp.h"
#include "ConfigPane.h"
#include "WidgetId.h"

enum
{
    update_language,
    update_autosave,
    update_rockey,
    update_connection,
    update_hours,
};

BEGIN_EVENT_TABLE(ConfigPane, wxPanel)
    EVT_CHECKBOX(myID_LANG_DEFAULT, ConfigPane::OnLangDefault)
    EVT_UPDATE_UI(myID_LANG_SELECT, ConfigPane::OnUpdateLangSelect)
    EVT_LISTBOX(myID_LANG_SELECT, ConfigPane::OnLangSelect)
    EVT_CHECKBOX(myID_OPT_AUTOSAVE_HISTORY, ConfigPane::OnAutoSaveHistory)
    EVT_CHECKBOX(myID_OPT_USING_ROCKEY4ND, ConfigPane::OnUsingRockey4ND)
END_EVENT_TABLE()

ConfigPane::ConfigPane()
{
    Init();
}

ConfigPane::ConfigPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

ConfigPane::~ConfigPane()
{

}

bool ConfigPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void ConfigPane::Init()
{
    _cfgDB = wxGetApp().GetConfigDatabase();
    TransferFromDatabase();
}

void ConfigPane::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    /* description */
    wxBoxSizer *descSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(descSizer, 0, wxALL | wxALIGN_CENTER | wxEXPAND, 5);
    descSizer->Add(new wxStaticText(this, wxID_STATIC,
        _("Important! All changed parameters will take effect at next boot!")),
        0, wxTOP | wxRIGHT | wxLEFT | wxALIGN_CENTER | wxEXPAND, 15);
    /* language */
    wxStaticBoxSizer *langSizer = new wxStaticBoxSizer(wxVERTICAL,
        this, _("Language "));
    allSizer->Add(langSizer, 0, wxALL | wxEXPAND, 15);
    wxCheckBox *cb = new wxCheckBox(this, myID_LANG_DEFAULT,
        _("Use system default language"));
    cb->SetValue(_language == eLANG_DEFAULT);
    langSizer->Add(cb, 0, wxALL, 5);
    wxArrayString langArray;
    langArray.push_back(_("English"));
    langArray.push_back(_("Chinese (Traditional)"));
    langArray.push_back(_("Chinese (Simplified)"));
    wxListBox *lb = new wxListBox(this, myID_LANG_SELECT, wxDefaultPosition,
        wxDefaultSize, langArray);
    switch (_language)
    {
    case eLANG_ENGLISH: lb->SetSelection(0); break;
    case eLANG_CHINESE_TRADITIONAL: lb->SetSelection(1); break;
    case eLANG_CHINESE_SIMPLIFIED: lb->SetSelection(2); break;
    default: break;
    }
    langSizer->Add(lb, 0, wxALL, 5);
    /* auto save history */
    wxStaticBoxSizer *optSizer = new wxStaticBoxSizer(wxVERTICAL,
        this, _("Options "));
    allSizer->Add(optSizer, 0, wxRIGHT | wxLEFT | wxBOTTOM | wxEXPAND, 15);
    cb = new wxCheckBox(this, myID_OPT_AUTOSAVE_HISTORY,
        _("When application terminates, auto save the transaction history."));
    cb->SetValue(_autoSave);
    optSizer->Add(cb, 0, wxALL | wxEXPAND, 5);
    cb = new wxCheckBox(this, myID_OPT_USING_ROCKEY4ND,
        _("Use ROCKEY4 (evaluate version only) to protect application."));
    cb->SetValue(_rockey);
    optSizer->Add(cb, 0, wxALL | wxEXPAND, 5);
    wxBoxSizer *connectionSizer = new wxBoxSizer(wxHORIZONTAL);
    optSizer->Add(connectionSizer, 1, wxLEFT, 5);
    connectionSizer->Add(new wxStaticText(this, wxID_STATIC,
        _("Maximum connection with remote (0: no limit) ")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    wxSpinCtrl *connection = new wxSpinCtrl(this, myID_OPT_MAX_CONNECTION, wxEmptyString,
        wxDefaultPosition, wxSize(40, -1), wxSP_ARROW_KEYS|wxSP_WRAP|wxALIGN_LEFT, 0, 10);
    connection->SetValue(_connections);
    connectionSizer->Add(connection, 0, wxALL|wxEXPAND, 0);
    wxBoxSizer *hoursSizer = new wxBoxSizer(wxHORIZONTAL);
    optSizer->Add(hoursSizer, 1, wxLEFT, 5);
    hoursSizer->Add(new wxStaticText(this, wxID_STATIC,
        _("Simulate lamp hour increase (0: don't increase. Unit: minutes) ")),
        0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    wxSpinCtrl *hours = new wxSpinCtrl(this, myID_OPT_HOURS_INTERVAL, wxEmptyString,
        wxDefaultPosition, wxSize(40, -1), wxSP_ARROW_KEYS|wxSP_WRAP|wxALIGN_LEFT, 0, 60);
    hours->SetValue(_hoursInterval);
    hoursSizer->Add(hours, 0, wxALL|wxEXPAND, 0);

    SetSizerAndFit(allSizer);
}

// event handlers
void ConfigPane::OnLangDefault(wxCommandEvent &event)
{
    wxListBox *list = wxDynamicCast(FindWindow(myID_LANG_SELECT), wxListBox);

    if (event.IsChecked())
        _language = eLANG_DEFAULT;
    else
    {
        switch (list->GetSelection())
        {
        case 0: _language = eLANG_ENGLISH; break;
        case 1: _language = eLANG_CHINESE_TRADITIONAL; break;
        case 2: _language = eLANG_CHINESE_SIMPLIFIED; break;
        default: _language = eLANG_DEFAULT; break;
        }
    }

    TransferToDatabase(update_language);
}

void ConfigPane::OnUpdateLangSelect(wxUpdateUIEvent &event)
{
    wxCheckBox *chk = wxDynamicCast(FindWindow(myID_LANG_DEFAULT), wxCheckBox);
    event.Enable(chk && !chk->IsChecked());
}

void ConfigPane::OnLangSelect(wxCommandEvent &event)
{
    switch (event.GetSelection())
    {
    case 0: _language = eLANG_ENGLISH; break;
    case 1: _language = eLANG_CHINESE_TRADITIONAL; break;
    case 2: _language = eLANG_CHINESE_SIMPLIFIED; break;
    default: _language = eLANG_DEFAULT; break;
    }

    TransferToDatabase(update_language);
}

void ConfigPane::OnAutoSaveHistory(wxCommandEvent &event)
{
    _autoSave = event.IsChecked();
    TransferToDatabase(update_autosave);
}

void ConfigPane::OnUsingRockey4ND(wxCommandEvent &event)
{
    _rockey = event.IsChecked();
    TransferToDatabase(update_rockey);
}

// helper functions
void ConfigPane::TransferToDatabase(int item)
{
    wxString sqlUpdate;

    switch (item)
    {
    case update_language:
        sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ");
        switch (_language)
        {
        case eLANG_ENGLISH: sqlUpdate << wxT("'English' "); break;
        case eLANG_CHINESE_TRADITIONAL: sqlUpdate << wxT("'TraditionalChinese' "); break;
        case eLANG_CHINESE_SIMPLIFIED: sqlUpdate << wxT("'SimplifiedChinese' "); break;
        default: sqlUpdate << wxT("'Default' "); break;
        }
        sqlUpdate << wxT("WHERE ConfigName = 'Language'");
        if (1 != _cfgDB->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update language setting!"));
        }
        break;

    case update_autosave:
        sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ");
        if (_autoSave)
        {
            sqlUpdate << wxT("'True' ");
        }
        else
        {
            sqlUpdate << wxT("'False' ");
        }
        sqlUpdate << wxT("WHERE ConfigName = 'AutoSaveHistory'");
        if (1 != _cfgDB->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update auto save history setting!"));
        }
        break;

    case update_rockey:
        sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ");
        if (_rockey)
            sqlUpdate << wxT("'True' ");
        else
            sqlUpdate << wxT("'False' ");
        sqlUpdate << wxT("WHERE ConfigName = 'UsingRockey4ND'");
        if (1 != _cfgDB->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update using rockey4nd setting!"));
        }
        break;

    case update_connection:
        sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ") << _connections
            << wxT(" WHERE ConfigName = 'MaxConnection'");
        if (1 != _cfgDB->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update maximun connection setting!"));
        }
        break;

    case update_hours:
        sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ") << _hoursInterval
            << wxT(" WHERE ConfigName = 'LampHoursIncreaseInterval'");
        if (1 != _cfgDB->ExecuteUpdate(sqlUpdate))
        {
            wxLogError(_("Fail to update lamp hours increase interval setting!"));
        }
        break;

    default:
        break;
    }
}

void ConfigPane::TransferFromDatabase()
{
    wxString sqlQuery, value;
    wxSQLite3ResultSet set;

    /* language */
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Language'");
    set = _cfgDB->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        value = set.GetAsString(0);
    set.Finalize();

    if (value == wxT("English"))
        _language = eLANG_ENGLISH;
    else if (value == wxT("TraditionalChinese"))
        _language = eLANG_CHINESE_TRADITIONAL;
    else if (value == wxT("SimplifiedChinese"))
        _language = eLANG_CHINESE_SIMPLIFIED;
    else
        _language = eLANG_DEFAULT;

    /* auto save */
    sqlQuery.clear();
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'AutoSaveHistory'");
    set = _cfgDB->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        value = set.GetAsString(0);
    set.Finalize();

    if ((value == wxT("True")) || (value == wxT("Yes")))
        _autoSave = true;
    else if ((value == wxT("False")) || (value == wxT("No")))
        _autoSave = false;
    else
        _autoSave = false;

    /* using rockey4nd */
    sqlQuery.clear();
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'UsingRockey4ND'");
    set = _cfgDB->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        value = set.GetAsString(0);
    set.Finalize();

    if ((value == wxT("True")) || (value == wxT("Yes")))
        _rockey = true;
    else if ((value == wxT("False")) || (value == wxT("No")))
        _rockey = false;
    else
        _rockey = false;

    /* max connection */
    sqlQuery.clear();
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'MaxConnection'");
    set = _cfgDB->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        _connections = set.GetInt(0);
    else
        _connections = 0;
    set.Finalize();
    
    /* lamp housr increase interval */
    sqlQuery.clear();
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'LampHoursIncreaseInterval'");
    set = _cfgDB->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        _hoursInterval = set.GetInt(0);
    else
        _hoursInterval = 0;
    set.Finalize();
}

