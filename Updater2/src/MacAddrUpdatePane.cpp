// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include "UpdaterApp.h"
#include "UpdateThread.h"
#include "NetAddrTextCtrl.h"
#include "MacAddrUpdatePane.h"
#include "WidgetId.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/result-unknown.xpm"
#include "img/result-pass.xpm"
#include "img/result-fail.xpm"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
#define CLOSE_COUNTDOWN     10
const wxBitmap MacAddrUpdatePane::_resultUnknown =  wxBitmap(result_unknown_xpm);
const wxBitmap MacAddrUpdatePane::_resultPass = wxBitmap(result_pass_xpm);
const wxBitmap MacAddrUpdatePane::_resultFail = wxBitmap(result_fail_xpm);

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MacAddrUpdatePane, wxPanel)
    EVT_TIMER(wxID_ANY, MacAddrUpdatePane::OnTimer)
END_EVENT_TABLE()

MacAddrUpdatePane::MacAddrUpdatePane()
{
    Init();
}

MacAddrUpdatePane::MacAddrUpdatePane(wxWindow *parent, wxWindowID id,
                                     const wxString &codeString, const wxPoint &pos,
                                     const wxSize &size, long style)
{
    Init();
    Create(parent, id, codeString, pos, size, style);
}

MacAddrUpdatePane::~MacAddrUpdatePane()
{
    if (_closeSelfTimer)
    {
        if (_closeSelfTimer->IsRunning())
            _closeSelfTimer->Stop();
        delete _closeSelfTimer;
    }
}

void MacAddrUpdatePane::Init()
{
    _codeString = wxEmptyString;
    _name = wxEmptyString;
    _ip = wxEmptyString;
    _mac = wxEmptyString;
    _row = -1;
    _resultBitmap = NULL;
    _multiFunctionBtn = NULL;
    _closeSelfTimer = new wxTimer(this);
    _closeTimeout = CLOSE_COUNTDOWN;
}

bool MacAddrUpdatePane::Create(wxWindow *parent, wxWindowID id,
                               const wxString &codeString, const wxPoint &pos,
                               const wxSize &size, long style)
{
    _codeString = codeString;
    wxStringTokenizer tokenzr(codeString, UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD);
    wxString token;
    long loop = 0;

    while (tokenzr.HasMoreTokens())
    {
        token = tokenzr.GetNextToken();
        switch (loop++)
        {
        case 0: token.ToLong(&_row); break;
        case 1: _name = token; break;
        case 2: _ip = token; break;
        case 3: _mac = token; break;
        default: break;
        }
    }

    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void MacAddrUpdatePane::CreateControls()
{
    wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);

    paneSizer->Add(new wxStaticText(this, wxID_STATIC, _("Please input a new MAC address in the below region:")), 0, wxTOP | wxLEFT | wxRIGHT, 15);
    wxString addrString = wxGetApp().m_pAppOptions->GetOption(wxT("VendorCode"));
    wxString productString = wxGetApp().m_pAppOptions->GetOption(wxT("CurrentProductCode"));
    if (productString.empty())
        addrString << wxT(":") << wxGetApp().m_pAppOptions->GetOption(wxT("FirstProductCode"));
    else
        addrString << wxT(":") << productString;
    NetAddrTextCtrl *addr = new NetAddrTextCtrl(this, myID_MAC_UPDATE_NETADDR_TEXTCTRL, NetAddrTextCtrl::NETADDR_TYPE_MAC, addrString);
    paneSizer->Add(addr, 0, wxALL | wxALIGN_CENTER, 10);
    paneSizer->Add(new wxStaticText(this, wxID_STATIC, _("Recommend the use of automatically generated MAC address.")), 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);

    _resultBitmap = new wxStaticBitmap(this, wxID_ANY, _resultUnknown);
    paneSizer->Add(_resultBitmap, 0, wxALL | wxALIGN_CENTER , 5);

    _multiFunctionBtn = new wxButton(this, wxID_ANY, _("Update"));
    _multiFunctionBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUpdatePane::OnUpdateButtonClicked, this);
    paneSizer->Add(_multiFunctionBtn, 0, wxALL | wxALIGN_CENTER, 15);

    SetSizerAndFit(paneSizer);
}

void MacAddrUpdatePane::UpdateReportFile(const wxString& macAddr)
{
    wxSQLite3Database *db = NULL;
    wxString sql;
    
    db = OpenReportDatabase((int)wxGetApp().m_pAppOptions->GetOption(wxT("ReportRotate"), NULL));

    if (db)
    {
        sql << wxT("INSERT INTO ReportTable (Date, Time, BoardName, OldMACAddress, NewMACAddress, Operator) ")
            << wxT("VALUES ('") 
            << wxDateTime::Now().FormatISODate() << wxT("', '")
            << wxDateTime::Now().FormatISOTime() << wxT("', '")
            << _name << wxT("', '")
            << _mac << wxT("', '")
            << macAddr << wxT("', '")
            << wxT("yh.yang@delta.com.tw")
            << wxT("')");

        db->ExecuteUpdate(sql);
    }

    CloseReportDatabase(db);
}


void MacAddrUpdatePane::CloseReportDatabase(wxSQLite3Database *db)
{
    if (db)
    {
        if (db->IsOpen())
            db->Close();
        delete db;
    }
}

