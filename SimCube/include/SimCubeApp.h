#ifndef _SIM_CUBE_APP_H_
#define _SIM_CUBE_APP_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class SimCubeApp : public wxApp
{
public:
    SimCubeApp();
    void Init();
    virtual bool OnInit();
    virtual int OnExit();
    
    wxSQLite3Database *GetMainDatabase() { return  _mainDB; }
    wxSQLite3Database *GetMemDatabase() { return _memDB; }

private:
    wxSQLite3Database *_mainDB;
    wxSQLite3Database *_memDB;
};

#endif /* _SIM_CUBE_APP_H_ */
