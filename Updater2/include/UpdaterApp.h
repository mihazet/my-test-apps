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

class ReportDataModel;
class AppLanguageInfo;

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
    wxString GetLanguageDescriptionFromISO639Code(const wxString &iso639);
    wxString GetLanguageISO639CodeFromDescription(const wxString &desc);
    wxVector<wxString> GetInstalledLanguages();

    wxVector<NetAdapter> m_Adapters;
    AppOptions *m_pAppOptions;
    wxAuiManager m_AuiManager;
    int m_UpdateThreadCount;
    bool m_SearchThreadRunning;
    ReportDataModel *m_reportModel;
    enum
    {
        KEY_OPTION,
        KEY_REPORT,
        KEY_MAX
    };
    wxString m_encryptionKey[KEY_MAX];

private:
    // event handlers
    void OnAppIdle(wxIdleEvent &event);

    // internal member functions
    bool DetectNetAdapter(bool *changed = NULL);
    wxString CalculateSubnetBroadcastAddress(wxString, wxString);
    void AddSupportedLanguages();
    void AddLanguageDescriptions();
    void AddSupportedLanguage(wxLanguage langId, const wxString &iso639);
    void AddLanguageDescription(wxLanguage langId, const wxString &desc);
    wxLanguage DetectInstalledLanguages();
    void ProcessCmdLine();

private:
    // internal data members
    wxVector<AppLanguageInfo *> _appLanguages;
    wxLocale _locale;
    bool _skipVmwareNetworkAdapter;

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(UpdaterApp)

#endif /* _SIM_CUBE_APP_H_ */
