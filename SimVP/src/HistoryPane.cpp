#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/wxsqlite3.h>
#include <wx/filename.h>
#include "SimCubeApp.h"
#include "HistoryPane.h"

#include "img/save_16.xpm"
#include "img/delete_16.xpm"
#include "img/flag_16.xpm"
#include "img/dictionary_16.xpm"

enum
{
    myID_HISTORY_VIEW = wxID_HIGHEST + 1,
    myID_SEARCH_HISTORY,
    myID_BTN_SAVE_HISTORY,
    myID_BTN_DELETE_HISTORY,
    myID_BTN_RESOLVE_ADDRESS,
    myID_BTN_AUTOSCROLL,
};

BEGIN_EVENT_TABLE(HistoryPane, wxPanel)
    EVT_BUTTON(myID_BTN_SAVE_HISTORY, HistoryPane::OnSaveHistory)
    EVT_BUTTON(myID_BTN_DELETE_HISTORY, HistoryPane::OnDeleteHistory)
    EVT_BUTTON(myID_BTN_RESOLVE_ADDRESS, HistoryPane::OnResolveAddress)
    EVT_BUTTON(myID_BTN_AUTOSCROLL, HistoryPane::OnAutoscroll)
END_EVENT_TABLE()

HistoryPane::HistoryPane()
{
    Init();
}

HistoryPane::HistoryPane(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

HistoryPane::~HistoryPane()
{
    /* auto save */
    wxSQLite3Database *memDB = wxGetApp().GetHistoryDatabase();
    if (memDB && memDB->IsOpen() && memDB->HasBackupSupport()
        && (memDB->GetLastRowId() != 0) && IsAutoSaveEnabled())
    {
        wxDateTime now = wxDateTime::Now();
        wxString target = wxT("autosave");
        if (!wxFileName::DirExists(target))
            wxFileName::Mkdir(target);
        target << wxFileName::GetPathSeparator() << now.Format(wxT("%y%m%d-%H%M%S")) << wxT(".db");
        memDB->Backup(target);
    }

    wxDELETE(_autoScrollBtnImg);
    wxDELETE(_resolveAddrBtnImg);
}

bool HistoryPane::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void HistoryPane::Init()
{
    _historyAutoScroll = false;
    _autoScrollBtnImg = new wxImage(flag_16_xpm);
    _resolveAddrBtnImg = new wxImage(dictionary_16_xpm);
    /* install an update hook for memory database */
    wxGetApp().GetHistoryDatabase()->SetUpdateHook(this);
}

void HistoryPane::CreateControls()
{
    wxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    /* history data view */
    _historyView = new wxDataViewCtrl(this, myID_HISTORY_VIEW);
    paneSizer->Add(_historyView, 1, wxALL | wxEXPAND, 5);
    _historyView->AssociateModel(wxGetApp().m_HistoryData);
    _historyView->AppendTextColumn(wxT("#"), HISTORY_DATA_ID, wxDATAVIEW_CELL_INERT, 30, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Timestamp"), HISTORY_DATA_TIMESTAMP, wxDATAVIEW_CELL_INERT, 125, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Direction"), HISTORY_DATA_DIRECTION, wxDATAVIEW_CELL_INERT, 60, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Remote"), HISTORY_DATA_REMOTE, wxDATAVIEW_CELL_INERT, 95, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Port"), HISTORY_DATA_PORT, wxDATAVIEW_CELL_INERT, 45, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Length"), HISTORY_DATA_LENGTH, wxDATAVIEW_CELL_INERT, 55, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Message"), HISTORY_DATA_MESSAGE, wxDATAVIEW_CELL_INERT, 360);
    /* history data ctrl */
    wxSizer *ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
    paneSizer->Add(ctrlSizer, 0, wxALL | wxEXPAND, 0);
    wxSearchCtrl *search = new wxSearchCtrl(this, myID_SEARCH_HISTORY);
    search->ShowCancelButton(true);
    ctrlSizer->Add(search, 1, wxALL|wxALIGN_CENTER, 5);
    ctrlSizer->AddStretchSpacer();
    wxBitmapButton *save = new wxBitmapButton(this, myID_BTN_SAVE_HISTORY, wxBitmap(save_16_xpm));
    save->SetToolTip(_("Save history"));
    wxBitmapButton *del = new wxBitmapButton(this, myID_BTN_DELETE_HISTORY, wxBitmap(delete_16_xpm));
    del->SetToolTip(_("Delete history"));
    wxBitmapButton *resolve = new wxBitmapButton(this, myID_BTN_RESOLVE_ADDRESS,
        wxGetApp().m_HistoryData->IsAddressResolved()
            ? wxBitmap(*_resolveAddrBtnImg) : wxBitmap(_resolveAddrBtnImg->ConvertToGreyscale()));
    resolve->SetToolTip(wxGetApp().m_HistoryData->IsAddressResolved()
        ? _("Address is resolved")
        : _("Address is not resolved"));
    wxBitmapButton *scroll = new wxBitmapButton(this, myID_BTN_AUTOSCROLL,
        _historyAutoScroll ? wxBitmap(*_autoScrollBtnImg) : wxBitmap(_autoScrollBtnImg->ConvertToGreyscale()));
    scroll->SetToolTip(_historyAutoScroll ? _("Autoscroll is enabled") : _("Autoscroll is disabled"));
    ctrlSizer->Add(save, 0, wxALL|wxALIGN_CENTER, 0);
    ctrlSizer->Add(del, 0, wxALL|wxALIGN_CENTER, 0);
    ctrlSizer->Add(resolve, 0, wxLEFT|wxALIGN_CENTER, 5);
    ctrlSizer->Add(scroll, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER, 5);

    SetSizerAndFit(paneSizer);
}

