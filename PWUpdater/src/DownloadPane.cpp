// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <wx/renderer.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpdThread.h"
#include "UartThread.h"
#include "PWUpdater.h"
#include "AppOptions.h"
#include "NetAdapter.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "xpm/delta_zh_TW.xpm"
#include "xpm/delta_en.xpm"
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
    EVT_THREAD(myID_THREAD_UART, DownloadPane::OnThreadUart)
    EVT_BUTTON(myID_BTN_COMPORT_REFRESH, DownloadPane::OnButtonSerialPortRefresh)
    EVT_BUTTON(myID_BTN_DOWNLOAD, DownloadPane::OnButtonDownload)
    EVT_UPDATE_UI(myID_BTN_DOWNLOAD, DownloadPane::OnUpdateUIButtonDownload)
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
    _downloading = false;
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    /* create ui */
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();

    /* start tftp worker thread */
    StartInternalTftpIfNeed();
    SearchImageFiles();

    /* start uart worker thread */
    StartUartThread();

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
    logoSizer->Add(new wxStaticText(this, wxID_STATIC, _("COM Port")), 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
    wxBoxSizer *refreshSizer = new wxBoxSizer(wxHORIZONTAL);
    logoSizer->Add(refreshSizer, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 5);
    wxChoice *portChoice = new wxChoice(this, myID_CHOICE_COMPORT, wxDefaultPosition, wxSize(80, -1));
    portChoice->Disable();
    refreshSizer->Add(portChoice, 0, wxALL | wxALIGN_CENTER, 0);
    wxBitmapButton *refreshBtn = new wxBitmapButton(this, myID_BTN_COMPORT_REFRESH, wxBitmap(refresh_16_xpm));
    refreshBtn->SetBitmapDisabled(wxBitmap(wxImage(refresh_16_xpm).ConvertToGreyscale()));
    refreshSizer->Add(refreshBtn, 0, wxALL, 0);
    logoSizer->AddStretchSpacer();
    if (wxT("zh_TW") == wxGetApp().m_pOpt->GetOption(wxT("Language")))
        logoSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(delta_zh_TW_xpm)));
    else
        logoSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(delta_en_xpm)));
    paneSizer->Add(logoSizer, 0, wxALL | wxEXPAND, 0);

    /* download file list and operation */
    wxBoxSizer *downloadSizer = new wxBoxSizer(wxVERTICAL);
    downloadSizer->Add(new wxStaticText(this, wxID_STATIC, _("Files To Download")), 0, wxTOP | wxLEFT | wxRIGHT, 5);
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

wxString DownloadPane::GetTftpServerIpAddress()
{
    bool useInternalTftp = false;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    wxVector<NetAdapter> &adapterList = wxGetApp().m_adapterList;
    wxVector<NetAdapter>::iterator it;
    if ((pOpt->GetOption(wxT("UseInternalTftp"), &useInternalTftp)) && useInternalTftp)
    {
        wxString intf;
        if (pOpt->GetOption(wxT("ActivedInterface"), intf))
        {
            for (it = adapterList.begin(); it != adapterList.end(); ++it)
            {
                if (it->GetName() == intf)
                    return it->GetIp();
            }
        }
    }
    else
    {
        return pOpt->GetOption(wxT("ExternalTftpAddress"));
    }

    return wxEmptyString;
}

void DownloadPane::StartUartThread()
{
    wxCriticalSection &cs = wxGetApp().m_uartCS;
    UartThread *&pUart = wxGetApp().m_pUartThread;

    cs.Enter();

    /* check if another uart thread is running... */
    if (pUart)
    {
        cs.Leave();
        wxLogWarning(wxT("There is another uart thread running!"));
        return;
    }

    pUart = new UartThread(this, myID_THREAD_UART);

    if (pUart->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create uart thread!"));
        wxDELETE(pUart);
    }
    else
    {
        if (pUart->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the uart thread!"));
            wxDELETE(pUart);
        }
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
#if 1 // endAddr = block end address
                    dbEntry.Replace(wxT("Offset"), wxT("End"));
                    pOpt->GetOption(dbEntry, strTemp);
                    strTemp.ToULong(&endAddr, 16);
#else // endAddr = block start address + file length
                    endAddr = startAddr + fileLength;
#endif

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

wxString DownloadPane::GetNextDownloadFile(const wxString &currentFile)
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    unsigned int row, nRow;
    wxVariant data;
    wxVector<wxString> files;
    bool findFile = true;

    /* check dvc */
    if (!lc || ((store = lc->GetStore()) == NULL))
        return wxEmptyString;

    /* first? */
    if (currentFile.empty())
        findFile = false;

    nRow = store->GetCount();
    for (row = 0; row < nRow; row++)
    {
        if (findFile)
        {
            store->GetValueByRow(data, row, DownloadFileList::DFL_COL_FILE);
            if (data.GetString() == currentFile)
                findFile = false;
        }
        else
        {
            store->GetValueByRow(data, row, DownloadFileList::DFL_COL_UPDATE);
            if (data.GetBool())
            {
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_FILE);
                return data.GetString();
            }
        }
    }

    return wxEmptyString;
}

