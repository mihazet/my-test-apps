// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/wxsqlite3.h>
#include <wx/filedlg.h>
#include "WidgetId.h"
#include "UpdaterApp.h"
#include "MacAddrUsagePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/import_32.xpm"
#include "img/export_32.xpm"
#include "img/clear_32.xpm"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
enum
{
    MAC_USAGE_ID,
    MAC_USAGE_DATE,
    MAC_USAGE_TIME,
    MAC_USAGE_BOARDNAME,
    MAC_USAGE_OLDMAC,
    MAC_USAGE_NEWMAC,
    MAC_USAGE_OPERATOR,

    MAC_USAGE_MAX
};

class ReportDataModel : public wxDataViewVirtualListModel
{
public:
    ReportDataModel()
    {
        _db = new wxSQLite3Database();
        _db->Open(wxT(":memory:"));
    }

    ~ReportDataModel()
    {
        if (_db)
        {
            if (_db->IsOpen())
                _db->Close();
            delete _db;
        }
    }

    virtual unsigned int GetColumnCount() const
    {
        return MAC_USAGE_MAX;
    }

    virtual wxString GetColumnType(unsigned int col) const
    {
        wxString type;

        switch (col)
        {
        case MAC_USAGE_ID:
        default:
            type = wxT("string");
            break;
        }

        return type;
    }

    virtual unsigned int GetRowCount()
    {
        unsigned int count = 0;

        if (_db->TableExists(wxT("ReportTable")))
        {
            wxSQLite3ResultSet set = _db->ExecuteQuery(wxT("SELECT count(*) from ReportTable"));
            if (set.NextRow())
            {
                count = (unsigned int)set.GetInt(0);
            }
            set.Finalize();
        }

        return count;
    }

    virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
    {
        wxString sqlQuery = wxT("SELECT ");
        wxSQLite3ResultSet set;

        switch (col)
        {
        default:
        case MAC_USAGE_ID: sqlQuery << wxT("Id"); break;
        case MAC_USAGE_DATE: sqlQuery << wxT("Date"); break;
        case MAC_USAGE_TIME: sqlQuery << wxT("Time"); break;
        case MAC_USAGE_BOARDNAME: sqlQuery << wxT("BoardName"); break;
        case MAC_USAGE_OLDMAC: sqlQuery << wxT("OldMACAddress"); break;
        case MAC_USAGE_NEWMAC: sqlQuery << wxT("NewMACAddress"); break;
        case MAC_USAGE_OPERATOR: sqlQuery << wxT("Operator"); break;
        }
        sqlQuery << wxT(" FROM ReportTable WHERE Id = ") << row + 1;

        set = _db->ExecuteQuery(sqlQuery);
        if (set.NextRow())
            variant = set.GetAsString(0);
        set.Finalize();
    }

    virtual bool SetValueByRow(const wxVariant &WXUNUSED(variant), unsigned int WXUNUSED(row), unsigned int WXUNUSED(col))
    {
        return false;
    }

    wxSQLite3Database *GetDB() { return _db; }

private:
    wxSQLite3Database *_db;
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
MacAddrUsagePane::MacAddrUsagePane()
{
    Init();
}

MacAddrUsagePane::MacAddrUsagePane(wxWindow *parent, wxWindowID id,
                                   const wxPoint &pos, const wxSize &size,
                                   long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

MacAddrUsagePane::~MacAddrUsagePane()
{

}

void MacAddrUsagePane::Init()
{
    _reportModel = new ReportDataModel();
}

bool MacAddrUsagePane::Create(wxWindow *parent, wxWindowID id,
                              const wxPoint &pos, const wxSize &size,
                              long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void MacAddrUsagePane::CreateControls()
{
    /* report view */
    _reportView = new wxDataViewCtrl(this, wxID_ANY);
    _reportView->AssociateModel(_reportModel);
    _reportView->AppendTextColumn(wxT("#"), MAC_USAGE_ID, wxDATAVIEW_CELL_INERT, 40, wxALIGN_CENTER);
    _reportView->AppendTextColumn(_("Date"), MAC_USAGE_DATE, wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT);
    _reportView->AppendTextColumn(_("Time"), MAC_USAGE_TIME, wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT);
    _reportView->AppendTextColumn(_("BoardName"), MAC_USAGE_BOARDNAME, wxDATAVIEW_CELL_INERT, 120, wxALIGN_LEFT);
    _reportView->AppendTextColumn(_("Old MAC Address"), MAC_USAGE_OLDMAC, wxDATAVIEW_CELL_INERT, 120, wxALIGN_CENTER);
    _reportView->AppendTextColumn(_("New MAC Address"), MAC_USAGE_NEWMAC, wxDATAVIEW_CELL_INERT, 120, wxALIGN_CENTER);
    _reportView->AppendTextColumn(_("Operator"), MAC_USAGE_OPERATOR, wxDATAVIEW_CELL_INERT, 300, wxALIGN_LEFT);

    /* report operation */
    wxButton *importBtn = new wxButton(this, wxID_ANY, _("Import"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    importBtn->SetBitmap(wxBitmap(import_32_xpm));
    importBtn->SetBitmapDisabled(wxBitmap(wxImage(import_32_xpm).ConvertToGreyscale()));
    importBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUsagePane::OnReportImport, this);
    wxButton *exportBtn = new wxButton(this, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    exportBtn->SetBitmap(wxBitmap(export_32_xpm));
    exportBtn->SetBitmapDisabled(wxBitmap(wxImage(export_32_xpm).ConvertToGreyscale()));
    exportBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUsagePane::OnReportExport, this);
    wxButton *clearBtn = new wxButton(this, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    clearBtn->SetBitmap(wxBitmap(clear_32_xpm));
    clearBtn->SetBitmapDisabled(wxBitmap(wxImage(clear_32_xpm).ConvertToGreyscale()));
    clearBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUsagePane::OnReportClear, this);
    wxBoxSizer *opSizer = new wxBoxSizer(wxHORIZONTAL);
    opSizer->Add(importBtn, 0, wxALL, 5);
    opSizer->Add(exportBtn, 0, wxALL, 5);
    opSizer->Add(clearBtn, 0, wxALL, 5);
    
    wxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    paneSizer->Add(opSizer, 0, wxALL | wxEXPAND, 5);
    paneSizer->Add(_reportView, 1, wxALL | wxEXPAND, 10);
    SetSizerAndFit(paneSizer);
}

// event handlers
void MacAddrUsagePane::OnReportImport(wxCommandEvent &WXUNUSED(event))
{
    wxString dir = wxGetApp().m_pAppOptions->GetOption(wxT("ReportFolder"));
    wxString wildcard = _("MAC address report file") + wxT(" (*.db)|*.db");
    wxFileDialog fileDlg(this, _("Import file"), dir, wxEmptyString,
        wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    wxSQLite3Database db;

    if (wxID_OK == fileDlg.ShowModal())
    {
        _reportModel->GetDB()->Restore(fileDlg.GetPath());
        _reportModel->Reset(_reportModel->GetRowCount());
    }    
}

void MacAddrUsagePane::OnReportExport(wxCommandEvent &WXUNUSED(event))
{

}

void MacAddrUsagePane::OnReportClear(wxCommandEvent &WXUNUSED(event))
{
    wxSQLite3Database *pDB = _reportModel->GetDB();
    wxString sql = wxT("DROP TABLE IF EXISTS ReportTable");

    if (pDB && pDB->IsOpen())
    {
        pDB->ExecuteUpdate(sql);
        _reportModel->Reset(0);
    }
}