void HistoryPane::UpdateCallback(wxUpdateType type, const wxString &database,
                                 const wxString &table, wxLongLong rowid)
{
    if (_historyAutoScroll && (type == SQLITE_INSERT)
        && (database == wxT("main")) && (table == wxT("HistoryTbl")))
    {
        wxDataViewItem item = wxGetApp().m_HistoryData->GetItem(rowid.ToLong());
        _historyView->EnsureVisible(item);
    }
}

void HistoryPane::OnSaveHistory(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dlg(this, _("Save history to"), wxT("."), wxEmptyString,
        wxT("SQLite3 Database files(*.db;*.db3)|*.db;*.db3"),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

    dlg.CenterOnParent();
    if (wxID_OK == dlg.ShowModal())
    {
        wxSQLite3Database *db = wxGetApp().GetHistoryDatabase();
        if (db && db->IsOpen() && db->HasBackupSupport())
            db->Backup(dlg.GetPath());
        else
        {
            if (db)
            {
                if (!db->IsOpen())
                {
                    wxLogError(_("Sorry! History database is closed!"));
                }
                if (!db->HasBackupSupport())
                {
                    wxLogError(_("Sorry! History database doesn't support backup feature!"));
                }
            }
        }
    }
}

void HistoryPane::OnDeleteHistory(wxCommandEvent &WXUNUSED(event))
{
    wxSQLite3Database *db = wxGetApp().GetHistoryDatabase();

    if (db && db->IsOpen())
    {
        if (0 != db->ExecuteUpdate(wxT("DELETE FROM HistoryTbl")))
        {
            HistoryDataModel *data = wxGetApp().m_HistoryData;
            data->Reset(0);
        }
    }
}

void HistoryPane::OnResolveAddress(wxCommandEvent &event)
{
    wxBitmapButton *btn = wxDynamicCast(event.GetEventObject(), wxBitmapButton);
    btn->SetToolTip(wxGetApp().m_HistoryData->IsAddressResolved()
        ? _("Address is not resolved") : _("Address is resolved"));
    btn->SetBitmap(wxGetApp().m_HistoryData->IsAddressResolved()
        ? _resolveAddrBtnImg->ConvertToGreyscale() : *_resolveAddrBtnImg);
    wxGetApp().m_HistoryData->ToggleAddressResolve();
    /* force specific column in the wxDVC to update */
    wxGetApp().m_HistoryData->ValueChanged(wxDataViewItem(), HISTORY_DATA_REMOTE);
}

void HistoryPane::OnAutoscroll(wxCommandEvent &event)
{
    wxBitmapButton *btn = wxDynamicCast(event.GetEventObject(), wxBitmapButton);
    btn->SetToolTip(_historyAutoScroll ? _("Autoscroll is disabled") : _("Autoscroll is enabled"));
    btn->SetBitmap(_historyAutoScroll ? _autoScrollBtnImg->ConvertToGreyscale() : *_autoScrollBtnImg);
    _historyAutoScroll = _historyAutoScroll ? false : true;
}

bool HistoryPane::IsAutoSaveEnabled()
{
    bool result = false;
    wxSQLite3Database *db = wxGetApp().GetConfigDatabase();
    wxSQLite3ResultSet set;
    wxString sql, value;

    sql << wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'AutoSaveHistory'");
    set = db->ExecuteQuery(sql);
    if (set.NextRow())
        value = set.GetAsString(0);
    set.Finalize();

    if ((value == wxT("True")) || (value == wxT("Yes")))
        result = true;

    return result;
}

////////////////////////////////////////////////////////////////////////////
HistoryDataModel::HistoryDataModel(wxSQLite3Database *db)
    : wxDataViewVirtualListModel(), _db(db)
{
    wxString sqlUpdate;
    int modified_rows;

    /* create table */
    sqlUpdate << wxT("CREATE TABLE HistoryTbl (Id INTEGER PRIMARY KEY, ")
                                        << wxT("Timestamp TEXT, ")
                                        << wxT("Direction TEXT, ")
                                        << wxT("Ip TEXT, ")
                                        << wxT("Host TEXT, ")
                                        << wxT("Port TEXT, ")
                                        << wxT("Length TEXT, ")
                                        << wxT("Message TEXT)");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);

    /* create trigger */
    sqlUpdate.clear();
    sqlUpdate << wxT("CREATE TRIGGER insert_timestamp AFTER INSERT ON HistoryTbl ")
              << wxT("BEGIN ")
              << wxT("UPDATE HistoryTbl SET Timestamp = DATETIME('NOW', 'LOCALTIME') WHERE rowid = new.rowid;")
              << wxT("END");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);

    /* init other private variables */
    _resolveAddress = false;
}

