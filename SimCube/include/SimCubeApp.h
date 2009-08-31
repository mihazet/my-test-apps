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

/* enable or disable software protection by using rockey4nd usb dongle */
#undef PROTECTED_BY_ROCKEY4_USB_DONGLE

class SimCubeApp : public wxApp
{
public:
    SimCubeApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();

    wxSQLite3Database *GetMainDatabase() { return _mainDB; }
    wxSQLite3Database *GetMemDatabase() { return _memDB; }
    wxVector<NetAdapter> m_Adapters;
    PeerDataModel *m_PeerData;
    HistoryDataModel *m_HistoryData;

private:
    // event handlers
#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    void OnAppIdle(wxIdleEvent &event);
#endif

    // internal member functions
#ifdef PROTECTED_BY_ROCKEY4_USB_DONGLE
    bool CheckRockey();
#endif
    bool DetectNetAdapter(bool *changed = NULL);
    wxString CalculateSubnetBroadcastAddress(wxString, wxString);

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

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(SimCubeApp)

#endif /* _SIM_CUBE_APP_H_ */

