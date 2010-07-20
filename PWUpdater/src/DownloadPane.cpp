// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <wx/renderer.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#ifdef __WXGTK__
#include <glob.h>
#endif
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpdThread.h"
#include "PWUpdater.h"
#include "AppOptions.h"
#include "NetAdapter.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "xpm/delta.xpm"
#include "xpm/download_to_chip2_64.xpm"
#include "xpm/refresh_16.xpm"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
class DownloadFileList : public wxDataViewListCtrl
{
public:
    DownloadFileList(wxWindow *parent, wxWindowID id = wxID_ANY);

    enum
    {
        DFL_COL_UPDATE,
        DFL_COL_FILE,
        DFL_COL_START,
        DFL_COL_END,
        DFL_COL_LENGTH,
        DFL_COL_PROGRESS,
        DFL_COL_RESULT,

        DFL_COL_MAX
    };
};

class MyCustomToggleRenderer : public wxDataViewCustomRenderer
{
public:
    MyCustomToggleRenderer()
        : wxDataViewCustomRenderer(wxT("bool"),
            wxDATAVIEW_CELL_ACTIVATABLE)
    { m_toggle = false; }

    virtual bool Render(wxRect cell, wxDC *dc, int WXUNUSED(state))
    {
        int flags = 0;
        if (m_toggle)
            flags |= wxCONTROL_CHECKED;
        if (GetMode() != wxDATAVIEW_CELL_ACTIVATABLE)
            flags |= wxCONTROL_DISABLED;

        // check boxes we draw must always have the same, standard size (if it's
        // bigger than the cell size the checkbox will be truncated because the
        // caller had set the clipping rectangle to prevent us from drawing outside
        // the cell)
        cell.SetSize(GetSize());

        wxRendererNative::Get().DrawCheckBox(
                GetOwner()->GetOwner(),
                *dc,
                cell,
                flags );

        return true;
    }

    virtual bool Activate(wxRect WXUNUSED(cell), wxDataViewModel *model,
        const wxDataViewItem &item, unsigned int col)
    {
        model->ChangeValue(!m_toggle, item, col);
        return false;
    }

    virtual bool LeftClick(wxPoint WXUNUSED(cursor), wxRect WXUNUSED(cell),
        wxDataViewModel *model, const wxDataViewItem &item,
        unsigned int col)
    {
        model->ChangeValue(!m_toggle, item, col);
        return false;
    }

    virtual wxSize GetSize() const
    {
        // the window parameter is not used by GetCheckBoxSize() so it's
        // safe to pass NULL
        return wxRendererNative::Get().GetCheckBoxSize(NULL);
    }

    virtual bool SetValue(const wxVariant &value)
    {
        m_toggle = value.GetBool();
        return true;
    }

    virtual bool GetValue(wxVariant &WXUNUSED(value)) const { return true; }

    virtual int GetAlignment() const { return wxALIGN_CENTER; };

private:
    bool m_toggle;
};

class MyCustomUpdateResultRenderer : public wxDataViewCustomRenderer
{
public:
    MyCustomUpdateResultRenderer()
        : wxDataViewCustomRenderer(wxT("string"),
            wxDATAVIEW_CELL_INERT)
    {
        m_result = wxEmptyString;
        m_ok = wxT("OK");
        m_error = wxT("ERROR");
        m_empty = true;
    }

    virtual bool Render(wxRect cell, wxDC *dc, int state)
    {
        long longValue = 0;

        if (!m_empty && m_result.ToLong(&longValue))
        {
            if (longValue)
                dc->SetBrush(*wxRED_BRUSH);
            else
                dc->SetBrush(*wxGREEN_BRUSH);
            dc->SetPen(*wxTRANSPARENT_PEN);

            cell.Deflate(2);
            dc->DrawRoundedRectangle(cell, 5);
            RenderText(longValue ? m_error : m_ok,
                0,
                wxRect(dc->GetTextExtent(longValue ? m_error : m_ok)).CenterIn(cell),
                dc,
                state);
        }
        else
        {
            RenderText(wxEmptyString, 0, cell, dc, state);
        }
        return true;
    }

    virtual wxSize GetSize() const
    {
        return wxSize(50, 20);
    }

    virtual bool SetValue(const wxVariant &value)
    {
        m_result = value.GetString();
        m_empty = m_result.empty();
        return true;
    }