void HistoryDataModel::GetValueByRow(wxVariant &variant,
                                     unsigned int row,
                                     unsigned int col) const
{
    wxString sqlQuery;
    wxSQLite3ResultSet set;

    sqlQuery << wxT("SELECT ");
    switch (col)
    {
        case HISTORY_DATA_ID: sqlQuery << wxT("Id"); break;
        case HISTORY_DATA_TIMESTAMP: sqlQuery << wxT("Timestamp"); break;
        case HISTORY_DATA_DIRECTION: sqlQuery << wxT("Direction"); break;
        case HISTORY_DATA_REMOTE:
            if (_resolveAddress)
                sqlQuery << wxT("Host");
            else
                sqlQuery << wxT("Ip");
            break;
        case HISTORY_DATA_PORT: sqlQuery << wxT("Port"); break;
        case HISTORY_DATA_LENGTH: sqlQuery << wxT("Length"); break;
        case HISTORY_DATA_MESSAGE: sqlQuery << wxT("Message"); break;
    }
    sqlQuery << wxT(" FROM HistoryTbl WHERE Id = ") << row + 1;

    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
    {
        variant = set.GetAsString(0);
    }
    set.Finalize();
}

bool HistoryDataModel::SetValueByRow(const wxVariant &WXUNUSED(variant),
                                     unsigned int WXUNUSED(row),
                                     unsigned int WXUNUSED(col))
{
    return false;
}

bool HistoryDataModel::AddData(const HistoryData &data)
{
    wxString sqlUpdate;
    bool result = true;

    sqlUpdate << wxT("INSERT INTO HistoryTbl (Direction, Ip, Host, Port, Length, Message) VALUES ('")
        << data.m_direction << wxT("', '") << data.m_ip << wxT("', '")
        << data.m_host << wxT("', '") << data.m_port << wxT("', '")
        << data.m_len << wxT("', '") << data.m_msg << wxT("')");

    if (1 != _db->ExecuteUpdate(sqlUpdate))
    {
        result = false;
        wxLogError(_("Fail to insert new value into history table."));
    }
    else
        RowAppended();

    return result;
}

