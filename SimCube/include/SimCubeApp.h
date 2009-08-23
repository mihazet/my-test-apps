#ifndef _SIM_CUBE_APP_H_
#define _SIM_CUBE_APP_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>
#include <wx/snglinst.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#endif
#include "NetAdapter.h"
#include "UDPProtocol.h"

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
    wxSQLite3Database *_mainDB;
    wxSQLite3Database *_memDB;
    wxLanguage _lang;
    wxLocale *_locale;
    wxSingleInstanceChecker *_onlyMe;
#ifdef __WXMSW__
    IP_ADAPTER_INFO *_adapterInfo;
#endif
    UDPProtocol *_udpProtocol;

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(SimCubeApp)

#endif /* _SIM_CUBE_APP_H_ */

