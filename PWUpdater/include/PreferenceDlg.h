/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _PREFERENCE_DLG_H_
#define _PREFERENCE_DLG_H_

#include <wx/wx.h>
#include <wx/dialog.h>

class PrefDlg : public wxDialog
{
public:
    PrefDlg(wxWindow *parent, wxWindowID id = wxID_ANY, bool auth = false);
    ~PrefDlg();
    void AddAuthorizedPage();
    void RemoveAuthorizedPage();

private:
    void AddUiPage();
    void AddTftpPage();
    void AddFlashPage();
    void RemovePage(wxWindowID id);
    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
    int CheckBoxLoad(const wxWindowID id, const wxString &opt);
    int CheckBoxSave(const wxWindowID id, const wxString &opt);
    int TextCtrlLoad(const wxWindowID id, const wxString &opt);
    int TextCtrlSave(const wxWindowID id, const wxString &opt);
    int InterfaceLoad();
    int InterfaceSave();
    int TftpRootLoad();
    int TftpRootSave();

    DECLARE_EVENT_TABLE()
};

#endif /* _PREFERENCE_DLG_H_ */

