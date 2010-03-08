// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <wx/renderer.h>
#include <wx/filepicker.h>
#include <wx/hyperlink.h>
#include <wx/tokenzr.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif
#include "WidgetId.h"
#include "DownloadPane.h"
#include "SearchThread.h"
#include "UpdateThread.h"
#include "UpdaterApp.h"
#include "AppOptions.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------
#include "img/search_32.xpm"
#include "img/download_to_chip2_64.xpm"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
class MyCustomToggleRenderer : public wxDataViewCustomRenderer
{
public:
    MyCustomToggleRenderer()
        : wxDataViewCustomRenderer(wxT("bool"),
            wxDATAVIEW_CELL_ACTIVATABLE,
            wxALIGN_CENTER)
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

    virtual bool GetValue(wxVariant &value) const
    {
        value = m_toggle;
        return true;
    }

private:
    bool m_toggle;
};

class MyCustomFilePathRenderer : public wxDataViewCustomRenderer
{
public:
    MyCustomFilePathRenderer()
        : wxDataViewCustomRenderer(wxT("string"),
            wxDATAVIEW_CELL_ACTIVATABLE,
            wxALIGN_LEFT)
    { m_path = wxEmptyString; }

    virtual bool Render(wxRect cell, wxDC *dc, int state)
    {
        RenderText(m_path, 0, cell, dc, state);
        return true;
    }

    virtual bool Activate(wxRect WXUNUSED(cell), wxDataViewModel *WXUNUSED(model),
        const wxDataViewItem &WXUNUSED(item), unsigned int WXUNUSED(col))
    {
        wxLogMessage(wxT("MyCustomFilePathRenderer activate"));
        return false;
    }

    virtual wxSize GetSize() const
    {
        const wxDataViewCtrl *view = GetView();
        if (!m_path.empty())
            return view->wxWindowBase::GetTextExtent(m_path);
        return wxSize(wxDVC_DEFAULT_RENDERER_SIZE,wxDVC_DEFAULT_RENDERER_SIZE);
    }

    virtual bool SetValue(const wxVariant &value)
    {
        m_path = value.GetString();
        return true;
    }

    virtual bool GetValue(wxVariant &value) const
    {
        value = m_path;
        return true;
    }

private:
    wxString m_path;
};

class DeviceList : public wxDataViewListCtrl
{
public:
    DeviceList(wxWindow *parent, wxWindowID id = wxID_ANY);

    enum
    {
        COLUMN_DEVICE_UPDATE,
        COLUMN_DEVICE_NAME,
        COLUMN_DEVICE_IPADDRESS,
        COLUMN_DEVICE_MACADDRESS,
        COLUMN_DEVICE_UPDATE_PROGRESS,
        COLUMN_DEVICE_SPECIFIC_IMAGE_FILE_PATH,       

        COLUMN_DEVICE_LIST_MAX
    };

private:
    void OnSelectionChanged(wxDataViewEvent &event);
    void OnItemActivated(wxDataViewEvent &event);
    void OnItemValueChanged(wxDataViewEvent &event);
};