    virtual bool GetValue(wxVariant &WXUNUSED(value)) const { return true; }

private:
    wxString m_result, m_ok, m_error;
    bool m_empty;
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
DownloadFileList::DownloadFileList(wxWindow *parent, wxWindowID id)
    : wxDataViewListCtrl(parent, id, wxDefaultPosition, wxDefaultSize,
    wxDV_SINGLE | wxDV_HORIZ_RULES | wxDV_VERT_RULES)
{
    int col;

    for (col = 0; col < DFL_COL_MAX; col++)
    {
        switch (col)
        {
        case DFL_COL_UPDATE:
            AppendColumn(new wxDataViewColumn(_("Update?"), new MyCustomToggleRenderer, col, 60));
            break;
        case DFL_COL_FILE:
            AppendColumn(new wxDataViewColumn(_("File"), new wxDataViewTextRenderer, col, 80, wxALIGN_LEFT));
            break;
        case DFL_COL_START:
            AppendColumn(new wxDataViewColumn(_("Start"), new wxDataViewTextRenderer, col, 80, wxALIGN_LEFT));
            break;
        case DFL_COL_END:
            AppendColumn(new wxDataViewColumn(_("End"), new wxDataViewTextRenderer, col, 80, wxALIGN_LEFT));
            break;
        case DFL_COL_LENGTH:
            AppendColumn(new wxDataViewColumn(_("Length"), new wxDataViewTextRenderer, col, 80, wxALIGN_LEFT));
            break;
        case DFL_COL_PROGRESS:
            AppendColumn(new wxDataViewColumn(_("Progress"), new wxDataViewProgressRenderer, col, 140, wxALIGN_LEFT));
            break;
        case DFL_COL_RESULT:
            AppendColumn(new wxDataViewColumn(_("Result"), new MyCustomUpdateResultRenderer, col, 80, wxALIGN_LEFT));
            break;
        default:
            break;
        }
    }
}

BEGIN_EVENT_TABLE(DownloadPane, wxPanel)
    EVT_THREAD(myID_THREAD_TFTPD, DownloadPane::OnThreadTftpd)
    EVT_BUTTON(myID_BTN_COMPORT_REFRESH, DownloadPane::OnButtonSerialPortRefresh)
END_EVENT_TABLE()

DownloadPane::DownloadPane()
{
    Init();
}

DownloadPane::DownloadPane(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

DownloadPane::~DownloadPane()
{
}

void DownloadPane::Init()
{
    _serialPort.clear();
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    /* create ui */
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();

    StartInternalTftpIfNeed();
    SearchImageFiles();

    /* scan serial port */
    DoSearchFreeSerialPort(true);

    return true;
}

void DownloadPane::RescanImageFiles()
{
    SearchImageFiles();
}

void DownloadPane::CreateControls()
{
    wxBoxSizer *paneSizer = new wxBoxSizer(wxHORIZONTAL);

    /* logo and com port */
    wxBoxSizer *logoSizer = new wxBoxSizer(wxVERTICAL);
    logoSizer->Add(new wxStaticText(this, wxID_STATIC, _("COM Port")), 0, wxALL | wxALIGN_CENTER, 5);
    wxBoxSizer *refreshSizer = new wxBoxSizer(wxHORIZONTAL);
    logoSizer->Add(refreshSizer, 0, wxALL | wxEXPAND, 5);
    refreshSizer->Add(new wxChoice(this, myID_CHOICE_COMPORT, wxDefaultPosition, wxSize(60, -1)), 0, wxALL | wxALIGN_CENTER, 0);
    refreshSizer->Add(new wxBitmapButton(this, myID_BTN_COMPORT_REFRESH, wxBitmap(refresh_16_xpm)), 0, wxALL, 0);
    logoSizer->AddStretchSpacer();
    logoSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(delta_xpm)));
    paneSizer->Add(logoSizer, 0, wxALL | wxEXPAND, 0);