void DownloadPane::GetFileInfo(const wxString &file, unsigned long *offset,
                               unsigned long *end, unsigned long *size)
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    unsigned int row, nRow;
    wxVariant data;
    unsigned long longTemp;

    /* parameters check */
    if (!offset || !end || !size)
        return;

    /* init parameters */
    *offset = *end = *size = 0;

    if (lc && ((store = lc->GetStore()) != NULL))
    {
        nRow = store->GetCount();
        for (row = 0; row < nRow; row++)
        {
            store->GetValueByRow(data, row, DownloadFileList::DFL_COL_FILE);
            if (data.GetString() == file)
            {
                /* offset */
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_START);
                data.GetString().ToULong(&longTemp, 16);
                *offset = longTemp;
                /* end */
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_END);
                data.GetString().ToULong(&longTemp, 16);
                *end = longTemp;
                /* size */
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_LENGTH);
                data.GetString().ToULong(&longTemp);
                *size = longTemp;
                break;
            }
        }
    }
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
        if (msg.GetEvent() == TFTPD_EVENT_READ_TRANSFER_DONE)
        {
            wxLogMessage(wxT("Read update: file %s, current = %d, total = %d"),
                fileName, currentBlock, totalBlock);
        }
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

void DownloadPane::OnThreadUart(wxThreadEvent &event)
{
    UartMessage message = event.GetPayload<UartMessage>();
    int evt = message.event;
    wxVector<wxString>::iterator it;
    wxChoice *portChoice;
    wxBitmapButton *portRefresh;
    ThreadSafeQueue<UartMessage> *&pQueue = wxGetApp().m_pUartQueue;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    wxString lastUsedPort, nextDownloadFile;
    unsigned long offset, end, size, longTemp;
    long downloadResult = UART_ERR_NO_ERROR;
    DownloadFileList *dfl = NULL;
    wxDataViewListStore *store = NULL;
    wxVariant data;
    int row, nRow;

    switch (evt)
    {
    case UART_EVENT_PORT_DETECTION:

        /* update ui */
        portChoice = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
        if (portChoice)
        {
            portChoice->Clear();
            for (it = message.payload.begin(); it != message.payload.end(); ++it)
                portChoice->AppendString(*it);
            portChoice->Enable();

            /* auto select last used port */
            pOpt->GetOption(wxT("LastUsedUartPort"), lastUsedPort);
            if (!lastUsedPort.empty())
            {
                /* check if the last used port is available or not */
                for (it = message.payload.begin(); it != message.payload.end(); ++it)
                {
                    if (*it == lastUsedPort)
                        break;
                }
                if (it != message.payload.end())
                    portChoice->SetStringSelection(*it);
            }
        }

        break;

    case UART_EVENT_CONNECTED:

        /* update com port choice - disable ui */
        portChoice = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
        if (portChoice)
        {
            portChoice->SetStringSelection(message.payload.at(0));
            portChoice->Disable();
        }
        /* update com port refresh button - disable ui */
        portRefresh = wxDynamicCast(FindWindow(myID_BTN_COMPORT_REFRESH), wxBitmapButton);
        if (portRefresh)
            portRefresh->Disable();
        /* update last used com port in database */
        pOpt->SetOption(wxT("LastUsedUartPort"), message.payload.at(0));

        break;

    case UART_EVENT_DISCONNECTED:

        /* update com port choice - enable ui */
        portChoice = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
        if (portChoice)
            portChoice->Enable();
        /* update com port refresh button - enable ui */
        portRefresh = wxDynamicCast(FindWindow(myID_BTN_COMPORT_REFRESH), wxBitmapButton);
        if (portRefresh)
            portRefresh->Enable();
        /* update last used com port in database? erase it? current no action */

        break;

    case UART_EVENT_DOWNLOAD_PROGRESS:

        if (message.payload.size() != 2)
        {
            wxLogError(wxT("Incorrect number of parameters for UART_EVENT_DOWNLOAD_PROGRESS"));
            break;
        }

        dfl = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), DownloadFileList);
        if (dfl && ((store = dfl->GetStore()) != NULL))
        {
            nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_FILE);
                if (data.GetString() == message.payload.at(0))
                {
                    message.payload.at(1).ToULong(&longTemp);
                    data = (long)longTemp;
                    store->SetValueByRow(data, row, DownloadFileList::DFL_COL_PROGRESS);
                    store->RowValueChanged(row, DownloadFileList::DFL_COL_PROGRESS);
                    break;
                }
            }
        }

        break;

    case UART_EVENT_DOWNLOAD_RESULT:

        if (message.payload.size() != 2)
        {
            wxLogError(wxT("Incorrect number of parameters for UART_EVENT_DOWNLOAD_RESULT"));
            break;
        }

        message.payload.at(1).ToLong(&downloadResult);

        /* update download result */
        dfl = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), DownloadFileList);
        if (dfl && (NULL != (store = dfl->GetStore())))
        {
            nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_FILE);
                if (data.GetString() == message.payload.at(0))
                {
                    data = wxString::Format(wxT("%ld"), downloadResult);
                    store->SetValueByRow(data, row, DownloadFileList::DFL_COL_RESULT);
                    store->RowValueChanged(row, DownloadFileList::DFL_COL_RESULT);
                    break;
                }
            }
        }

        /* continue? */
        nextDownloadFile = GetNextDownloadFile(message.payload.at(0));
        if ((downloadResult == UART_ERR_NO_ERROR) && !nextDownloadFile.empty())
        {
            GetFileInfo(nextDownloadFile, &offset, &end, &size);
            UartMessage msg(UART_EVENT_DOWNLOAD_NEXT);
            pOpt->GetOption(wxT("RubyDownloadMemory")).ToULong(&longTemp, 16);
            msg.payload.push_back(wxString::Format(wxT("0x%x"), longTemp));
            msg.payload.push_back(nextDownloadFile);
            msg.payload.push_back(wxString::Format(wxT("0x%x"), offset));
            msg.payload.push_back(wxString::Format(wxT("0x%x"), end));
            msg.payload.push_back(wxString::Format(wxT("%lu"), size));
            pQueue->EnQueue(msg);
        }
        else
        {
            if (downloadResult != UART_ERR_NO_ERROR) // error
            {
                wxLogMessage(wxT("Fail to download file [%s], error [%ld]"),
                    message.payload.at(0), downloadResult);
            }
            else if (nextDownloadFile.empty()) // all images download completed
            {

            }

            UartMessage msg(UART_EVENT_DISCONNECT);
            pQueue->EnQueue(msg);

            /* re-enable the button */
            _downloading = false;
        }

        break;

    default:
        wxLogError(wxT("Uart event %d is not handled by main thread"), evt);
        break;
    }
}

