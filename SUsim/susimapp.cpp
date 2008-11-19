/////////////////////////////////////////////////////////////////////////////
// Name:        susimapp.cpp
// Purpose:     
// Author:      YHYang
// Modified by: 
// Created:     11/17/2008 6:53:35 PM
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

#include "susimapp.h"

////@begin XPM images

////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( SUsimApp )
////@end implement app


/*!
 * SUsimApp type definition
 */

IMPLEMENT_CLASS( SUsimApp, wxApp )


/*!
 * SUsimApp event table definition
 */

BEGIN_EVENT_TABLE( SUsimApp, wxApp )

////@begin SUsimApp event table entries
////@end SUsimApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for SUsimApp
 */

SUsimApp::SUsimApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void SUsimApp::Init()
{
////@begin SUsimApp member initialisation
////@end SUsimApp member initialisation
}

/*!
 * Initialisation for SUsimApp
 */

bool SUsimApp::OnInit()
{    
////@begin SUsimApp initialisation
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
////@end SUsimApp initialisation

    return true;
}


/*!
 * Cleanup for SUsimApp
 */

int SUsimApp::OnExit()
{    
////@begin SUsimApp cleanup
    return wxApp::OnExit();
////@end SUsimApp cleanup
}

