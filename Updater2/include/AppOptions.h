#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class AppOptions
{
public:
    AppOptions(const wxString& name = wxT("appoptions.db"));
    ~AppOptions();

    bool SetOption(const wxString& option, const wxString& value);
    bool GetOption(const wxString& option, wxString& value);
    wxString GetOption(const wxString& option);

private:
    void Init();

    wxSQLite3Database *_db;
};

#endif /* _APP_OPTIONS_H_ */
