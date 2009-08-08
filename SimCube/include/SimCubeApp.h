#ifndef _SIM_CUBE_APP_H_
#define _SIM_CUBE_APP_H_

#include <wx/wx.h>

class SimCubeApp : public wxApp
{
public:
    SimCubeApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();
};

#endif /* _SIM_CUBE_APP_H_ */