class MyLinkAction : public wxHyperlinkCtrl
{
public:
    MyLinkAction(wxWindow *parent, wxWindowID id, const wxString &text)
        : wxHyperlinkCtrl(parent, id, text, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxBORDER_NONE | wxHL_ALIGN_CENTRE)
    {
        SetVisitedColour(GetNormalColour());
    }
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
DeviceList::DeviceList(wxWindow *parent, wxWindowID id)
    : wxDataViewListCtrl(parent, id, wxDefaultPosition, wxDefaultSize,
    wxDV_SINGLE | wxDV_HORIZ_RULES | wxDV_VERT_RULES)
{
    int column;

    for (column = 0; column < COLUMN_DEVICE_LIST_MAX; column++)
    {
        switch (column)
        {
        case COLUMN_DEVICE_UPDATE:
            AppendColumn(new wxDataViewColumn(_("Update?"), new MyCustomToggleRenderer, column, 60));
            break;
        case COLUMN_DEVICE_NAME:
            AppendColumn(new wxDataViewColumn(_("Name"), new wxDataViewTextRenderer, column, 120, wxALIGN_LEFT));
            break;
        case COLUMN_DEVICE_IPADDRESS:
            AppendColumn(new wxDataViewColumn(_("IP Address"), new wxDataViewTextRenderer, column, 120, wxALIGN_LEFT));
            break;
        case COLUMN_DEVICE_MACADDRESS:
            AppendColumn(new wxDataViewColumn(_("MAC Address"), new wxDataViewTextRenderer, column, 120, wxALIGN_LEFT));
            break;
        case COLUMN_DEVICE_UPDATE_PROGRESS:
            AppendColumn(new wxDataViewColumn(_("Progress"), new wxDataViewProgressRenderer, column, 200, wxALIGN_LEFT));
            break;
        case COLUMN_DEVICE_SPECIFIC_IMAGE_FILE_PATH:
            AppendColumn(new wxDataViewColumn(_("Device-specific Image File Path"), new MyCustomFilePathRenderer, column, 250, wxALIGN_LEFT));
            break;
        default:
            break;
        }
    }

    //Bind(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, &DeviceList::OnSelectionChanged, this);
    //Bind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &DeviceList::OnItemActivated, this);
    //Bind(wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, &DeviceList::OnItemValueChanged, this);
}

void DeviceList::OnSelectionChanged(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Selection changed"));
}

void DeviceList::OnItemActivated(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Item activated"));
}

void DeviceList::OnItemValueChanged(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Item value changed"));
}

BEGIN_EVENT_TABLE(DownloadPane, wxPanel)
    EVT_BUTTON(myID_DOWNLOAD_SEARCH_BTN, DownloadPane::OnSearchButtonClicked)
    EVT_BUTTON(myID_DOWNLOAD_SELECTED_BTN, DownloadPane::OnDownloadButtonClicked)
    EVT_UPDATE_UI(myID_DOWNLOAD_SELECTED_BTN, DownloadPane::OnUpdateDownloadButton)
    EVT_UPDATE_UI(myID_DOWNLOAD_GLOBAL_FILE, DownloadPane::OnUpdateGlobalFilePath)
    EVT_THREAD(myID_SEARCH_THREAD, DownloadPane::OnSearchThread)
    EVT_THREAD(myID_UPDATE_THREAD, DownloadPane::OnUpdateThread)
    EVT_HYPERLINK(myID_TARGET_CHECK_ALL, DownloadPane::OnDeviceCheckAll)
    EVT_HYPERLINK(myID_TARGET_UNCHECK_ALL, DownloadPane::OnDeviceUncheckAll)
    EVT_HYPERLINK(myID_DOWNLOAD_TARGET_LIST_SELECT_NONE, DownloadPane::OnDeviceListSelectNone)
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
    SaveOptionValue();
}

void DownloadPane::Init()
{
    _updateThreadCount = 0;
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    InitOptionValue();
    Center();
    return true;
}

