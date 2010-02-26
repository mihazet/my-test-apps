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
    SearchThread(TargetsPane *handler);
    ~SearchThread();
    virtual wxThread::ExitCode Entry();

private:
    TargetsPane *_pHandler;
    unsigned char *_recvBuf;
};

typedef enum
{
    SEARCH_THREAD_COMPLETED,
    SEARCH_THREAD_TARGET_FOUND,
} STMType;

class SearchThreadMessage
{
public:
    SearchThreadMessage()
    {
        type = SEARCH_THREAD_COMPLETED;
        ip = name = wxEmptyString;
    }
    STMType type;
    wxString ip;
    wxString name;
};

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
#define RECVBUFSIZE 128
SearchThread::SearchThread(TargetsPane *handler) 
    : wxThread(wxTHREAD_DETACHED), _pHandler(handler)
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxIPV4address local;

    if (!netAdapter.empty())
    {
        local.Hostname(netAdapter.at(0).GetIp());
        netAdapter.at(0).udp = new wxDatagramSocket(local, wxSOCKET_NOWAIT);
    }

    _recvBuf = new unsigned char[RECVBUFSIZE];
}

SearchThread::~SearchThread()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    if (!netAdapter.empty())
    {
        wxDELETE(netAdapter.at(0).udp);
    }

    wxDELETE(_recvBuf);
}

wxThread::ExitCode SearchThread::Entry()
{
    int loop;
    wxThreadEvent event(wxEVT_COMMAND_THREAD, myID_SEARCH_THREAD);
    SearchThreadMessage msg;
    const unsigned char targetQueryCommand[] =
    {
        0x53, 0x51, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const unsigned char targetReplyCommand[] =
    {
        0x73, 0x71, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxIPV4address broadcast, remote;
    broadcast.Hostname(netAdapter.at(0).GetBroadcast());
    broadcast.Service(40000);
    wxDatagramSocket *udpSocket = netAdapter.at(0).udp;

    wxLogMessage(wxT("Target search thread is running!"));
    
    for (loop = 0; (loop < 4) && !TestDestroy(); loop++)
    {
        wxLogVerbose(wxT("Broadcast target discovery command!"));
        udpSocket->SendTo(broadcast, targetQueryCommand, 16);

        wxDateTime start = wxDateTime::Now(), current;
        while (!TestDestroy())
        {
            udpSocket->RecvFrom(remote, _recvBuf, RECVBUFSIZE);
            if (!udpSocket->Error())
            {
                wxUint32 count = udpSocket->LastCount();
                if (count > 0)
                {
                    wxLogVerbose(wxT("Receive %d byte(s) data from %s"),
                        count, remote.IPAddress());
                    if ((count > 16) && (count <= 36))
                    {
                        if (!memcmp(_recvBuf, targetReplyCommand, 16))
                        {
                            msg.type = SEARCH_THREAD_TARGET_FOUND;
                            msg.ip = remote.IPAddress();
                            msg.name = wxString(&_recvBuf[16]);
                            event.SetPayload(msg);
                            wxQueueEvent(_pHandler, event.Clone());
                        }
                    }
                }                
            }
            current = wxDateTime::Now();
            wxTimeSpan diff = current.Subtract(start);
            if (diff.GetSeconds() >= 3)
                break;
        }
    }

    /* thread completed */
    msg.type = SEARCH_THREAD_COMPLETED;
    event.SetPayload(msg);
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
    bgSizer->Add(new wxButton(this, wxID_ANY, wxT("Cancel")), 0, wxALL, 5);

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
    SearchThreadMessage msg = event.GetPayload<SearchThreadMessage>();
    wxButton *btn;

    switch (msg.type)
    {
    case SEARCH_THREAD_COMPLETED:
        wxLogMessage(wxT("Target search thread is completed!"));
        btn = wxDynamicCast(FindWindow(myID_TARGET_SEARCH_BTN), wxButton);
        if (btn) btn->Enable(true);
        break;
    case SEARCH_THREAD_TARGET_FOUND:
        wxLogMessage(wxT("Target found: name(%s), ip(%s)"), msg.name, msg.ip);
        break;
    default:
        break;
    }
}
