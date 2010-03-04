// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <wx/renderer.h>
#include <wx/filepicker.h>
#include <wx/hyperlink.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif
#include "WidgetId.h"
#include "DownloadPane.h"
#include "SearchThread.h"
#include "UpdateThread.h"
#include "UpdaterApp.h"

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

    virtual bool Activate(wxRect WXUNUSED(cell), wxDataViewModel *model,
        const wxDataViewItem &item, unsigned int col)
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

class TargetList : public wxDataViewListCtrl
{
public:
    TargetList(wxWindow *parent, wxWindowID id = wxID_ANY);

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
TargetList::TargetList(wxWindow *parent, wxWindowID id)
    : wxDataViewListCtrl(parent, id, wxDefaultPosition, wxDefaultSize,
    wxDV_SINGLE | wxDV_HORIZ_RULES | wxDV_VERT_RULES)
{
    AppendColumn(new wxDataViewColumn(_("Update?"), new MyCustomToggleRenderer, 0, 60, wxALIGN_CENTER, wxDATAVIEW_COL_RESIZABLE));
    AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, 120);
    AppendTextColumn(_("IP Address"), wxDATAVIEW_CELL_INERT, 120);
    AppendTextColumn(_("MAC Address"), wxDATAVIEW_CELL_INERT, 120);
    AppendProgressColumn(_("Progress"), wxDATAVIEW_CELL_INERT, 200);
    AppendColumn(new wxDataViewColumn(_("Target-specific Image File Path"), new MyCustomFilePathRenderer, 5, 250, wxALIGN_LEFT));

    //Bind(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, &TargetList::OnSelectionChanged, this);
    //Bind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &TargetList::OnItemActivated, this);
    //Bind(wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, &TargetList::OnItemValueChanged, this);
}

void TargetList::OnSelectionChanged(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Selection changed"));
}

void TargetList::OnItemActivated(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Item activated"));
}

void TargetList::OnItemValueChanged(wxDataViewEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Item value changed"));
}

