#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/wxsqlite3.h>
#include "SimCubeApp.h"
#include "HistoryPane.h"

#include "img/save_16.xpm"
#include "img/delete_16.xpm"
#include "img/flag_16.xpm"

enum
{
    myID_HISTORY_VIEW = wxID_HIGHEST + 1,
    myID_SEARCH_HISTORY,
    myID_BTN_SAVE_HISTORY,
    myID_BTN_DELETE_HISTORY,
    myID_BTN_AUTOSCROLL,
};

BEGIN_EVENT_TABLE(HistoryPane, wxPanel)
    EVT_BUTTON(myID_BTN_SAVE_HISTORY, HistoryPane::OnSaveHistory)
    EVT_BUTTON(myID_BTN_DELETE_HISTORY, HistoryPane::OnDeleteHistory)
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
    delete _autoScrollBtnImg;
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
    /* install an update hook for memory database */
    wxGetApp().GetMemDatabase()->SetUpdateHook(this);
}

void HistoryPane::CreateControls()
{
    wxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    /* history data view */
    _historyView = new wxDataViewCtrl(this, myID_HISTORY_VIEW);
    paneSizer->Add(_historyView, 1, wxALL | wxEXPAND, 5);
    _historyView->AssociateModel(wxGetApp().m_HistoryData);
    _historyView->AppendTextColumn(wxT("#"), 0, wxDATAVIEW_CELL_INERT, 30, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Timestamp"), 1, wxDATAVIEW_CELL_INERT, 125);
    _historyView->AppendTextColumn(_("IP Address"), 2, wxDATAVIEW_CELL_INERT, 95);
    _historyView->AppendTextColumn(_("Port"), 3, wxDATAVIEW_CELL_INERT, 45, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Length"), 4, wxDATAVIEW_CELL_INERT, 55, wxALIGN_CENTER);
    _historyView->AppendTextColumn(_("Message"), 5, wxDATAVIEW_CELL_INERT, 360);
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
    wxBitmapButton *scroll = new wxBitmapButton(this, myID_BTN_AUTOSCROLL,
        _historyAutoScroll ? wxBitmap(*_autoScrollBtnImg) : wxBitmap(_autoScrollBtnImg->ConvertToGreyscale()));
    scroll->SetToolTip(_historyAutoScroll ? _("Autoscroll is enabled") : _("Autoscroll is disabled"));
    ctrlSizer->Add(save, 0, wxALL|wxALIGN_CENTER, 0);
    ctrlSizer->Add(del, 0, wxALL|wxALIGN_CENTER, 0);
    ctrlSizer->Add(scroll, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER, 5);
    
    SetSizerAndFit(paneSizer);
}

void HistoryPane::UpdateCallback(wxUpdateType type, const wxString &database,
                                 const wxString &table, wxLongLong rowid)
{
    if (_historyAutoScroll && (type == SQLITE_INSERT) 
        && (database == wxT("main")) && (table == wxT("HistoryTbl")))
    {
        //HistoryDataModel *data = wxGetApp().m_HistoryData;
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
        wxSQLite3Database *db = wxGetApp().GetMemDatabase();
        if (db && db->IsOpen() && db->HasBackupSupport())
            db->Backup(dlg.GetPath());
        else
        {
            if (db)
            {
                if (!db->IsOpen())
                    wxLogError(_("Sorry! History database is closed!"));
                if (!db->HasBackupSupport())
                    wxLogError(_("Sorry! History database doesn't support backup feature!"));
            }
        }
    }
}

void HistoryPane::OnDeleteHistory(wxCommandEvent &WXUNUSED(event))
{
    wxSQLite3Database *db = wxGetApp().GetMemDatabase();

    if (db && db->IsOpen())
    {
        if (0 != db->ExecuteUpdate(wxT("DELETE FROM HistoryTbl")))
        {
            HistoryDataModel *data = wxGetApp().m_HistoryData;
            data->Reset(0);
        }
    }
}

void HistoryPane::OnAutoscroll(wxCommandEvent &event)
{
    wxBitmapButton *btn = wxDynamicCast(event.GetEventObject(), wxBitmapButton);
    btn->SetToolTip(_historyAutoScroll ? _("Autoscroll is disabled") : _("Autoscroll is enabled"));
    btn->SetBitmap(_historyAutoScroll ? _autoScrollBtnImg->ConvertToGreyscale() : *_autoScrollBtnImg);
    _historyAutoScroll = _historyAutoScroll ? false : true;
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
                                        << wxT("Ip TEXT, ")
                                        << wxT("Port TEXT, ")
                                        << wxT("Length TEXT, ")
                                        << wxT("Data TEXT)");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);

    /* create trigger */
    sqlUpdate.clear();
    sqlUpdate << wxT("CREATE TRIGGER insert_timestamp AFTER INSERT ON HistoryTbl ")
              << wxT("BEGIN ")
              << wxT("UPDATE HistoryTbl SET Timestamp = DATETIME('NOW', 'LOCALTIME') WHERE rowid = new.rowid;")
              << wxT("END");
    modified_rows = _db->ExecuteUpdate(sqlUpdate);
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
        case 0: sqlQuery << wxT("Id"); break;
        case 1: sqlQuery << wxT("Timestamp"); break;
        case 2: sqlQuery << wxT("Ip"); break;
        case 3: sqlQuery << wxT("Port"); break;
        case 4: sqlQuery << wxT("Length"); break;
        case 5: sqlQuery << wxT("Data"); break;
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

bool HistoryDataModel::AddData(const wxString &ip, unsigned short port, 
                               const wxString &data, size_t len)
{
    wxString sqlUpdate;
    bool result = true;

    sqlUpdate << wxT("INSERT INTO HistoryTbl (Ip, Port, Data, Length) VALUES ('")
        << ip << wxString::Format(wxT("', '%u', '"), port) << data
        << wxString::Format(wxT("', '%ld')"), len);

    if (1 != _db->ExecuteUpdate(sqlUpdate))
    {
        result = false;
        wxLogError(_("Fail to insert new value into history table."));
    }
    else
        RowAppended();

    return result;
}

