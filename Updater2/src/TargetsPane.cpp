// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "WidgetId.h"
#include "TargetsPane.h"
#include "UpdaterApp.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
class SearchThread : public wxThread
{
public:
    SearchThread(TargetsPane *handler) : wxThread(wxTHREAD_DETACHED)
    { _pHandler = handler; }
    ~SearchThread() { }

    virtual wxThread::ExitCode Entry();

private:
    TargetsPane *_pHandler;
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
wxThread::ExitCode SearchThread::Entry()
{
    int loop = 5;

    while (loop > 0)
    {
        if (TestDestroy())
            break;

        wxLogMessage(wxT("Message from thread[%ld], loop = %d"), GetId(), loop);
        loop--;

        wxMilliSleep(3000);
    }

    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_SEARCH_THREAD);
    wxQueueEvent(_pHandler, event.Clone());

    return (ExitCode)0;
}

BEGIN_EVENT_TABLE(TargetsPane, wxPanel)
    EVT_BUTTON(myID_TARGET_SEARCH_BTN, TargetsPane::OnSearchButtonClicked)
    EVT_THREAD(myID_SEARCH_THREAD, TargetsPane::OnSearchThread)
END_EVENT_TABLE()

TargetsPane::TargetsPane()
{
    Init();
}

TargetsPane::TargetsPane(wxWindow *parent, wxWindowID id, 
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

TargetsPane::~TargetsPane()
{

}

void TargetsPane::Init()
{

}

bool TargetsPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void TargetsPane::CreateControls()
{
    wxBoxSizer *bgSizer = new wxBoxSizer(wxVERTICAL);

    bgSizer->Add(new wxButton(this, myID_TARGET_SEARCH_BTN, wxT("Search")), 0, wxALL, 5);

    SetSizer(bgSizer);
}

//
// event handlers
//
void TargetsPane::OnSearchButtonClicked(wxCommandEvent &event)
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

void TargetsPane::OnSearchThread(wxThreadEvent &event)
{
    wxButton *btn = wxDynamicCast(FindWindow(myID_TARGET_SEARCH_BTN), wxButton);
    if (btn) btn->Enable(true);
}
