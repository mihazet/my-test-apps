/////////////////////////////////////////////////////////////////////////////
// Name:        configaadlg.cpp
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/15/2008 7:04:16 PM
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "configaadlg.h"
#include "aardvark.h"

////@begin XPM images
////@end XPM images


/*!
 * ConfigAADlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConfigAADlg, wxDialog )


/*!
 * ConfigAADlg event table definition
 */

BEGIN_EVENT_TABLE( ConfigAADlg, wxDialog )

////@begin ConfigAADlg event table entries
////@end ConfigAADlg event table entries

END_EVENT_TABLE()


/*!
 * ConfigAADlg constructors
 */

ConfigAADlg::ConfigAADlg()
{
    Init();
}

ConfigAADlg::ConfigAADlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * ConfigAADlg creator
 */

bool ConfigAADlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ConfigAADlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ConfigAADlg creation
    FindDevices();
    return true;
}


/*!
 * ConfigAADlg destructor
 */

ConfigAADlg::~ConfigAADlg()
{
////@begin ConfigAADlg destruction
////@end ConfigAADlg destruction
}


/*!
 * Member initialisation
 */

void ConfigAADlg::Init()
{
////@begin ConfigAADlg member initialisation
////@end ConfigAADlg member initialisation
}


/*!
 * Control creation for ConfigAADlg
 */

