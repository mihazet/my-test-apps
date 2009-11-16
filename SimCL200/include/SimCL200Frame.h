#ifndef _SIMCL200FRAME_H_
#define _SIMCL200FRAME_H_

#include <wx/wx.h>

class SimCL200Frame: public wxFrame
{    

public:
    /// Constructors
    SimCL200Frame();
    SimCL200Frame(wxWindow *parent, wxWindowID id = wxID_ANY,
                  const wxString &caption = wxT("Chroma meter CL-200 RS232 Simulator"),
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxDEFAULT_FRAME_STYLE);

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
                const wxString &caption = wxT("Chroma meter CL-200 RS232 Simulator"),
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE);

    /// Destructor
    ~SimCL200Frame();

private:
    void Init();
    void CreateControls();

    DECLARE_CLASS( SimCL200Frame )
    DECLARE_EVENT_TABLE()
};

#endif
    // _SIMCL200FRAME_H_
