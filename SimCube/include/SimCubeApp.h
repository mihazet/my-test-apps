#ifndef _SIM_CUBE_APP_H_
#define _SIM_CUBE_APP_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>
#include <wx/snglinst.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#elif defined (__WXGTK__)
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#endif
#include "NetAdapter.h"
#include "PeerPane.h"
#include "HistoryPane.h"
#include "UDPProtocol.h"
#include "TCPProtocol.h"

////////////////////////////////////////////////////////////////////////////
class SimCubeStatusBar : public wxStatusBar
{
public:
    SimCubeStatusBar() { Init(); }
    SimCubeStatusBar(wxWindow *parent, long style = wxSTB_DEFAULT_STYLE);
    virtual ~SimCubeStatusBar();
    bool Create(wxWindow *parent, long style = wxSTB_DEFAULT_STYLE);

private:
    void Init();

    /* event handlers */
    void OnSize(wxSizeEvent &event);

    DECLARE_EVENT_TABLE()
};
////////////////////////////////////////////////////////////////////////////
class SimCubeApp : public wxApp
{
public:
    SimCubeApp();
    ~SimCubeApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();

    wxSQLite3Database *GetMainDatabase() { return _mainDB; }
    wxSQLite3Database *GetMemDatabase() { return _memDB; }
    wxVector<NetAdapter> m_Adapters;
    PeerDataModel *m_PeerData;
    HistoryDataModel *m_HistoryData;
    SimCubeStatusBar *m_StatusBar;

private:
    // event handlers
    void OnAppIdle(wxIdleEvent &event);

    // internal member functions
    bool CheckRockey();
    bool DetectNetAdapter(bool *changed = NULL);
    wxString CalculateSubnetBroadcastAddress(wxString, wxString);
    void InitMainDatabase();

private:
    // internal data members
    wxSQLite3Database *_mainDB;
    wxSQLite3Database *_memDB;
    wxLanguage _lang;
    wxLocale *_locale;
    wxSingleInstanceChecker *_onlyMe;
#ifdef __WXMSW__
    IP_ADAPTER_INFO *_adapterInfo;
#elif defined (__WXGTK__)
    struct ifreq *_adapterInfo;
#endif
    UDPProtocol *_udpProtocol;
    TCPProtocol *_tcpProtocol;
    bool _usingRockey;

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(SimCubeApp)

#endif /* _SIM_CUBE_APP_H_ */

