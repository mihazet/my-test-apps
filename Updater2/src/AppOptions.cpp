// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "AppOptions.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
AppOptions::AppOptions(const wxString& name)
{
    wxStandardPathsBase& stdPaths = wxStandardPaths::Get();
    wxString dbDir = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();

    if (NULL != (_db = new wxSQLite3Database))
    {
        _db->Open(dbDir + name);
        if (_db->IsOpen())
            Init();
    }
}

AppOptions::~AppOptions()
{
    if (_db)
    {
        if (_db->IsOpen())
            _db->Close();
        delete _db;
    }
}

void AppOptions::Init()
{
    wxString sql;

    sql << wxT("BEGIN TRANSACTION;")
        << wxT("CREATE TABLE IF NOT EXISTS AppOptions (OptionName TEXT UNIQUE, OptionValue TEXT);")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"Language\", \"Default\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"ActivePerspective\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"DefaultPerspective\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"LogVerbose\", \"0\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"SearchMethod\", \"0\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"SearchCount\", \"3\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"UseGlobalFile\", \"1\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"GlobalFilePath\", \"\");")
        << wxT("COMMIT TRANSACTION;");

    _db->ExecuteUpdate(sql);
}

bool AppOptions::SetOption(const wxString& option, const wxString& value)
{
    bool result = true;
    wxString sqlUpdate;

    sqlUpdate << wxT("UPDATE AppOptions SET OptionValue = '") << value 
        << wxT("' WHERE OptionName = '") << option << wxT("'");
    if (1 != _db->ExecuteUpdate(sqlUpdate))
        result = false;

    return result;
}

bool AppOptions::GetOption(const wxString& option, wxString& value)
{
    wxSQLite3ResultSet set;
    wxString sqlQuery;
    bool result = false;

    sqlQuery << wxT("SELECT OptionValue FROM AppOptions WHERE OptionName = '") << option << wxT("'");
    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
    {
        value = set.GetAsString(0);
        result = true;
    }
    set.Finalize();

    return result;
}

wxString AppOptions::GetOption(const wxString& option)
{
    wxString result;

    if (GetOption(option, result))
        return result;
    else
        return wxEmptyString;
}