void DownloadPane::CreateControls()
{
    _promptForModifyMAC = new wxInfoBar(this);
    _promptForModifyMAC->AddButton(myID_MODIFY_MAC_BTN, _("Start to modify..."));
    // Must use dynamic event handler here, use static event table is not working here.
    // Becuase the button clicked event is handled in the wxInfoBar itself and don't propagate to it parent...
    _promptForModifyMAC->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DownloadPane::OnModifyMACButtonClicked, this, myID_MODIFY_MAC_BTN);
    _promptForModifyMAC->AddButton(wxID_ANY, _("Ignore"));
    _promptForModifyMAC->SetFont(GetFont().Bold().Larger());
    _promptForUpdateError = new wxInfoBar(this);
    _promptForUpdateError->SetFont(GetFont().Bold().Larger());

    wxStaticBoxSizer *listBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Device list"));

    /* target list box */
    wxButton *search = new wxButton(this, myID_DOWNLOAD_SEARCH_BTN, wxT("Search"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    search->SetBitmap(wxBitmap(search_32_xpm));
    search->SetBitmapDisabled(wxBitmap(wxImage(search_32_xpm).ConvertToGreyscale()));
    wxVector<NetAdapter> &adapter = wxGetApp().m_Adapters;
    wxString addr = _("Method 1");
    if (!adapter.empty())
        addr << wxT(": ") + adapter.at(0).GetBroadcast();
    wxRadioButton *searchMethod1 = new wxRadioButton(this, myID_SEARCH_METHOD1_RB, addr, wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton *searchMethod2 = new wxRadioButton(this, myID_SEARCH_METHOD2_RB, _("Method 2: 255.255.255.255"));
    wxBoxSizer *methodSizer = new wxBoxSizer(wxVERTICAL);
    methodSizer->Add(searchMethod1, 0, wxALL, 5);
    methodSizer->Add(searchMethod2, 0, wxALL, 5);
    //methodSizer->AddStretchSpacer(1);
    wxBoxSizer *searchSizer = new wxBoxSizer(wxHORIZONTAL);
    searchSizer->Add(search, 0, wxALL | wxEXPAND, 5);
    searchSizer->Add(methodSizer, 0, wxALL, 5);
    listBoxSizer->Add(searchSizer, 0, wxALL, 5);

    wxBoxSizer *selectSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer->Add(new MyLinkAction(this, myID_TARGET_CHECK_ALL, _("Check All")), 0, wxLEFT, 5);
    selectSizer->Add(new MyLinkAction(this, myID_TARGET_UNCHECK_ALL, _("Uncheck All")), 0, wxLEFT | wxRIGHT, 5);
    selectSizer->Add(new MyLinkAction(this, myID_DOWNLOAD_TARGET_LIST_SELECT_NONE, _("Select None")), 0, wxLEFT, 5);
    listBoxSizer->Add(selectSizer, 0, wxALL, 0);

    DeviceList *tl = new DeviceList(this, myID_DOWNLOAD_TARGET_LIST);
    listBoxSizer->Add(tl, 1, wxALL | wxEXPAND, 5);

    /* target operation box */
    wxButton *download = new wxButton(this, myID_DOWNLOAD_SELECTED_BTN, _("Update selected"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    download->SetBitmap(wxBitmap(download_to_chip2_64_xpm));
    download->SetBitmapDisabled(wxBitmap(wxImage(download_to_chip2_64_xpm).ConvertToGreyscale()));

    wxRadioButton *rb1 = new wxRadioButton(this, myID_DOWNLOAD_SPECIFIC_RB, _("Use Device-specific Image File"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton *rb2 = new wxRadioButton(this, myID_DOWNLOAD_GLOBAL_RB, _("Use Global Image File"));
    wxBoxSizer *radioSizer = new wxBoxSizer(wxVERTICAL);
    radioSizer->AddStretchSpacer(1);
    radioSizer->Add(rb1, 0, wxALL, 5);
    radioSizer->Add(rb2, 0, wxALL, 5);

    wxFilePickerCtrl *filePicker = new wxFilePickerCtrl(this, myID_DOWNLOAD_GLOBAL_FILE, wxEmptyString, wxFileSelectorPromptStr, wxT("Bootloader or Firmware Binary Files(*.brec)|*.brec|All Files(*.*)|*.*"));
    wxBoxSizer *fileSizer = new wxBoxSizer(wxVERTICAL);
    fileSizer->AddStretchSpacer(1);
    fileSizer->Add(new wxStaticText(this, wxID_STATIC, _("Global Image File Path:")), 0, wxALL, 5);
    fileSizer->Add(filePicker, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *operationBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Device operation"));
    operationBoxSizer->Add(download, 0, wxALL | wxEXPAND, 5);
    operationBoxSizer->Add(radioSizer, 0, wxALL | wxEXPAND, 5);
    operationBoxSizer->Add(fileSizer, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *bgSizer = new wxBoxSizer(wxVERTICAL);
    bgSizer->Add(_promptForModifyMAC, wxSizerFlags().Expand());
    bgSizer->Add(_promptForUpdateError, wxSizerFlags().Expand());
    bgSizer->Add(listBoxSizer, 1, wxALL | wxEXPAND, 5);
    bgSizer->Add(operationBoxSizer, 0, wxALL | wxEXPAND, 5);

    SetSizer(bgSizer);
}

void DownloadPane::InitOptionValue()
{
    AppOptions *pOpt = wxGetApp().m_pAppOptions;
    wxString value;
    long longValue;

    if (pOpt)
    {
        // Currently, there seems to be a strange behavior when set value on the radiobutton which
        // is the first one in it group and only 2 radiobutton in this group.
        // If you set the first one radiobutton to true, it works well, but if you set it to false,
        // the other radiobutton doesn't become true.
        // However, if we set value on the 2nd radiobutton in this group, there is no problem.

        // search method
        wxRadioButton *searchRB = wxDynamicCast(FindWindow(myID_SEARCH_METHOD2_RB), wxRadioButton);
        if (searchRB && pOpt->GetOption(wxT("SearchMethod"), value))
        {
            if (value.ToLong(&longValue))
                searchRB->SetValue(longValue == 1);
        }

        // use global file
        wxRadioButton *globalRB = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_RB), wxRadioButton);
        if (globalRB && pOpt->GetOption(wxT("UseGlobalFile"), value))
        {
            if (value.ToLong(&longValue))
                globalRB->SetValue(longValue == 1);
        }

        // global file path
        wxFilePickerCtrl *filePicker = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_FILE), wxFilePickerCtrl);
        if (filePicker && pOpt->GetOption(wxT("GlobalFilePath"), value))
        {
            filePicker->SetPath(value);
        }
    }
}

void DownloadPane::SaveOptionValue()
{
    AppOptions *pOpt = wxGetApp().m_pAppOptions;
    wxString value;

    if (pOpt)
    {
        // search method
        wxRadioButton *searchRB = wxDynamicCast(FindWindow(myID_SEARCH_METHOD2_RB), wxRadioButton);
        if (searchRB)
        {
            if (searchRB->GetValue())
                value = wxT("1");
            else
                value = wxT("0");
            pOpt->SetOption(wxT("SearchMethod"), value);
        }

        // use global file
        wxRadioButton *globalRB = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_RB), wxRadioButton);
        if (globalRB)
        {
            if (globalRB->GetValue())
                value = wxT("1");
            else
                value = wxT("0");
            pOpt->SetOption(wxT("UseGlobalFile"), value);
        }

        // global file path
        wxFilePickerCtrl *filePicker = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_FILE), wxFilePickerCtrl);
        if (filePicker)
        {
            pOpt->SetOption(wxT("GlobalFilePath"), filePicker->GetPath());
        }
    }
}

