/////////////////////////////////////////////////////////////////////////////
// Name:        senduartapp.cpp
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/21/2008 2:10:13 PM
// RCS-ID:      
// Copyright:   Copyright 2008, Delta Electronics, Inc. All rights reserved.
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

#include "senduartapp.h"

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( SendUartApp )
////@end implement app


/*!
 * SendUartApp type definition
 */

IMPLEMENT_CLASS( SendUartApp, wxApp )


/*!
 * SendUartApp event table definition
 */

BEGIN_EVENT_TABLE( SendUartApp, wxApp )

////@begin SendUartApp event table entries
////@end SendUartApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for SendUartApp
 */

SendUartApp::SendUartApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void SendUartApp::Init()
{
////@begin SendUartApp member initialisation
	m_appConfig = NULL;
////@end SendUartApp member initialisation
}

/*!
 * Initialisation for SendUartApp
 */

bool SendUartApp::OnInit()
{
    GenDefaultAppConfig();
////@begin SendUartApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	SendUart* mainWindow = new SendUart(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
////@end SendUartApp initialisation

    return true;
}


/*!
 * Cleanup for SendUartApp
 */

int SendUartApp::OnExit()
{    
////@begin SendUartApp cleanup
	return wxApp::OnExit();
////@end SendUartApp cleanup
}


/*!
 * generate default app config if no config exists
 */

