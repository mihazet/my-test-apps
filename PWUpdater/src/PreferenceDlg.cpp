/*
 *  PreferenceDlg.cpp - System-wide configuration management.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "PreferenceDlg.h"

#define wxLOG_COMPONENT "PWUpdater/pref"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(PrefDlg, wxDialog)
END_EVENT_TABLE()

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
PrefDlg::PrefDlg(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, _("Preference"), wxDefaultPosition,
      wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
    wxBoxSizer *dlgSizer = new wxBoxSizer(wxVERTICAL);

    dlgSizer->Add(new wxButton(this, wxID_ANY, _("PressMe")), 1, wxALL | wxEXPAND , 5);

    /* standard dialog button sizer */
    dlgSizer->Add(
        CreateStdDialogButtonSizer(wxOK|wxCANCEL|wxAPPLY),
        0, wxALL | wxEXPAND, 5);

    SetSizerAndFit(dlgSizer);
}

PrefDlg::~PrefDlg()
{

}

bool PrefDlg::TransferDataFromWindow()
{
    wxLogMessage(wxT("transfer data from window"));

    return true;
}

bool PrefDlg::TransferDataToWindow()
{
    wxLogMessage(wxT("transfer data to window"));

    return true;
}
