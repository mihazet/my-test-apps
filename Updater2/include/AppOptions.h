#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_

#include <wx/wx.h>

class AppOptions
{
public:
    AppOptions();

    bool SetOption(const wxString& option, const wxString& value);
    bool GetOption(const wxString& option, wxString& value);
    wxString GetOption(const wxString& option);
};

#endif /* _APP_OPTIONS_H_ */