    /* download file list and operation */
    wxBoxSizer *downloadSizer = new wxBoxSizer(wxVERTICAL);
    downloadSizer->Add(new wxStaticText(this, wxID_STATIC, _("Files To Download")), 0, wxALL, 5);
    downloadSizer->Add(new DownloadFileList(this, myID_DOWNLOAD_FILE_LIST), 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *opSizer = new wxBoxSizer(wxHORIZONTAL);
    downloadSizer->Add(opSizer, 0, wxALL | wxEXPAND, 0);
    wxStaticBoxSizer *optSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Options"));
    opSizer->Add(optSizer, 1, wxALL | wxEXPAND, 5);
    optSizer->Add(new wxCheckBox(this, myID_CHKBOX_NOTIFY_ON_COMPLETED, _("Notify On Completion")), 0, wxALL, 5);
    optSizer->Add(new wxCheckBox(this, myID_CHKBOX_RESET_TARGET_AFTER_DOWNLOAD, _("Reset Target After Download")), 0, wxALL, 5);
    wxButton *downloadBtn = new wxButton(this, myID_BTN_DOWNLOAD, _("Download selected"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    downloadBtn->SetBitmap(wxBitmap(download_to_chip2_64_xpm));
    downloadBtn->SetBitmapDisabled(wxBitmap(wxImage(download_to_chip2_64_xpm).ConvertToGreyscale()));
    opSizer->Add(downloadBtn, 0, wxALL | wxEXPAND, 10);

    paneSizer->Add(downloadSizer, 1, wxALL | wxEXPAND, 0);

    SetSizerAndFit(paneSizer);
}

bool DownloadPane::StartInternalTftpIfNeed()
{
    bool useInternalTftp = false;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    wxVector<NetAdapter> &adapterList = wxGetApp().m_adapterList;
    wxVector<NetAdapter>::iterator it;
    if ((pOpt->GetOption(wxT("UseInternalTftp"), &useInternalTftp)) && useInternalTftp)
    {
        wxString intf, tftpRoot = pOpt->GetOption(wxT("TftpdRoot"));
        if (pOpt->GetOption(wxT("ActivedInterface"), intf))
        {
            for (it = adapterList.begin(); it != adapterList.end(); ++it)
            {
                if (it->GetName() == intf)
                {
                    DoStartTftpServerThread(it->GetIp(), tftpRoot);
                    break;
                }
            }
        }
    }

    return useInternalTftp;
}

void DownloadPane::SearchImageFiles()
{
    bool useInternalTftp = false;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    if ((pOpt->GetOption(wxT("UseInternalTftp"), &useInternalTftp)) && useInternalTftp)
        DoSearchLocalImageFiles();
    else
    {
        /* search image files in the external tftp server */
    }
}

void DownloadPane::DoStartTftpServerThread(const wxString &ipAddr,
                                           const wxString &root)
{
    wxIPV4address local;
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;

    /* assign preferred interface */
    if (ipAddr.empty())
        local.AnyAddress();
    else
        local.Hostname(ipAddr);

    cs.Enter();

    /* check if another tftp server thread is running... */
    if (pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is another tftp server thread running!"));
        return;
    }

    pServer = new TftpdServerThread(this, myID_THREAD_TFTPD, local, root);

    if (pServer->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create tftp server thread!"));
        wxDELETE(pServer);
    }
    else
    {
        if (pServer->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the tftp server thread!"));
            wxDELETE(pServer);
        }
    }

    cs.Leave();
}

void DownloadPane::DoStopTftpServerThread()
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;

    cs.Enter();

    /* check if tftp server thread is already running... */
    if (!pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is no tftp server thread running."));
        return;
    }

    if (pServer->Delete() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't delete tftp server thread!"));
    }

    cs.Leave();
}

void DownloadPane::DoSearchLocalImageFiles()
{
    wxStringTokenizer tokenizr;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    bool &keyFound = wxGetApp().m_keyFound;
    wxVector<wxString> fileOrder;
    wxVector<wxString> fileAuth;
    wxVector<wxString>::iterator it, it2;
    wxString dbValue, token, rootPath, fileName, dbEntry, strTemp;
    wxFileName fn;
    bool needAuth;
    int column;
    wxVector<wxVariant> data;
    unsigned long startAddr, endAddr, fileLength;

    /* load image files search order */
    dbValue = pOpt->GetOption(wxT("ImageFilesSearchOrder"));
    tokenizr.SetString(dbValue, wxT(";"));
    while (tokenizr.HasMoreTokens())
    {
        token = tokenizr.GetNextToken();
        fileOrder.push_back(token);
    }

    /* load image files need auth */
    dbValue = pOpt->GetOption(wxT("ImageFilesNeedAuth"));
    tokenizr.SetString(dbValue, wxT(";"));
    while (tokenizr.HasMoreTokens())
    {
        token = tokenizr.GetNextToken();
        fileAuth.push_back(token);
    }

    /* start to search files */
    DownloadFileList *dfl = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), DownloadFileList);
    wxDataViewListStore *store = dfl ? static_cast<wxDataViewListStore *>(dfl->GetModel()) : NULL;
    rootPath = pOpt->GetOption(wxT("TftpdRoot"));
    if (rootPath.empty()) rootPath = wxGetCwd();
    if (store) store->DeleteAllItems();
    for (it = fileOrder.begin(); it != fileOrder.end(); ++it)
    {
        needAuth = false;
        for (it2 = fileAuth.begin(); it2 != fileAuth.end(); ++it2)
        {
            if (!it->Cmp(*it2))
            {
                needAuth = true;
                break;
            }
        }

        if ((needAuth && keyFound) || !needAuth)
        {
            fn.Clear();
            fileName = pOpt->GetOption(*it);
            fn.Assign(rootPath, fileName);
            if (fn.IsFileReadable())
            {
                if (dfl && store)
                {
                    dbEntry = *it;
                    dbEntry.Replace(wxT("Image"), wxT("Offset"));
                    pOpt->GetOption(dbEntry, strTemp);
                    strTemp.ToULong(&startAddr, 16);
                    fileLength = wxFileName::GetSize(fn.GetFullPath()).ToULong();
                    endAddr = startAddr + fileLength;

                    for (column = 0; column < DownloadFileList::DFL_COL_MAX; column++)
                    {
                        switch (column)
                        {
                        case DownloadFileList::DFL_COL_UPDATE:
                            data.push_back(false);
                            break;
                        case DownloadFileList::DFL_COL_FILE:
                            data.push_back(fileName);
                            break;
                        case DownloadFileList::DFL_COL_START:
                            data.push_back(wxString::Format(wxT("0x%lX"), startAddr));
                            break;
                        case DownloadFileList::DFL_COL_END:
                            data.push_back(wxString::Format(wxT("0x%lX"), endAddr));
                            break;
                        case DownloadFileList::DFL_COL_LENGTH:
                            data.push_back(wxString::Format(wxT("%lu"), fileLength));
                            break;
                        case DownloadFileList::DFL_COL_PROGRESS:
                            data.push_back(wxEmptyString);
                            break;
                        case DownloadFileList::DFL_COL_RESULT:
                            data.push_back(wxEmptyString);
                            break;
                        default:
                            break;
                        }
                    }
                    dfl->AppendItem(data);
                    data.clear();
                }
            }
        }
    }
}

