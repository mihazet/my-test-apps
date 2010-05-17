#ifndef _UPDATER_APP_H_
#define _UPDATER_APP_H_

#include <wx/wx.h>
#include <wx/vector.h>
#include <wx/aui/framemanager.h>
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
#include "AppOptions.h"

////////////////////////////////////////////////////////////////////////////
class UpdaterApp : public wxApp
{
public:
    UpdaterApp();
    ~UpdaterApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();
    long HalfMAC2Long(const wxString& halfMAC);
    long long FullMAC2LongLong(const wxString& fullMAC);

    wxVector<NetAdapter> m_Adapters;
    AppOptions *m_pAppOptions;
    wxAuiManager m_AuiManager;
    int m_UpdateThreadCount;
    bool m_SearchThreadRunning;

private:
    // event handlers
    void OnAppIdle(wxIdleEvent &event);

    // internal member functions
    bool DetectNetAdapter(bool *changed = NULL);
    wxString CalculateSubnetBroadcastAddress(wxString, wxString);

private:
    // internal data members
    wxLanguage _lang;
    wxLocale _locale;
#ifdef __WXMSW__
    IP_ADAPTER_INFO *_adapterInfo;
#elif defined (__WXGTK__)
    struct ifreq *_adapterInfo;
#endif
    bool _skipVmwareNetworkAdapter;

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(UpdaterApp)

#endif /* _SIM_CUBE_APP_H_ */