void DownloadPane::OnButtonSerialPortRefresh(wxCommandEvent &WXUNUSED(event))
{
    wxChoice *portChoice = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
    UartMessage message(UART_EVENT_PORT_SCAN);
    ThreadSafeQueue<UartMessage> *&pQueue = wxGetApp().m_pUartQueue;

    if (portChoice && pQueue)
    {
        portChoice->Disable();
        pQueue->EnQueue(message);
    }
}

void DownloadPane::OnButtonDownload(wxCommandEvent &event)
{
    wxString file;
    unsigned long offset, end, size, longTemp;
    ThreadSafeQueue<UartMessage> *&pQueue = wxGetApp().m_pUartQueue;
    AppOptions *&pOpt = wxGetApp().m_pOpt;
    UartMessage message(UART_EVENT_DOWNLOAD_FIRST);
    wxString tftpIpAddress = GetTftpServerIpAddress();
    wxChoice *choice = NULL;
    wxButton *btn = NULL;
    DownloadFileList *dfl = NULL;
    wxDataViewListStore *store = NULL;
    unsigned int row, nRow;
    wxVariant data;

    file = GetNextDownloadFile();
    if (!file.empty())
    {
        choice = wxDynamicCast(FindWindow(myID_CHOICE_COMPORT), wxChoice);
        dfl = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), DownloadFileList);
        btn = wxDynamicCast(event.GetEventObject(), wxButton);
        if (choice && dfl && (NULL != (store = dfl->GetStore())) && btn)
        {
            message.payload.push_back(choice->GetStringSelection());
            if (tftpIpAddress.empty())
            {
                wxLogError(wxT("No valid tftp server address"));
                return;
            }
            message.payload.push_back(tftpIpAddress);
            pOpt->GetOption(wxT("RubyDownloadMemory")).ToULong(&longTemp, 16);
            GetFileInfo(file, &offset, &end, &size);
            message.payload.push_back(wxString::Format(wxT("0x%x"), longTemp));
            message.payload.push_back(file);
            message.payload.push_back(wxString::Format(wxT("0x%x"), offset));
            message.payload.push_back(wxString::Format(wxT("0x%x"), end));
            message.payload.push_back(wxString::Format(wxT("%lu"), size));
            pQueue->EnQueue(message);

            /* clear old download progress and result */
            nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                data = (long)0;
                store->SetValueByRow(data, row, DownloadFileList::DFL_COL_PROGRESS);
                store->RowValueChanged(row, DownloadFileList::DFL_COL_PROGRESS);
                data = wxEmptyString;
                store->SetValueByRow(data, row, DownloadFileList::DFL_COL_RESULT);
                store->RowValueChanged(row, DownloadFileList::DFL_COL_RESULT);
            }

            /* disable button */
            _downloading = true;
            btn->Disable();
        }
    }
}

void DownloadPane::OnUpdateUIButtonDownload(wxUpdateUIEvent &event)
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_FILE_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    bool at_least_one_is_checked = false;
    unsigned int row, nRow;
    wxVariant data;

    if (lc)
    {
        if (_downloading == false)
        {
            store = lc->GetStore();
            nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                store->GetValueByRow(data, row, DownloadFileList::DFL_COL_UPDATE);
                if (data.GetBool())
                {
                    at_least_one_is_checked = true;
                    break;
                }
            }
        }

        event.Enable((_downloading == false) && (at_least_one_is_checked == true));
    }
}
