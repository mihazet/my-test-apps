#ifndef _UPDATER_FRAME_H_
#define _UPDATER_FRAME_H_

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/aui/framemanager.h>

// status bar fields
enum
{
    SBAR_FIELD_MESSAGE,
    SBAR_FIELD_ACTIVE_INTERFACE,
    SBAR_FIELD_MAX
};

class UpdaterFrame : public wxFrame
{
public:
    // ctor
    UpdaterFrame();
    UpdaterFrame(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("Updater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

    // dtor
    ~UpdaterFrame();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("Updater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

private:
    void Init();
    void CreateControls();
    void RetrieveFrameSizeAndPosition(int *x, int *y, int *w, int *h);

    // event handlers
    void OnViewPane(wxCommandEvent &event);
    void OnUpdatePane(wxUpdateUIEvent &event);
    void OnResetLayout(wxCommandEvent &event);
    void OnEraseBackground(wxEraseEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnClose(wxCloseEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);

private:
    wxAuiManager _auiManager;

    DECLARE_EVENT_TABLE()
};

#endif /* _UPDATER_FRAME_H_ */
