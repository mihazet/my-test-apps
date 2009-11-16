// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "SimCL200Frame.h"

enum
{
    myID_ = wxID_HIGHEST + 1,
};

IMPLEMENT_CLASS(SimCL200Frame, wxFrame)

BEGIN_EVENT_TABLE(SimCL200Frame, wxFrame)
END_EVENT_TABLE()

SimCL200Frame::SimCL200Frame()
{
    Init();
}

SimCL200Frame::SimCL200Frame(wxWindow *parent, wxWindowID id,
                             const wxString &caption, const wxPoint &pos,
                             const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool SimCL200Frame::Create(wxWindow *parent, wxWindowID id,
                           const wxString &caption, const wxPoint &pos,
                           const wxSize &size, long style)
{
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();
    Centre();
    return true;
}

SimCL200Frame::~SimCL200Frame()
{
}

void SimCL200Frame::Init()
{
}

void SimCL200Frame::CreateControls()
{
    wxBoxSizer *frameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel *basePanel = new wxPanel(this);
    frameSizer->Add(basePanel, 1, wxEXPAND | wxALL, 0);
    wxBoxSizer *basePanelSizer = new wxBoxSizer(wxVERTICAL);
    basePanel->SetSizer(basePanelSizer);
    wxButton *btn = new wxButton(basePanel, wxID_ANY, wxT("Test Button"));
    basePanelSizer->Add(btn, 0, wxEXPAND | wxALL, 5);
    wxButton *btn2 = new wxButton(basePanel, wxID_ANY, wxT("Another Button"));
    basePanelSizer->Add(btn2, 0, wxEXPAND | wxALL, 5);
    
    SetSizerAndFit(frameSizer);
}