bool DownloadPane::IsMACAddressInvalid(const wxString &mac_address)
{
    bool invalid = false;

    if (!mac_address.Cmp(wxT("00:1D:72:9C:94:E5")))
        invalid = true;

    return invalid;
}

//
// event handlers
//
void DownloadPane::OnSearchButtonClicked(wxCommandEvent &event)
{
    AppOptions *pOpt = wxGetApp().m_pAppOptions;
    wxString codedString, value;
    long longValue = 1;

    /* search count */
    if (pOpt && pOpt->GetOption(wxT("SearchCount"), value) && value.ToLong(&longValue))
        codedString << longValue;
    else
        codedString << wxT("1");
    codedString << SEARCH_THREAD_CODEDSTRING_DELIMIT_WORD;

    /* broadcast address */
    wxRadioButton *methodRB = wxDynamicCast(FindWindow(myID_SEARCH_METHOD1_RB), wxRadioButton);
    if (methodRB && methodRB->GetValue())
        codedString << wxT("0");
    else
        codedString << wxT("1");

    SearchThread *thread = new SearchThread(this, codedString);
    if (thread
        && (thread->Create() == wxTHREAD_NO_ERROR)
        && (thread->Run() == wxTHREAD_NO_ERROR))
    {
        wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
        wxDataViewListStore *model;
        wxButton *btn = wxDynamicCast(FindWindow(event.GetId()), wxButton);

        if (lc)
        {
            model = static_cast<wxDataViewListStore *>(lc->GetModel());
            model->DeleteAllItems();
        }
        if (btn)
            btn->Enable(false);
    }
}

