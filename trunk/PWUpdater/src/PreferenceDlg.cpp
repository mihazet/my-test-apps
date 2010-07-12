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
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include "AppOptions.h"
#include "PreferenceDlg.h"

#define wxLOG_COMPONENT "PWUpdater/pref"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
enum
{
    ID_UI_LANG = wxID_HIGHEST + 1,
    ID_UI_LAYOUT_MEMORY,
    ID_UI_POS_SIZE_MEMORY,
    ID_TFTP_AUTOSTART,
    ID_TFTP_INTERFACE,
    ID_TFTP_ROOTPATH,
    ID_TFTP_TIMEOUT,
    ID_TFTP_RETRANSMIT,
    ID_TFTP_NEGOTIATION,
    ID_FLASH_DL_OFFSET,
    ID_FLASH_SPI_UBOOT_OFFSET,
    ID_FLASH_SPI_UBOOT_IMAGE,
    ID_FLASH_UBOOT_OFFSET,
    ID_FLASH_UBOOT_IMAGE,
    ID_FLASH_KERNEL_OFFSET,
    ID_FLASH_KERNEL_IMAGE,
    ID_FLASH_FS_OFFSET,
    ID_FLASH_FS_IMAGE,
    ID_FLASH_SPLASH_OFFSET,
    ID_FLASH_SPLASH_IMAGE,
};

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

    /* ui page */
    wxPanel *uiPage = new wxPanel(prefNB, wxID_ANY);

    wxStaticBoxSizer *langSizer = new wxStaticBoxSizer(wxVERTICAL, uiPage, _("Language selection"));
    langSizer->Add(new wxChoice(uiPage, ID_UI_LANG), 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *memSizer = new wxStaticBoxSizer(wxVERTICAL, uiPage, _("Memory"));
    memSizer->Add(new wxCheckBox(uiPage, ID_UI_LAYOUT_MEMORY, _("Remember application layout.")), 0, wxALL, 5);
    memSizer->Add(new wxCheckBox(uiPage, ID_UI_POS_SIZE_MEMORY, _("Remember application size and position.")), 0, wxALL, 5);
    
    wxBoxSizer *uiSizer = new wxBoxSizer(wxVERTICAL);
    uiSizer->Add(langSizer, 0, wxALL | wxEXPAND, 5);
    uiSizer->Add(memSizer, 0, wxALL | wxEXPAND, 5);
    uiPage->SetSizer(uiSizer);

    /* tftp server page */
    wxPanel *tftpPage = new wxPanel(prefNB, wxID_ANY);

    wxStaticBoxSizer *bgServiceSizer = new wxStaticBoxSizer(wxVERTICAL, tftpPage, _("Background service"));
    bgServiceSizer->Add(new wxCheckBox(tftpPage, ID_TFTP_AUTOSTART, _("Enable build-in TFTPD server.")), 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *tftpOptSizer = new wxStaticBoxSizer(wxVERTICAL, tftpPage, _("Tftp server option"));
    wxGridBagSizer *optGridSizer = new wxGridBagSizer(5, 5);
    optGridSizer->AddGrowableCol(0);
    wxGBPosition pos;
    wxGBSpan span;
    pos.SetRow(0);
    pos.SetCol(0);
    span.SetColspan(2);
    optGridSizer->Add(new wxStaticText(tftpPage, wxID_STATIC, _("Bind tftp service on this interface:")), pos, span, wxALIGN_CENTER_VERTICAL);
    pos.SetRow(pos.GetRow() + 1);
    optGridSizer->Add(new wxChoice(tftpPage, ID_TFTP_INTERFACE), pos, span, wxEXPAND);
    pos.SetRow(pos.GetRow() + 1);
    optGridSizer->Add(new wxStaticText(tftpPage, wxID_STATIC, _("Tftp server root path:")), pos, span, wxALIGN_CENTER_VERTICAL);
    pos.SetRow(pos.GetRow() + 1);
    optGridSizer->Add(new wxDirPickerCtrl(tftpPage, ID_TFTP_ROOTPATH, wxEmptyString, wxDirSelectorPromptStr, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE & ~wxDIRP_DIR_MUST_EXIST), pos, span, wxEXPAND);
    pos.SetRow(pos.GetRow() + 1);
    span.SetColspan(1);
    optGridSizer->Add(new wxStaticText(tftpPage, wxID_STATIC, _("Transmit timeout (unit: second)")), pos, span, wxALIGN_CENTER_VERTICAL);
    pos.SetCol(1);
    optGridSizer->Add(new wxTextCtrl(tftpPage, ID_TFTP_TIMEOUT), pos, span, wxALIGN_RIGHT);
    pos.SetRow(pos.GetRow() + 1);
    pos.SetCol(0);
    optGridSizer->Add(new wxStaticText(tftpPage, wxID_STATIC, _("Retransmit maximum count")), pos, span, wxALIGN_CENTER_VERTICAL);
    pos.SetCol(1);
    optGridSizer->Add(new wxTextCtrl(tftpPage, ID_TFTP_RETRANSMIT), pos, span, wxALIGN_RIGHT);
    pos.SetRow(pos.GetRow() + 1);
    pos.SetCol(0);
    span.SetColspan(2);
    optGridSizer->Add(new wxCheckBox(tftpPage, ID_TFTP_NEGOTIATION, _("Allow option negotiation. (See RFC2347, RFC2348 and RFC2349 for detail)")), pos, span);
    tftpOptSizer->Add(optGridSizer, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *tftpSizer = new wxBoxSizer(wxVERTICAL);
    tftpSizer->Add(bgServiceSizer, 0, wxALL | wxEXPAND, 5);
    tftpSizer->Add(tftpOptSizer, 0, wxALL | wxEXPAND, 5);
    tftpPage->SetSizer(tftpSizer);

    /* flash chip page */
    wxPanel *flashPage = new wxPanel(prefNB, wxID_ANY);

    wxStaticBoxSizer *ddrSizer = new wxStaticBoxSizer(wxVERTICAL, flashPage, _("DDR memory layout"));
    wxFlexGridSizer *ddrGridSizer = new wxFlexGridSizer(2, 1, 5);
    ddrGridSizer->AddGrowableCol(0, 1);
    ddrGridSizer->AddSpacer(0);
    ddrGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("offset")), 0, wxALL | wxALIGN_CENTER, 0);
    ddrGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Temporary storage")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    ddrGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_DL_OFFSET), 0, wxALL, 0);
    ddrSizer->Add(ddrGridSizer, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *norSizer = new wxStaticBoxSizer(wxVERTICAL, flashPage, _("NOR flash memory layout"));
    wxFlexGridSizer *norGridSizer = new wxFlexGridSizer(3, 1, 5);
    norGridSizer->AddGrowableCol(0, 1);
    norGridSizer->AddSpacer(0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("name")), 0, wxALL | wxALIGN_CENTER, 0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("offset")), 0, wxALL | wxALIGN_CENTER, 0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Bootloader")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_UBOOT_IMAGE), 0, wxALL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_UBOOT_OFFSET), 0, wxALL, 0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Linux Kernel")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_KERNEL_IMAGE), 0, wxALL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_KERNEL_OFFSET), 0, wxALL, 0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Root File System")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_FS_IMAGE), 0, wxALL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_FS_OFFSET), 0, wxALL, 0);
    norGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Splash")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_SPLASH_IMAGE), 0, wxALL, 0);
    norGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_SPLASH_OFFSET), 0, wxALL, 0);
    norSizer->Add(norGridSizer, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *spiSizer = new wxStaticBoxSizer(wxVERTICAL, flashPage, _("SPI flash memory layout"));
    wxFlexGridSizer *spiGridSizer = new wxFlexGridSizer(3, 1, 5);
    spiGridSizer->AddGrowableCol(0, 1);
    spiGridSizer->AddSpacer(0);
    spiGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("name")), 0, wxALL | wxALIGN_CENTER, 0);
    spiGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("offset")), 0, wxALL | wxALIGN_CENTER, 0);
    spiGridSizer->Add(new wxStaticText(flashPage, wxID_STATIC, _("Bootloader")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    spiGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_SPI_UBOOT_IMAGE), 0, wxALL, 0);
    spiGridSizer->Add(new wxTextCtrl(flashPage, ID_FLASH_SPI_UBOOT_OFFSET), 0, wxALL, 0);
    spiSizer->Add(spiGridSizer, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer *flashSizer = new wxBoxSizer(wxVERTICAL);
    flashSizer->Add(ddrSizer, 0, wxALL | wxEXPAND, 5);
    flashSizer->Add(norSizer, 0, wxALL | wxEXPAND, 5);
    flashSizer->Add(spiSizer, 0, wxALL | wxEXPAND, 5);
    flashPage->SetSizer(flashSizer);

    /* pages organization */
    prefNB->AddPage(uiPage, _("User interface"), true);
    prefNB->AddPage(tftpPage, _("Tftp server"), false);
    prefNB->AddPage(flashPage, _("Flash chip"), false);

    /* dialog organization */
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