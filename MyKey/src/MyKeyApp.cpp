/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyApp.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     23/05/2010 18:35:34
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

#include "../include/MyKeyApp.h"

////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( MyKeyApp )
////@end implement app


/*
 * MyKeyApp type definition
 */

IMPLEMENT_CLASS( MyKeyApp, wxApp )


/*
 * MyKeyApp event table definition
 */

BEGIN_EVENT_TABLE( MyKeyApp, wxApp )

////@begin MyKeyApp event table entries
////@end MyKeyApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for MyKeyApp
 */

MyKeyApp::MyKeyApp()
{
    Init();
}


/*
 * Member initialisation
 */

void MyKeyApp::Init()
{
////@begin MyKeyApp member initialisation
////@end MyKeyApp member initialisation
}

/*
 * Initialisation for MyKeyApp
 */

bool MyKeyApp::OnInit()
{    
////@begin MyKeyApp initialisation
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
////@end MyKeyApp initialisation

    return true;
}


/*
 * Cleanup for MyKeyApp
 */

int MyKeyApp::OnExit()
{    
////@begin MyKeyApp cleanup
    return wxApp::OnExit();
////@end MyKeyApp cleanup
}

