#include <wx/wx.h>
#include "SimCubeApp.h"
#include "ConfigPane.h"

enum
{
    ID_LANG_DEFAULT = wxID_HIGHEST + 100,
    ID_LANG_SELECT,
    ID_UDP_PORT,
};

BEGIN_EVENT_TABLE(ConfigPane, wxPanel)
    EVT_CHECKBOX(ID_LANG_DEFAULT, ConfigPane::OnLangDefault)
    EVT_UPDATE_UI(ID_LANG_SELECT, ConfigPane::OnUpdateLangSelect)
    EVT_LISTBOX(ID_LANG_SELECT, ConfigPane::OnLangSelect)
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
    _db = wxGetApp().GetMainDatabase();
    TransferFromDatabase();
}

void ConfigPane::CreateControls()
{
    wxSizer *allSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *descSizer = new wxBoxSizer(wxHORIZONTAL);
    allSizer->Add(descSizer, 0, wxALL | wxALIGN_CENTER | wxEXPAND, 5);
    descSizer->Add(new wxStaticText(this, wxID_STATIC,
        _("Important! All changed parameters will take effect at next boot!")),
        0, wxTOP | wxRIGHT | wxLEFT | wxALIGN_CENTER | wxEXPAND, 15);
    wxStaticBoxSizer *langSizer = new wxStaticBoxSizer(wxVERTICAL,
        this, _("Language "));
    allSizer->Add(langSizer, 0, wxALL | wxEXPAND, 15);
    wxCheckBox *cb = new wxCheckBox(this, ID_LANG_DEFAULT,
        _("Use system default language"));
    cb->SetValue(_language == eLANG_DEFAULT);
    langSizer->Add(cb, 0, wxALL, 5);
    wxArrayString langArray;
    langArray.push_back(_("English"));
    langArray.push_back(_("Chinese (Traditional)"));
    langArray.push_back(_("Chinese (Simplified)"));
    wxListBox *lb = new wxListBox(this, ID_LANG_SELECT, wxDefaultPosition,
        wxDefaultSize, langArray);
    switch (_language)
    {
    case eLANG_ENGLISH: lb->SetSelection(0); break;
    case eLANG_CHINESE_TRADITIONAL: lb->SetSelection(1); break;
    case eLANG_CHINESE_SIMPLIFIED: lb->SetSelection(2); break;
    }
    langSizer->Add(lb, 0, wxALL, 5);
    wxStaticBoxSizer *portSizer = new wxStaticBoxSizer(wxHORIZONTAL,
        this, _("Port "));
    allSizer->Add(portSizer, 0, wxRIGHT | wxLEFT | wxEXPAND, 15);
    portSizer->Add(new wxStaticText(this, wxID_STATIC,
        _("Which port will be used by UDP protocol? ")), 0,
        wxALL | wxEXPAND, 5);
    portSizer->Add(new wxTextCtrl(this, ID_UDP_PORT), 1,
        wxALL | wxEXPAND, 5);    

    SetSizerAndFit(allSizer);
}

// event handlers
void ConfigPane::OnLangDefault(wxCommandEvent &event)
{
    wxListBox *list = wxDynamicCast(FindWindow(ID_LANG_SELECT), wxListBox);

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

    TransferToDatabase();
}

void ConfigPane::OnUpdateLangSelect(wxUpdateUIEvent &event)
{
    wxCheckBox *chk = wxDynamicCast(FindWindow(ID_LANG_DEFAULT), wxCheckBox);
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

    TransferToDatabase();
}

// helper functions
void ConfigPane::TransferToDatabase()
{
    wxString sqlUpdate;

    /* language */
    sqlUpdate.clear();
    sqlUpdate << wxT("UPDATE CfgTbl SET ConfigValue = ");
    switch (_language)
    {
    case eLANG_ENGLISH: sqlUpdate << wxT("'English' "); break;
    case eLANG_CHINESE_TRADITIONAL: sqlUpdate << wxT("'Chinese Traditional' "); break;
    case eLANG_CHINESE_SIMPLIFIED: sqlUpdate << wxT("'Chinese Simplified' "); break;
    default: sqlUpdate << wxT("'Default' "); break;
    }
    sqlUpdate << wxT("WHERE ConfigName = 'Language'");
    if (1 != _db->ExecuteUpdate(sqlUpdate))
        wxLogError(_("Fail to update language setting!"));
}

void ConfigPane::TransferFromDatabase()
{
    wxString sqlQuery, value;
    wxSQLite3ResultSet set;

    /* language */
    sqlQuery << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'Language'");
    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
        value = set.GetAsString(0);
    set.Finalize();

    if (value == wxT("English"))
        _language = eLANG_ENGLISH;
    else if (value == wxT("Chinese Traditional"))
        _language = eLANG_CHINESE_TRADITIONAL;
    else if (value == wxT("Chinese Simplified"))
        _language = eLANG_CHINESE_SIMPLIFIED;
    else
        _language = eLANG_DEFAULT;
}