void DownloadPane::OnDownloadButtonClicked(wxCommandEvent &event)
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
    wxFilePickerCtrl *filePicker = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_FILE), wxFilePickerCtrl);

    if (lc && filePicker)
    {
        wxString file = filePicker->GetPath();
        wxDataViewListStore *store = lc->GetStore();
        if (store && !file.empty())
        {
            unsigned int row, nRow = store->GetCount();
            _updateThreadCount = 0;
            for (row = 0; row < nRow; row++)
            {
                wxString threadCodedWord;
                wxVariant data;
                store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_UPDATE);
                if (data.GetBool())
                {
                    store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_NAME);
                    wxString name = data.GetString();
                    store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_IPADDRESS);
                    wxString ip = data.GetString();
                    store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_MACADDRESS);
                    wxString mac = data.GetString();
                    threadCodedWord.clear();
                    threadCodedWord
                        << row << UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD
                        << name << UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD
                        << ip << UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD
                        << mac;

                    UpdateThread *thread = new UpdateThread(this, threadCodedWord, file);
                    if (thread
                        && (thread->Create() == wxTHREAD_NO_ERROR)
                        && (thread->Run() == wxTHREAD_NO_ERROR))
                    {
                        _updateThreadCount++;
                    }
                }
            }

            if (_updateThreadCount)
            {
                /* Besides disable button in update ui event handler, we also disable button here
                   right away to avoid this click action re-enter again */
                wxButton *btn = wxDynamicCast(event.GetEventObject(), wxButton);
                if (btn)
                    btn->Enable(false);
            }
            else
                wxLogVerbose(wxT("No target is selected! Please make sure the first column is checked if you want to update it."));
        }
        else
            wxLogError(wxT("Can not find store in wxDataViewListCtrl instance to validate required information!"));

        lc->UnselectAll();
    }
    else
        wxLogError(wxT("Can not find wxDataViewListCtrl instance to validate required information!"));
}

void DownloadPane::OnModifyMACButtonClicked(wxCommandEvent &event)
{
    wxLogMessage(wxT("User press modify MAC address button!"));

    // just call skip here, so this event will be handled by the wxInfoBarBase, which will hide the infobar.
    event.Skip();
}

void DownloadPane::OnUpdateDownloadButton(wxUpdateUIEvent &event)
{
    wxRadioButton *rb = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_RB), wxRadioButton);
    wxFilePickerCtrl *picker = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_FILE), wxFilePickerCtrl);
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    bool at_least_one_is_checked = false, use_global_file = true, all_files_are_ok = true;
    unsigned int row, nRow;
    wxVariant data;

    if (lc && rb && picker)
    {
        /* if use global file, only need to check the global file path */
        use_global_file = rb->GetValue();
        if (use_global_file)
        {
            wxFileName globalFile(picker->GetPath());
            if (!globalFile.FileExists())
                all_files_are_ok = false;
        }

        /* if use device-specific file, need to check file path for every checked row */
        store = lc->GetStore();
        nRow = store->GetCount();
        for (row = 0; row < nRow; row++)
        {
            store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_UPDATE);
            if (data.GetBool())
            {
                at_least_one_is_checked = true;
                if (use_global_file)
                    break;
                else
                {
                    store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_SPECIFIC_IMAGE_FILE_PATH);
                    wxFileName specificFile(data.GetString());
                    if (!specificFile.FileExists())
                    {
                        all_files_are_ok = false;
                        break;
                    }
                }
            }
        }
    }

    if ((_updateThreadCount == 0)
        && at_least_one_is_checked
        && all_files_are_ok)
        event.Enable(true);
    else
        event.Enable(false);
}

