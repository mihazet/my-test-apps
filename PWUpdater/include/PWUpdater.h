/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _PWUPDATER_H_
#define _PWUPDATER_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/aui/framemanager.h>
#include <wx/vector.h>
#ifdef __WXMSW__
#include <iphlpapi.h>
#elif defined (__WXGTK__)
#include <stdio.h>
#include <dtdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#endif
#include "AppOptions.h"
#include "NetAdapter.h"

class TftpdServerThread;
class TftpdTransmissionThread;
class RockeyThread;

// ========================================================================
// Application class
// ========================================================================
class PWUpdaterApp : public wxApp
{
public:
    PWUpdaterApp() { Init(); }
    ~PWUpdaterApp() { Term(); }

    /* tftpd server thread managment */
    TftpdServerThread *m_pTftpdServerThread;
    wxCriticalSection m_serverCS;

    /* tftpd transmission threads management */
    wxVector<TftpdTransmissionThread *> m_tftpdTransmissionThreads;
    wxCriticalSection m_transmissionCS;

    /* rockey thread management */
    RockeyThread *m_pRockeyThread;
    wxCriticalSection m_rockeyCS;

    /* key info */
    wxString m_user;
    wxString m_contact;

    /* Application options */
    AppOptions *m_pOpt;
    bool m_keyFound;
    wxVector<NetAdapter> m_adapterList;

private:
    void Init();
    void Term();
    virtual bool OnInit();
    bool DetectNetAdapter();

#ifdef __WXMSW__
    IP_ADAPTER_INFO
#elif defined (__WXGTK__)
    struct ifreq
#endif
        *_adapterInfo;
};

DECLARE_APP(PWUpdaterApp)

// ========================================================================
// GUI (main frame) class
// ========================================================================
class PWUpdaterFrame : public wxFrame
{
public:
    PWUpdaterFrame() { Init(); }
    PWUpdaterFrame(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("PWUpdater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);
    ~PWUpdaterFrame();
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("PWUpdater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

    enum
    {
        STATBAR_FLD_GENERAL,
        STATBAR_FLD_TFTP,
        STATBAR_FLD_COMPORT,
        
        STATBAR_FLD_MAX
    };

private:
    /* helper functions */
    void Init();
    void CreateControls();
    void StartRockeyThread();
    
    /* event handler functions */
    void OnClose(wxCloseEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnPref(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnRockey(wxThreadEvent &event);

    /* data members */
    wxAuiManager _auiMgr;

    DECLARE_EVENT_TABLE()
};

#endif /* _PWUPDATER_H_ */

