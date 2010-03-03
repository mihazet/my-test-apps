// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <wx/renderer.h>
#include <wx/filepicker.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif
#include "WidgetId.h"
#include "DownloadPane.h"
#include "SearchThread.h"
#include "UpdaterApp.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

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
    EVT_THREAD(myID_SEARCH_THREAD, DownloadPane::OnSearchThread)
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

    /* target search */
    listBoxSizer->Add(new wxButton(this, myID_DOWNLOAD_SEARCH_BTN, wxT("Search")), 0, wxALL, 5);

    /* target list */
    TargetList *tl = new TargetList(this, myID_DOWNLOAD_TARGET_LIST);
    listBoxSizer->Add(tl, 1, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *operationBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Target operation"));

    /* target operations */
    wxBoxSizer *op1Sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *op2Sizer = new wxBoxSizer(wxVERTICAL);
    
    op1Sizer->Add(new wxButton(this, myID_DOWNLOAD_SELECTED_BTN, _("Update Selected")), 1, wxALL | wxEXPAND, 5);
    
    op2Sizer->Add(new wxCheckBox(this, myID_DOWNLOAD_SPECIFIC_CB, _("Use Target-specific image?")), 0, wxALL, 5);
    op2Sizer->Add(new wxStaticText(this, wxID_STATIC, _("Global Image File Path: ")), 0, wxALL, 5);
    op2Sizer->Add(new wxFilePickerCtrl(this, myID_DOWNLOAD_GLOBAL_FILE), 0, wxALL | wxEXPAND, 5);

    operationBoxSizer->Add(op1Sizer, 0, wxALL | wxEXPAND, 5);
    operationBoxSizer->Add(op2Sizer, 1, wxALL | wxEXPAND, 5);
    
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

