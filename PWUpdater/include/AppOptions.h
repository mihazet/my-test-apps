/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_

#include <wx/wx.h>
#include <wx/wxsqlite3.h>

class AppOptions
{
public:
    AppOptions();
    ~AppOptions();

    /* accessor */
    bool SetOption(const wxString &option, const wxString &value);
    bool SetOption(const wxString &option, const long value);
    bool SetOption(const wxString &option, const bool value);

    bool GetOption(const wxString &option, wxString &value);
    wxString GetOption(const wxString &option);
    long GetOption(const wxString &option, long *value);
    bool GetOption(const wxString &option, bool *value);

private:
    void DB_Init();

    wxSQLite3Database *_db;
};

#endif /* _APP_OPTIONS_H_ */
