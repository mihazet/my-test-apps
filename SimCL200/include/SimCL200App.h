#ifndef _SIMCL200APP_H_
#define _SIMCL200APP_H_

#include <wx/wx.h>

class SimCL200App: public wxApp
{    
public:
    SimCL200App();
    virtual bool OnInit();
    virtual int OnExit();

private:
    void Init();

    DECLARE_CLASS(SimCL200App)
    DECLARE_EVENT_TABLE()
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(SimCL200App)
////@end declare app

#endif
    // _SIMCL200APP_H_
