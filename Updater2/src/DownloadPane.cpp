// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dataview.h>
#include <iphlpapi.h>
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

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
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
    wxBoxSizer *bgSizer = new wxBoxSizer(wxVERTICAL);

    /* target search */
    bgSizer->Add(new wxButton(this, myID_DOWNLOAD_SEARCH_BTN, wxT("Search")), 0, wxALL, 5);

    /* target list */
    wxDataViewListCtrl *lc = new wxDataViewListCtrl(this, myID_DOWNLOAD_TARGET_LIST,
        wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_HORIZ_RULES);
    lc->AppendToggleColumn(_("Update?"), wxDATAVIEW_CELL_ACTIVATABLE, 60);
    lc->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, 120);
    lc->AppendTextColumn(_("IP Address"), wxDATAVIEW_CELL_INERT, 120);
    lc->AppendTextColumn(_("MAC Address"), wxDATAVIEW_CELL_INERT, 120);
    lc->AppendProgressColumn(_("Progress"), wxDATAVIEW_CELL_INERT, 250);
    lc->AppendTextColumn(_("File path"), wxDATAVIEW_CELL_INERT);

    bgSizer->Add(lc, 1, wxALL | wxEXPAND, 5);

    /* target operations */
    
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
        wxButton *btn = wxDynamicCast(FindWindow(event.GetId()), wxButton);
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
