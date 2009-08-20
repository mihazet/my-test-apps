#ifndef _SIM_CUBE_FRAME_H_
#define _SIM_CUBE_FRAME_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/aui/framemanager.h>

class SimCubeFrame : public wxFrame
{
public:
    // ctor
    SimCubeFrame();
    SimCubeFrame(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("Cube Simulator"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);
        
    // dtor
    ~SimCubeFrame();
    
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("Cube Simulator"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

private:
    void Init();
    void CreateControls();
    void RetrieveFrameSizeAndPosition(int *x, int *y, int *w, int *h);

    // event handlers
    void OnEraseBackground(wxEraseEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnClose(wxCloseEvent &event);
    
private:
    wxAuiManager _auiManager;
    
    DECLARE_EVENT_TABLE()
};

#endif /* _SIM_CUBE_FRAME_H_ */
