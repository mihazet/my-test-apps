/////////////////////////////////////////////////////////////////////////////
// Name:        tpaaapp.cpp
// Purpose:     
// Author:      yhuiyang@gmail.com
// Modified by: 
// Created:     12/15/2008 5:20:47 PM
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

#include "../include/tpaaapp.h"

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( TPAAApp )
////@end implement app


/*!
 * TPAAApp type definition
 */

IMPLEMENT_CLASS( TPAAApp, wxApp )


/*!
 * TPAAApp event table definition
 */

BEGIN_EVENT_TABLE( TPAAApp, wxApp )

////@begin TPAAApp event table entries
////@end TPAAApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for TPAAApp
 */

TPAAApp::TPAAApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void TPAAApp::Init()
{
////@begin TPAAApp member initialisation
////@end TPAAApp member initialisation
}

/*!
 * Initialisation for TPAAApp
 */

bool TPAAApp::OnInit()
{    
////@begin TPAAApp initialisation
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
	ConfigAADlg* mainWindow = new ConfigAADlg(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
////@end TPAAApp initialisation

    return true;
}


/*!
 * Cleanup for TPAAApp
 */

int TPAAApp::OnExit()
{    
////@begin TPAAApp cleanup
	return wxApp::OnExit();
////@end TPAAApp cleanup
}