void SendUartApp::GenDefaultAppConfig(void)
{
    wxString str;
    long order = 1;

    wxGetApp().SetVendorName(wxT("Delta Electronics, Inc."));
    if (!m_appConfig)
        m_appConfig = new wxConfig(wxGetApp().GetClassName(), wxGetApp().GetVendorName());
    
    m_appConfig->SetPath(wxT("/CommandGroup"));
    if (!m_appConfig->Read(wxT("ActiveGroup"), &str, wxT("VW5129AF")))
        m_appConfig->Write(wxT("ActiveGroup"), str);
        
    if (!m_appConfig->HasGroup(wxT("VW5129AF")))
    {
        m_appConfig->SetPath(wxT("VW5129AF"));
        InitAppConfigForNumeric(wxT("Contrast"), order++, 0x00000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Brightness"), order++, 0x01000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Color"), order++, 0x02000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Tint"), order++, 0x03000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Sharpness"), order++, 0x04000000L, 6L, 0L);
        InitAppConfigForNumeric(wxT("R.CutOff"), order++, 0x05000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("G.CutOff"), order++, 0x06000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("B.CutOff"), order++, 0x07000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("R.Drive"), order++, 0x08000000L, 200L, 0L);
        InitAppConfigForNumeric(wxT("G.Drive"), order++, 0x09000000L, 200L, 0L);
        InitAppConfigForNumeric(wxT("B.Drive"), order++, 0x0A000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("R.InputGain"), order++, 0x0B000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("G.InputGain"), order++, 0x0C000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("B.InputGain"), order++, 0x0D000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("R.InputOffset"), order++, 0x0E000000L, 127L, 0L);
        InitAppConfigForNumeric(wxT("G.InputOffset"), order++, 0x0F000000L, 127L, 0L);
        InitAppConfigForNumeric(wxT("B.InputOffset"), order++, 0x10000000L, 127L, 0L);
        InitAppConfigForNumeric(wxT("ImageOrientation"), order++, 0x11000000L, 4L, 0L);
        InitAppConfigForPairNumeric(wxT("H-V Position"), order++, 0x12000000L, 15L, -15L, 15L, -15L);
        InitAppConfigForNumeric(wxT("Frequency"), order++, 0x13000000L, 100L, -100L);
        InitAppConfigForNumeric(wxT("Phase"), order++, 0x14000000L, 15L, -16L);
        InitAppConfigForNumeric2Byte(wxT("H-Capture"), order++, 0x15000000L, 500L, -500L);
        InitAppConfigForNumeric2Byte(wxT("V-Capture"), order++, 0x16000000L, 300L, -300L);
        InitAppConfigForNumericHex(wxT("AllTilingNumber"), order++, 0x17000000L, 136L, 0L);
        InitAppConfigForNumericHex(wxT("ProjectorRowCol"), order++, 0x18000000L, 136L, 0L);
        InitAppConfigForNumeric(wxT("Baud Rate"), order++, 0x19000000L, 3L, 0L);
        InitAppConfigForNumeric(wxT("Test Pattern"), order++, 0x1A000000L, 59L, 0L);
        InitAppConfigForNumeric(wxT("R.Primary Red"), order++, 0x1B000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("R.Primary Green"), order++, 0x1C000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("R.Primary Blue"), order++, 0x1D000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("G.Primary Red"), order++, 0x1E000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("G.Primary Green"), order++, 0x1F000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("G.Primary Blue"), order++, 0x20000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("B.Primary Red"), order++, 0x21000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("B.Primary Green"), order++, 0x22000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("B.Primary Blue"), order++, 0x23000000L, 100L, 0L);
        InitAppConfigForNumeric(wxT("Lamp Select"), order++, 0x2B000000L, 2L, 0L);
        InitAppConfigForNumeric(wxT("Lamp A Power"), order++, 0x2C000000L, 5L, 0L);
        InitAppConfigForNumeric(wxT("Freeze"), order++, 0x2D000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Index Delay"), order++, 0x2E000000L, 359L, 0L);
        InitAppConfigForNumeric(wxT("Color Temp."), order++, 0x30000000L, 4L, 0L);
        InitAppConfigForNumeric(wxT("Video Contrast"), order++, 0x31000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Video Brightness"), order++, 0x32000000L, 255L, 0L);
        InitAppConfigForNumeric(wxT("Aspect Ratio"), order++, 0x33000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Main Input Select"), order++, 0x34000000L, 10L, 0L);
        InitAppConfigForNumeric(wxT("Language"), order++, 0x37000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Menu H Position"), order++, 0x38000000L, 50L, 0L);
        InitAppConfigForNumeric(wxT("Menu V Position"), order++, 0x39000000L, 50L, 0L);
        InitAppConfigForNumeric(wxT("Reset All Setting"), order++, 0x3A000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("CCA"), order++, 0x40000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Force Timing"), order++, 0x41000000L, 2L, 0L);
        InitAppConfigForNumeric(wxT("Video Standard"), order++, 0x42000000L, 5L, 0L);
        InitAppConfigForNumeric(wxT("Input Video Black"), order++, 0x43000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Gamma"), order++, 0x44000000L, 5L, 0L);
        InitAppConfigForNumeric(wxT("Projector ID"), order++, 0x45000000L, 99L, 0L);
        InitAppConfigForNumeric(wxT("Power Delay"), order++, 0x46000000L, 10L, 0L);
        InitAppConfigForNumeric(wxT("Frame Lock"), order++, 0x47000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("RGB First Auto"), order++, 0x48000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("AGC"), order++, 0x49000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Hot Swap"), order++, 0x4A000000L, 2L, 0L);
        InitAppConfigForPairNumeric(wxT("User Mode"), order++, 0x4B000000L, 30L, 1L, 1L, 0L);
        InitAppConfigForNumeric(wxT("OSD OnOff"), order++, 0x4C000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("Lamp B Power"), order++, 0x4D000000L, 5L, 0L);
        InitAppConfigForNumeric(wxT("Lamp Auto Switch"), order++, 0x4E000000L, 1L, 0L);
        InitAppConfigForNumeric(wxT("White Peaking"), order++, 0x4F000000L, 100L, 0L);
    }
}
void SendUartApp::InitAppConfigForNumeric(wxString prop, long idx, long code, long max, long min)
{
    wxString str = wxString::Format(wxT("%03lu"), idx);
    m_appConfig->Write(str + wxT("/Property"), prop);
    m_appConfig->Write(str + wxT("/Type"), wxT("Numeric"));
    m_appConfig->Write(str + wxT("/Code"), code);
    m_appConfig->Write(str + wxT("/Max"), max);
    m_appConfig->Write(str + wxT("/Min"), min);
}
void SendUartApp::InitAppConfigForPairNumeric(wxString prop, long idx, long code, long max1, long min1, long max2, long min2)
{
    wxString str = wxString::Format(wxT("%03lu"), idx);
    m_appConfig->Write(str + wxT("/Property"), prop);
    m_appConfig->Write(str + wxT("/Type"), wxT("Pair"));
    m_appConfig->Write(str + wxT("/Code"), code);
    m_appConfig->Write(str + wxT("/Max1"), max1);
    m_appConfig->Write(str + wxT("/Min1"), min1);
    m_appConfig->Write(str + wxT("/Max2"), max2);
    m_appConfig->Write(str + wxT("/Min2"), min2);
}
void SendUartApp::InitAppConfigForNumeric2Byte(wxString prop, long idx, long code, long max, long min)
{
    wxString str = wxString::Format(wxT("%03lu"), idx);
    m_appConfig->Write(str + wxT("/Property"), prop);
    m_appConfig->Write(str + wxT("/Type"), wxT("Numeric2Byte"));
    m_appConfig->Write(str + wxT("/Code"), code);
    m_appConfig->Write(str + wxT("/Max"), max);
    m_appConfig->Write(str + wxT("/Min"), min);
}
void SendUartApp::InitAppConfigForNumericHex(wxString prop, long idx, long code, long max, long min)
{
    wxString str = wxString::Format(wxT("%03lu"), idx);
    m_appConfig->Write(str + wxT("/Property"), prop);
    m_appConfig->Write(str + wxT("/Type"), wxT("NumericHex"));
    m_appConfig->Write(str + wxT("/Code"), code);
    m_appConfig->Write(str + wxT("/Max"), max);
    m_appConfig->Write(str + wxT("/Min"), min);
}
