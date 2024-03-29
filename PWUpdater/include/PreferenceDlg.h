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
    int LanguageLoad();
    int LanguageSave();
    int InterfaceLoad();
    int InterfaceSave();
    int TftpRootLoad();
    int TftpRootSave();
    int SoundFileLoad(bool success);
    int SoundFileSave(bool success);
    int NetAddrLoad(const wxWindowID id, const wxString &opt);
    int NetAddrSave(const wxWindowID id, const wxString &opt);
    int SpinCtrlLoad(const wxWindowID id, const wxString &opt);
    int SpinCtrlSave(const wxWindowID id, const wxString &opt);

    DECLARE_EVENT_TABLE()
};

#endif /* _PREFERENCE_DLG_H_ */