void ConfigAADlg::CreateControls()
{    
////@begin ConfigAADlg content construction
    ConfigAADlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Configure Aardvark Adapter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer5, 2, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select an Aardvark"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText6->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Verdana")));
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

    wxListCtrl* itemListCtrl7 = new wxListCtrl( itemDialog1, ID_LISTCTRL_ADAPTER_LIST, wxDefaultPosition, wxSize(520, -1), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES );
    itemBoxSizer5->Add(itemListCtrl7, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemRadioBox9Strings;
    itemRadioBox9Strings.Add(_("I2C - SPI"));
    itemRadioBox9Strings.Add(_("I2C - GPIO"));
    itemRadioBox9Strings.Add(_("SPI - GPIO"));
    itemRadioBox9Strings.Add(_("GPIO only"));
    itemRadioBox9Strings.Add(_("Batch Mode"));
    itemRadioBox9Strings.Add(_("I2C Monitor"));
    wxRadioBox* itemRadioBox9 = new wxRadioBox( itemDialog1, ID_RADIOBOX, _("Select a Mode"), wxDefaultPosition, wxDefaultSize, itemRadioBox9Strings, 6, wxRA_SPECIFY_ROWS );
    itemRadioBox9->SetSelection(0);
    itemBoxSizer8->Add(itemRadioBox9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxALIGN_LEFT|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_BUTTON_REFRESH_ADAPTERLIST, _("Refresh list"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemButton11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer12 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer12, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer12->AddButton(itemButton13);

    wxButton* itemButton14 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer12->AddButton(itemButton14);

    itemStdDialogButtonSizer12->Realize();

////@end ConfigAADlg content construction
    ModifyControls();
}


/*!
 * Should we show tooltips?
 */

bool ConfigAADlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ConfigAADlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConfigAADlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ConfigAADlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ConfigAADlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ConfigAADlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ConfigAADlg icon retrieval
}

/*!
 * Modify the GUI controls
 */

void ConfigAADlg::ModifyControls(void)
{
    wxListCtrl *pAdapterList = wxDynamicCast(FindWindow(ID_LISTCTRL_ADAPTER_LIST), wxListCtrl);
    pAdapterList->InsertColumn(0, wxT("Port"), wxLIST_FORMAT_CENTER, 40);
    pAdapterList->InsertColumn(1, wxT("UniqueID"), wxLIST_FORMAT_CENTER, 90);
    pAdapterList->InsertColumn(2, wxT("HW Ver"), wxLIST_FORMAT_CENTER, 60);
    pAdapterList->InsertColumn(3, wxT("FW Ver"), wxLIST_FORMAT_CENTER, 60);
    pAdapterList->InsertColumn(4, wxT("SW Ver"), wxLIST_FORMAT_CENTER, 60);
    pAdapterList->InsertColumn(5, wxT("I2C"), wxLIST_FORMAT_CENTER, 35);
    pAdapterList->InsertColumn(6, wxT("I2C Monitor"), wxLIST_FORMAT_CENTER, 80);
    pAdapterList->InsertColumn(7, wxT("SPI"), wxLIST_FORMAT_CENTER, 35);
    pAdapterList->InsertColumn(8, wxT("GPIO"), wxLIST_FORMAT_CENTER, 45);
}

/*!
 * Find Devices
 */

void ConfigAADlg::FindDevices(void)
{
    int nDevices;
    aa_u16 *pDevPortList = NULL;
    aa_u32 *pDevUidList = NULL;
    Aardvark aa;
    AardvarkExt aa_ext;
    wxListCtrl *pAdapterList = wxDynamicCast(FindWindow(ID_LISTCTRL_ADAPTER_LIST), wxListCtrl);
    wxListItem item;
    
    /* first call to retrieve device count in system. */
    nDevices = aa_find_devices(0, NULL);
    if (nDevices == AA_UNABLE_TO_LOAD_LIBRARY)
    {
        wxLogError(wxT("Unable to load library, please make sure aardvark.dll is shipped with the executable file."));
        return;
    }
    else if (nDevices == AA_UNABLE_TO_LOAD_DRIVER)
    {
        wxLogError(wxT("Unable to load driver, please make sure you have installed the aardvark adapter driver first."));
        return;
    }
    else if ((nDevices == AA_INCOMPATIBLE_LIBRARY) || (nDevices == AA_INCOMPATIBLE_DEVICE))
    {
        wxLogError(wxT("Version of library and firmware are in-compatible"));
        return;
    }
    else if (nDevices < 0)
    {
        wxLogError(wxT("aa_find_devices() return error = %d"), nDevices);
        return;
    }
    
    /* second call to retrieve all device port info. */
    pDevPortList = (aa_u16 *)malloc(sizeof(aa_u16) * nDevices);
    pDevUidList = (aa_u32 *)malloc(sizeof(aa_u32) * nDevices);
    nDevices = aa_find_devices_ext(nDevices, pDevPortList, nDevices, pDevUidList);
    
    /* iterate all devices */
    for (long idx = 0, listId = 0; idx < nDevices; idx++)
    {
        if (!(pDevPortList[idx] & AA_PORT_NOT_FREE)) // only open the freed devices.
        {
            aa = aa_open_ext(pDevPortList[idx], &aa_ext);
            if (aa == AA_UNABLE_TO_OPEN)
                wxLogError(wxT("Unable to open specific Aardvark adapter@Port%d."), pDevPortList[idx]);
            else if (aa == AA_INCOMPATIBLE_DEVICE)
                wxLogError(wxT("Incompatible device@Port%d."), pDevPortList[idx]);
            else // OK
            {
                /* port id */
                item.SetId(listId);
                item.SetColumn(0);
                item.SetMask(wxLIST_MASK_TEXT);
                item.SetText(wxString::Format(wxT("%d"), pDevPortList[idx] & ~AA_PORT_NOT_FREE));
                long temp = pAdapterList->InsertItem(item);
                
                if (temp == -1)
                {
                    aa_close(aa);
                    continue;
                }

                /* unique id */
                pAdapterList->SetItem(temp, 1, wxString::Format(wxT("%u-%d"), pDevUidList[idx]/1000000, pDevUidList[idx]%1000000));
                
                /* ver: hw, fw, sw */
                pAdapterList->SetItem(temp, 2, wxString::Format(wxT("v%u.%d"), aa_ext.version.hardware >> 8, aa_ext.version.hardware & 0xFF));
                pAdapterList->SetItem(temp, 3, wxString::Format(wxT("v%u.%d"), aa_ext.version.firmware >> 8, aa_ext.version.firmware & 0xFF));
                pAdapterList->SetItem(temp, 4, wxString::Format(wxT("v%u.%d"), aa_ext.version.software >> 8, aa_ext.version.software & 0xFF));

                /* feature: i2c, monitor, spi, gpio */
                pAdapterList->SetItem(temp, 5, wxString::Format(aa_ext.features & AA_FEATURE_I2C ? wxT("Yes") : wxT("No")));
                pAdapterList->SetItem(temp, 6, wxString::Format(aa_ext.features & AA_FEATURE_I2C_MONITOR ? wxT("Yes") : wxT("No")));
                pAdapterList->SetItem(temp, 7, wxString::Format(aa_ext.features & AA_FEATURE_SPI ? wxT("Yes") : wxT("No")));
                pAdapterList->SetItem(temp, 8, wxString::Format(aa_ext.features & AA_FEATURE_GPIO ? wxT("Yes") : wxT("No")));

                listId++;
                aa_close(aa);
            }
        }
    }
    
    free(pDevPortList);
    free(pDevUidList);
}