BEGIN_EVENT_TABLE(DownloadPane, wxPanel)
    EVT_BUTTON(myID_DOWNLOAD_SEARCH_BTN, DownloadPane::OnSearchButtonClicked)
    EVT_BUTTON(myID_DOWNLOAD_SELECTED_BTN, DownloadPane::OnUpdateButtonClicked)
    EVT_THREAD(myID_SEARCH_THREAD, DownloadPane::OnSearchThread)
    EVT_THREAD(myID_UPDATE_THREAD, DownloadPane::OnUpdateThread)
    EVT_HYPERLINK(myID_TARGET_CHECK_ALL, DownloadPane::OnTargetCheckAll)
    EVT_HYPERLINK(myID_TARGET_UNCHECK_ALL, DownloadPane::OnTargetUncheckAll)
    EVT_HYPERLINK(myID_DOWNLOAD_TARGET_LIST_SELECT_NONE, DownloadPane::OnTargetListSelectNone)
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
    _updateThreadCount = 0;
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void DownloadPane::CreateControls()
{
    wxStaticBoxSizer *listBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Target list"));

    /* target list box */
    wxButton *search = new wxButton(this, myID_DOWNLOAD_SEARCH_BTN, wxT("Search"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    search->SetBitmap(wxBitmap(search_32_xpm));
    listBoxSizer->Add(search, 0, wxALL, 5);

    wxBoxSizer *selectSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer->Add(new MyLinkAction(this, myID_TARGET_CHECK_ALL, _("Check All")), 0, wxLEFT, 5);
    selectSizer->Add(new MyLinkAction(this, myID_TARGET_UNCHECK_ALL, _("Uncheck All")), 0, wxLEFT | wxRIGHT, 5);
    selectSizer->Add(new MyLinkAction(this, myID_DOWNLOAD_TARGET_LIST_SELECT_NONE, _("Select None")), 0, wxLEFT, 5);
    listBoxSizer->Add(selectSizer, 0, wxALL, 0);

    TargetList *tl = new TargetList(this, myID_DOWNLOAD_TARGET_LIST);
    listBoxSizer->Add(tl, 1, wxALL | wxEXPAND, 5);

    /* target operation box */
    wxButton *download = new wxButton(this, myID_DOWNLOAD_SELECTED_BTN, _("Update selected"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    download->SetBitmap(wxBitmap(download_to_chip2_64_xpm));
    
    wxRadioButton *rb1 = new wxRadioButton(this, myID_DOWNLOAD_SPECIFIC_RB, _("Use Target-specific Image File"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton *rb2 = new wxRadioButton(this, myID_DOWNLOAD_GLOBAL_RB, _("Use Global Image File"));
    wxBoxSizer *radioSizer = new wxBoxSizer(wxVERTICAL);
    radioSizer->AddStretchSpacer(1);
    radioSizer->Add(rb1, 0, wxALL, 5);
    radioSizer->Add(rb2, 0, wxALL, 5);

    wxFilePickerCtrl *filePicker = new wxFilePickerCtrl(this, myID_DOWNLOAD_GLOBAL_FILE);
    wxBoxSizer *fileSizer = new wxBoxSizer(wxVERTICAL);
    fileSizer->AddStretchSpacer(1);
    fileSizer->Add(new wxStaticText(this, wxID_STATIC, _("Global Image File Path:")), 0, wxALL, 5);
    fileSizer->Add(filePicker, 0, wxALL | wxEXPAND, 5);
    
    wxStaticBoxSizer *operationBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Target operation"));
    operationBoxSizer->Add(download, 0, wxALL | wxEXPAND, 5);
    operationBoxSizer->Add(radioSizer, 0, wxALL | wxEXPAND, 5);
    operationBoxSizer->Add(fileSizer, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *bgSizer = new wxBoxSizer(wxVERTICAL);
    bgSizer->Add(listBoxSizer, 1, wxALL | wxEXPAND, 5);
    bgSizer->Add(operationBoxSizer, 0, wxALL | wxEXPAND, 5);

    SetSizer(bgSizer);
}

//
// event handlers
//
void DownloadPane::OnSearchButtonClicked(wxCommandEvent &event)
{
    SearchThread *thread = new SearchThread(this);
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

void DownloadPane::OnUpdateButtonClicked(wxCommandEvent &event)
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);

    if (lc)
    {
        wxDataViewListStore *store = lc->GetStore();
        if (store)
        {
            unsigned int row, nRow = store->GetCount();
            _updateThreadCount = 0;
            for (row = 0; row < nRow; row++)
            {
                wxVariant data;
                store->GetValueByRow(data, row, 0);
                if (data.GetBool())
                {
                    store->GetValueByRow(data, row, 2);
                    wxString ip = data.GetString();

                    UpdateThread *thread = new UpdateThread(this, ip, row);
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

    switch (msg.type)
    {
    case SEARCH_THREAD_COMPLETED:
        wxLogMessage(wxT("Target search thread is completed!"));
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
                model->GetValueByRow(variant, row, 1);
                name = variant.GetString();
                model->GetValueByRow(variant, row, 2);
                ip = variant.GetString();
                model->GetValueByRow(variant, row, 3);
                mac = variant.GetString();
                if ((name == msg.name) && (ip == msg.ip) && (mac == msg.mac))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                data.push_back(false);
                data.push_back(msg.name);
                data.push_back(msg.ip);
                data.push_back(msg.mac);
                data.push_back(0);
                data.push_back(wxEmptyString);
                lc->AppendItem(data);
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
    wxButton *btn;
    wxDataViewListCtrl *lc;
    wxDataViewListStore *store;

    switch (msg.type)
    {
    case UPDATE_THREAD_COMPLETED:
        wxLogMessage(wxT("UpdateThread is completed with error code = %d"), msg.error);
        if (--_updateThreadCount == 0)
        {
            if ((btn = wxDynamicCast(FindWindow(myID_DOWNLOAD_SELECTED_BTN), wxButton)) != NULL)
                btn->Enable();
        }
        break;
    case UPDATE_THREAD_PROGRESS:
        if ((lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl)) != NULL)
        {
            if ((store = lc->GetStore()) != NULL)
            {
                wxString ipInList;
                wxVariant data;
                store->GetValueByRow(data, msg.row, 2);
                ipInList = data.GetString();
                if (ipInList == msg.targetIpAddress)
                {
                    data = (long)msg.progress;
                    store->SetValueByRow(data, msg.row, 4);
                    store->RowChanged(msg.row);
                }
            }
        }
        break;
    default:
        break;
    }
}

void DownloadPane::OnTargetCheckAll(wxHyperlinkEvent &WXUNUSED(event))
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
                store->SetValueByRow(data, row, 0);
            }

            /* this cause list refresh */
            if (nRow != 0)
                store->RowChanged(row - 1);
        }
    }
}

void DownloadPane::OnTargetUncheckAll(wxHyperlinkEvent &WXUNUSED(event))
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
                store->SetValueByRow(data, row, 0);
            }

            /* this cause list refresh */
            if (nRow != 0)
                store->RowChanged(row - 1);
        }
    }
}

void DownloadPane::OnTargetListSelectNone(wxHyperlinkEvent &WXUNUSED(event))
{
    wxDataViewListCtrl *lc = wxDynamicCast(FindWindow(myID_DOWNLOAD_TARGET_LIST), wxDataViewListCtrl);

    if (lc)
        lc->UnselectAll();
}
