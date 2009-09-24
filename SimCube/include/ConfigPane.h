#ifndef _CONFIG_PANE_H_
#define _CONFIG_PANE_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class ConfigPane : public wxPanel
{
public:
    // ctor
    ConfigPane();
    ConfigPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~ConfigPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

private:
    void Init();
    void CreateControls();

    /* event handlers */
    void OnLangDefault(wxCommandEvent &event);
    void OnUpdateLangSelect(wxUpdateUIEvent &event);
    void OnLangSelect(wxCommandEvent &event);
    void OnAutoSaveHistory(wxCommandEvent &event);
    void OnUsingRockey4ND(wxCommandEvent &event);

    /* helper functions */
    void TransferToDatabase(int);
    void TransferFromDatabase(void);

    wxSQLite3Database *_cfgDB;
    enum
    {
        eLANG_DEFAULT,
        eLANG_ENGLISH,
        eLANG_CHINESE_TRADITIONAL,
        eLANG_CHINESE_SIMPLIFIED,
    } _language;
    bool _autoSave;
    bool _rockey;

    DECLARE_EVENT_TABLE()
};

#endif /* _CONFIG_PANE_H_ */