void DownloadPane::OnUpdateGlobalFilePath(wxUpdateUIEvent &event)
{
    wxRadioButton *useGlobalFile = wxDynamicCast(FindWindow(myID_DOWNLOAD_GLOBAL_RB), wxRadioButton);
    if (useGlobalFile && useGlobalFile->GetValue())
        event.Enable(true);
    else
        event.Enable(false);
}

void DownloadPane::OnSearchThread(wxThreadEvent &event)
{
    SearchThreadMessage msg = event.GetPayload<SearchThreadMessage>();
    wxButton *btn;
    wxDataViewListStore *model;
    wxDataViewListCtrl *lc;
    wxVector<wxVariant> data;
    wxString name, ip, mac;
    unsigned int nRow, row;
    bool found = false;
    wxVariant variant;
    int column;

    switch (msg.type)
    {
    case SEARCH_THREAD_COMPLETED:
        wxLogMessage(wxT("Device search thread is completed!"));
        btn = wxDynamicCast(FindWindow(myID_DOWNLOAD_SEARCH_BTN), wxButton);
        if (btn) btn->Enable(true);
        break;
    case SEARCH_THREAD_TARGET_FOUND:
        lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
        if (lc)
        {
            model = static_cast<wxDataViewListStore *>(lc->GetModel());
            /* iterator all data in list to avoid the same data */
            nRow = model->GetCount();
            for (row = 0; row < nRow; row++)
            {
                model->GetValueByRow(variant, row, DeviceList::COLUMN_DEVICE_NAME);
                name = variant.GetString();
                model->GetValueByRow(variant, row, DeviceList::COLUMN_DEVICE_IPADDRESS);
                ip = variant.GetString();
                model->GetValueByRow(variant, row, DeviceList::COLUMN_DEVICE_MACADDRESS);
                mac = variant.GetString();
                if ((name == msg.name) && (ip == msg.ip) && (mac == msg.mac))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                for (column = 0; column < DeviceList::COLUMN_DEVICE_LIST_MAX; column++)
                {
                    switch (column)
                    {
                    case DeviceList::COLUMN_DEVICE_UPDATE:
                        data.push_back(false);
                        break;
                    case DeviceList::COLUMN_DEVICE_NAME:
                        data.push_back(msg.name);
                        break;
                    case DeviceList::COLUMN_DEVICE_IPADDRESS:
                        data.push_back(msg.ip);
                        break;
                    case DeviceList::COLUMN_DEVICE_MACADDRESS:
                        data.push_back(msg.mac);
                        break;
                    case DeviceList::COLUMN_DEVICE_UPDATE_PROGRESS:
                        data.push_back(0);
                        break;
                    case DeviceList::COLUMN_DEVICE_SPECIFIC_IMAGE_FILE_PATH:
                        data.push_back(wxEmptyString);
                        break;
                    default:
                        // FIXME
                        data.push_back(0);
                        break;
                    }
                }
                lc->AppendItem(data);

                if (IsMACAddressInvalid(msg.mac)) // replace to invalid mac address
                {
                    wxString msg_to_modify_invalid_mac;
                    msg_to_modify_invalid_mac
                        << _("Device") << wxT(" ") << msg.name << wxT(" ") << _("with invalid MAC address")
                        << wxT(" (") << msg.mac << wxT(")! ") << _("Would you like to modify it?");
                    _promptForModifyMAC->ShowMessage(msg_to_modify_invalid_mac, wxICON_WARNING);
                }
            }
        }
        break;
    default:
        break;
    }
}

