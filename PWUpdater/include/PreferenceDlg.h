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
    PrefDlg(wxWindow *parent);
    ~PrefDlg();


private:
    virtual bool TransferDataFromWindow();
    virtual bool TransferDataToWindow();
    int CheckBoxLoad(const wxWindowID id, const wxString &opt);
    int CheckBoxSave(const wxWindowID id, const wxString &opt);

    DECLARE_EVENT_TABLE()
};

#endif /* _PREFERENCE_DLG_H_ */

