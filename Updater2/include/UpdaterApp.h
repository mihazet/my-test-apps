#ifndef _UPDATER_APP_H_
#define _UPDATER_APP_H_

#include <wx/wx.h>
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

////////////////////////////////////////////////////////////////////////////
class UpdaterApp : public wxApp
{
public:
    UpdaterApp();
    ~UpdaterApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();

    wxVector<NetAdapter> m_Adapters;

private:
    // event handlers
    void OnAppIdle(wxIdleEvent &event);

    // internal member functions
    bool DetectNetAdapter(bool *changed = NULL);
    wxString CalculateSubnetBroadcastAddress(wxString, wxString);

private:
    // internal data members
    wxLanguage _lang;
    wxLocale *_locale;
    wxSingleInstanceChecker *_onlyMe;
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