void DownloadPane::OnUpdateThread(wxThreadEvent &event)
{
    UpdateThreadMessage msg = event.GetPayload<UpdateThreadMessage>();
    wxDataViewListCtrl *lc;
    wxDataViewListStore *store;
    wxStringTokenizer tokenizer;
    int loop = 0, row = -1, error = UTERROR_UNKNOWN, progress = 0;
    long longValue;
    wxString name, ip;

    switch (msg.type)
    {
    case UPDATE_THREAD_COMPLETED:

        tokenizer.SetString(msg.payload, UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD);
        while (tokenizer.HasMoreTokens())
        {
            wxString token = tokenizer.GetNextToken();
            switch (loop++)
            {
            case 0: // row
                break;
            case 1: // name
                name = token;
                break;
            case 2: // ip
                ip = token;
                break;
            case 3: // mac
                break;
            case 4: // error
                if (token.ToLong(&longValue))
                {
                    error = (int)longValue;
                    wxLogVerbose(_("UpdateThread is completed with error code = %d"), error);
                }
                else
                {
                    wxLogError(_("UpdateThread is completed with unknown error code"));
                }
                break;
            default:
                break;
            }
        }

        _updateThreadCount--;

        if (error)
        {
            wxString msg_for_update_error;
            msg_for_update_error << _("Device") << wxT(" ") << name << wxT("(") << ip << wxT(") ")
                << _("update procedure is failed, error code = ") << error;
            _promptForUpdateError->ShowMessage(msg_for_update_error, wxICON_ERROR);
        }

        break;

    case UPDATE_THREAD_PROGRESS:

        tokenizer.SetString(msg.payload, UPDATE_THREAD_CODEDSTRING_DELIMIT_WORD);
        while (tokenizer.HasMoreTokens())
        {
            wxString token = tokenizer.GetNextToken();
            switch (loop++)
            {
            case 0: // row
                if (token.ToLong(&longValue))
                    row = (int)longValue;
                break;
            case 1: // name
                name = token;
                break;
            case 2: // ip
                ip = token;
                break;
            case 3: // mac
                break;
            case 4: // progress
                if (token.ToLong(&longValue))
                    progress = (int)longValue;
                break;
            default:
                break;
            }
        }

        if (((lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl)) != NULL) && (row != -1))
        {
            if ((store = lc->GetStore()) != NULL)
            {
                wxString ipInList;
                wxVariant data;
                store->GetValueByRow(data, row, DeviceList::COLUMN_DEVICE_IPADDRESS);
                ipInList = data.GetString();
                if (ipInList == ip)
                {
                    data = (long)progress;
                    store->SetValueByRow(data, row, DeviceList::COLUMN_DEVICE_UPDATE_PROGRESS);
                    store->RowChanged(row);
                }
            }
        }
        break;
    default:
        break;
    }
}

void DownloadPane::OnDeviceCheckAll(wxHyperlinkEvent &WXUNUSED(event))
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    wxVariant data = true;

    if (lc)
    {
        if ((store = lc->GetStore()) != NULL)
        {
            unsigned int row, nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                store->SetValueByRow(data, row, DeviceList::COLUMN_DEVICE_UPDATE);
            }

            /* this cause list refresh */
            if (nRow != 0)
                store->RowChanged(row - 1);
        }
    }
}

void DownloadPane::OnDeviceUncheckAll(wxHyperlinkEvent &WXUNUSED(event))
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);
    wxDataViewListStore *store;
    wxVariant data = false;

    if (lc)
    {
        if ((store = lc->GetStore()) != NULL)
        {
            unsigned int row, nRow = store->GetCount();
            for (row = 0; row < nRow; row++)
            {
                store->SetValueByRow(data, row, DeviceList::COLUMN_DEVICE_UPDATE);
            }

            /* this cause list refresh */
            if (nRow != 0)
                store->RowChanged(row - 1);
        }
    }
}

void DownloadPane::OnDeviceListSelectNone(wxHyperlinkEvent &WXUNUSED(event))
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);

    if (lc)
        lc->UnselectAll();
}