wxSQLite3Database *MacAddrUpdatePane::OpenReportDatabase(int reportRotateType)
{
    wxString sql, dbPath;
    wxSQLite3Database *db = NULL;

    /* determinate the db file path */
    dbPath = wxGetApp().m_pAppOptions->GetOption(wxT("ReportFolder"))
        + wxFileName::GetPathSeparator();
    switch (reportRotateType)
    {
    case 0: // never
        dbPath << wxT("report.db");
        break;
    case 1: // year
        dbPath << wxString::Format(wxT("report-%d.db"),
            wxDateTime::Now().GetYear());
        break;
    case 2: // month
        dbPath << wxString::Format(wxT("report-%d-%d.db"),
            wxDateTime::Now().GetYear(),
            wxDateTime::Now().GetMonth() + 1);
        break;
    case 3: // week
        dbPath << wxString::Format(wxT("report-%dW%d.db"),
            wxDateTime::Now().GetYear(),
            wxDateTime::Now().GetWeekOfYear());
        break;
    case 4: // limit
        dbPath << wxString::Format(wxT("report-L%ld.db"),
            wxGetApp().m_pAppOptions->GetOption(wxT("ReportCurrent"), NULL));
        // TODO: check if limit reached. If reached, update ReportCurrent and change dbPath
        break;
    }

    /* open and check db file */
    if (NULL != (db = new wxSQLite3Database()))
    {
        db->Open(dbPath);
        if (db->IsOpen())
        {
            sql << wxT("CREATE TABLE IF NOT EXISTS ReportTable (")
                << wxT("Id INTEGER PRIMARY KEY")
                << wxT(", ") << wxT("Date TEXT")
                << wxT(", ") << wxT("Time TEXT")
                << wxT(", ") << wxT("BoardName TEXT")
                << wxT(", ") << wxT("OldMACAddress TEXT")
                << wxT(", ") << wxT("NewMACAddress TEXT")
                << wxT(", ") << wxT("Operator TEXT")
                << wxT(")");
            db->ExecuteUpdate(sql);
        }
    }

    return db;
}

// event handlers
void MacAddrUpdatePane::OnUpdateButtonClicked(wxCommandEvent& WXUNUSED(event))
{   
    NetAddrTextCtrl *addr = wxDynamicCast(FindWindow(myID_MAC_UPDATE_NETADDR_TEXTCTRL), NetAddrTextCtrl);
    UpdateThread *thread = new UpdateThread(this, _codeString, wxEmptyString, addr->GetValue());
    if (thread && (thread->Create() == wxTHREAD_NO_ERROR) && (thread->Run() == wxTHREAD_NO_ERROR))
        wxGetApp().m_UpdateThreadCount++;
    _multiFunctionBtn->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUpdatePane::OnUpdateButtonClicked, this);
    Bind(wxEVT_COMMAND_THREAD, &MacAddrUpdatePane::OnThread, this, myID_UPDATE_THREAD);
}

void MacAddrUpdatePane::OnCloseBttonClicked(wxCommandEvent& WXUNUSED(event))
{
    _multiFunctionBtn->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUpdatePane::OnCloseBttonClicked, this);
    wxGetApp().m_AuiManager.DetachPane(this);
    Destroy();
}

void MacAddrUpdatePane::OnThread(wxThreadEvent& event)
{
    UpdateThreadMessage msg = event.GetPayload<UpdateThreadMessage>();
    wxString label, token;
    wxStringTokenizer tokenzr;
    int loop = 0;
    long longValue = 0;
    bool invalidResponse = false, pass = false;

    wxASSERT_MSG((wxGetApp().m_UpdateThreadCount == 1), wxT("Update thread count should be 1 when receiving this event"));

    switch (msg.type)
    {
    case UPDATE_THREAD_MODIFY_MAC_ADDRESS_COMPLETED:

        tokenzr.SetString(msg.payload, UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD);
        while (tokenzr.HasMoreTokens())
        {
            token = tokenzr.GetNextToken();
            switch (loop++)
            {
            case 0: // row
                token.ToLong(&longValue);
                if (_row != longValue)
                    invalidResponse = true;
                break;
            case 1: // name
                if (_name != token)
                    invalidResponse = true;
                break;
            case 2: // ip
                if (_ip != token)
                    invalidResponse = true;
                break;
            case 3: // mac
                break;
            case 4: // result
                token.ToLong(&longValue);
                if (longValue == UTERROR_NOERROR)
                    pass = true;
                break;
            default:
                break;
            }

            if (invalidResponse)
                break;
        }

        if (!invalidResponse)
        {
            if (pass)
            {
                /* add new record into report file */
                NetAddrTextCtrl *addr = wxDynamicCast(FindWindow(myID_MAC_UPDATE_NETADDR_TEXTCTRL), NetAddrTextCtrl);
                UpdateReportFile(addr->GetValue());

                // TODO: if using generated mac, update the mac for next one generated.

                /* update result bitmap */
                _resultBitmap->SetBitmap(_resultPass);

                /* start update button label timer and do the first re-label */
                _closeSelfTimer->Start(1000);
                label << _("Close") << wxT(" [") << _closeTimeout << wxT("]");                
            }
            else
            {
                /* update result bitmap */
                _resultBitmap->SetBitmap(_resultFail);
                /* update button label */
                label << _("Close");
            }

            /* udpate the button label */
            _multiFunctionBtn->SetLabel(label);

            /* change button event handler and zero thread counter */
            _multiFunctionBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MacAddrUpdatePane::OnCloseBttonClicked, this);
            wxGetApp().m_UpdateThreadCount = 0;            
        }
        break;
    default:
        wxLogWarning(wxT("Unsupported message type (%d) received!"), msg.type);
        break;
    }
}

void MacAddrUpdatePane::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    wxString label;

    if (--_closeTimeout >= 0)
    {
        label << _("Close") << wxT(" [") << _closeTimeout << wxT("]");
        _multiFunctionBtn->SetLabel(label);
    }
    else
    {
        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, _multiFunctionBtn->GetId());
        evt.SetEventObject(_multiFunctionBtn);
        _multiFunctionBtn->GetEventHandler()->ProcessEvent(evt);
    }
}
