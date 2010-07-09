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
#include <wx/notebook.h>
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
      wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxNotebook *prefNB = new wxNotebook(this, wxID_ANY);

    /* tftp server page */
    wxPanel *tftpPage = new wxPanel(prefNB, wxID_ANY);

    /* flash chip page */
    wxPanel *flashPage = new wxPanel(prefNB, wxID_ANY);

    prefNB->AddPage(tftpPage, _("Tftp server"), true);
    prefNB->AddPage(flashPage, _("Flash chip"), false);

    wxBoxSizer *dlgSizer = new wxBoxSizer(wxVERTICAL);
    dlgSizer->Add(prefNB, 1, wxALL | wxEXPAND, 5);
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