int DownloadPane::DoSearchFreeSerialPort(bool update)
{
    char port[16];
    wxSerialPort com;
    int id;

    _serialPort.clear();

#if defined (__WXMSW__)
    for (id = 0; id < 100; id++)
    {
        COMMCONFIG cc;
        DWORD dwSize = sizeof(cc);

        sprintf(&port[0], "COM%d", id);
        if (::GetDefaultCommConfigA(port, &cc, &dwSize))
        {
            if (cc.dwProviderSubType == PST_RS232)
            {
                if (id >= 10)
                    sprintf(&port[0], "\\\\.\\COM%d", id);
                if (com.Open(port) < 0)
                    continue;
                _serialPort.push_back(id);
                com.Close();
            }
        }
    }

#elif defined (__WXGTK__)

    glob_t globbuf;

    // search standard serial port
    strcpy(&port[0], "/dev/ttyS*");
    if (0 == glob(port, GLOB_ERR, NULL, &globbuf))
    {
        // no error, glob was successful
        for (id = 0; id < globbuf.gl_pathc; id++)
        {
            if (com.Open(globbuf.gl_pathv[id]) < 0)
                continue;
            _serialPort.push_back(id);
            com.Close();
        }
    }
    globfree(&globbuf);

#endif

    if (update)
    {
        wxChoice *portSel = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
        wxVector<int>::iterator it;
        if (portSel)
        {
            portSel->Clear();
            for (it = _serialPort.begin(); it < _serialPort.end(); ++it)
                portSel->Append(wxString::Format(wxT("COM%d"), *it));
        }
    }

    return _serialPort.size();
}

//
// event handlers
//
void DownloadPane::OnThreadTftpd(wxThreadEvent &event)
{
    TftpdMessage msg = event.GetPayload<TftpdMessage>();
    int transferMode, errorCode, currentBlock, totalBlock;
    wxString fileName, errorMessage, statusText;
    PWUpdaterFrame *frame = NULL;
    wxStatusBar *bar = NULL;

    switch (msg.GetEvent())
    {
    case TFTPD_EVENT_SERVER_STARTED:
        if (NULL != (frame = wxDynamicCast(FindWindowById(myID_FRAME), PWUpdaterFrame)))
        {
            if (NULL != (bar = frame->GetStatusBar()))
            {
                statusText = _("Internal TFTP") +
                    wxString::Format(wxT(" [%d.%d.%d.%d] "),
                        (msg.GetNum1() >> 8) & 0xFF,
                        (msg.GetNum1() & 0xFF),
                        (msg.GetNum2() >> 8) & 0xFF,
                        (msg.GetNum2() & 0xFF)) +
                    _("Root Dir") + wxT(" [") + msg.GetString() + wxT("]");
                bar->SetStatusText(statusText, PWUpdaterFrame::STATBAR_FLD_TFTP);
            }
        }
        break;
    case TFTPD_EVENT_READ_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_READ_TRANSFER_UPDATE:
    case TFTPD_EVENT_READ_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_WRITE_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_WRITE_TRANSFER_UPDATE:
    case TFTPD_EVENT_WRITE_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_ERROR:
        errorCode = msg.GetNum1();
        errorMessage = msg.GetString();
        wxLogMessage(wxT("Tftp error: error code = %d, error message = %s"),
            errorCode, errorMessage);
        break;
    default:
        wxLogError(wxT("Unknown TFTPD messge received! (%d"), msg.GetEvent());
        return;
    }
}

void DownloadPane::OnButtonSerialPortRefresh(wxCommandEvent &WXUNUSED(event))
{
    DoSearchFreeSerialPort(true);
}

