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
////@end SendUartApp member initialisation
}

/*!
 * Initialisation for SendUartApp
 */

bool SendUartApp::OnInit()
{    
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

