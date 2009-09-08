/////////////////////////////////////////////////////////////////////////////
// Based on led.cpp implementation in awxLed developed by Joachim Buermann.
// Copyright (c) 2001 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include "led.h"
#include "img/leds.xpm"

BEGIN_EVENT_TABLE(awxLed, wxWindow)
    EVT_ERASE_BACKGROUND(awxLed::OnErase)
    EVT_PAINT(awxLed::OnPaint)
    EVT_SIZE(awxLed::OnSize)
END_EVENT_TABLE()

BlinkTimer *awxLed::m_timer = NULL;
wxVector<awxLed *> awxLed::m_allLeds;
wxIcon *awxLed::m_icons[awxLED_COLOR_INVALID] = { NULL, NULL, NULL, NULL };
int awxLed::m_blink = 0;

awxLed::awxLed(wxWindow *parent, wxWindowID id, awxLedState state,
               awxLedColor color1, awxLedColor color2)
{
    Create(parent, id, state, color1, color2);
}

awxLed::awxLed(wxWindow *parent, wxWindowID id, int stateColorsPair)
{
    awxLedState state = awxLED_STATE_FIX;
    awxLedColor color[2] = { awxLED_COLOR_BLANK, awxLED_COLOR_BLANK };
    GetStateColorsPair(stateColorsPair, &state, &color[0], &color[1]);   
    Create(parent, id, state, color[0], color[1]);
}

bool awxLed::Create(wxWindow *parent, wxWindowID id, awxLedState state,
               awxLedColor color1, awxLedColor color2)
{
    m_state = state;
    m_color[0] = color1;
    m_color[1] = color2;
    m_bitmap = new wxBitmap(16,16);
    m_blink = 0;
    m_x = m_y = 0;

    /* create timer in first awxLed ctor */
    if (m_allLeds.empty())
    {
        m_icons[awxLED_COLOR_BLANK] = new wxIcon(led_off_xpm);
        m_icons[awxLED_COLOR_RED] = new wxIcon(led_red_xpm);
        m_icons[awxLED_COLOR_GREEN] = new wxIcon(led_green_xpm);
        m_icons[awxLED_COLOR_YELLOW] = new wxIcon(led_yellow_xpm);
        m_timer = new BlinkTimer(this);
    }

    if ((state == awxLED_STATE_BLINK) && !m_timer->IsRunning())
        m_timer->Start(500);

    /* add itself into the global awxLed vector */
    m_allLeds.push_back(this);

    return wxWindow::Create(parent, id, wxDefaultPosition, wxSize(16, 16),
        wxNO_FULL_REPAINT_ON_RESIZE);
}

awxLed::~awxLed()
{
    /* remove itself from the global awxLed vector */
    for (wxVector<awxLed *>::iterator it = m_allLeds.begin();
        it != m_allLeds.end();
        ++it)
    {
        if (*it == this)
        {
            m_allLeds.erase(it);
            break;
        }
    }

    /* delete timer in last awxLed dtor */
    if (m_allLeds.empty())
    {
        if (m_timer)
        {
            m_timer->Stop();
            delete m_timer;
        }

        delete m_icons[awxLED_COLOR_BLANK];
        delete m_icons[awxLED_COLOR_RED];
        delete m_icons[awxLED_COLOR_GREEN];
        delete m_icons[awxLED_COLOR_YELLOW];
    }

    delete m_bitmap;    
};

void awxLed::DrawOnBitmap()
{
    wxSize s = GetClientSize();
    if ((m_bitmap->GetWidth() != s.GetWidth())
        || (m_bitmap->GetHeight() != s.GetHeight()))
    {
	   m_bitmap->Create(s.x, s.y);
    }
    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);

    wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE), wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();

    if (m_state == awxLED_STATE_BLINK)
        dc.DrawIcon(*m_icons[m_color[m_blink]], m_x, m_y);
    else
        dc.DrawIcon(*m_icons[m_color[0]], m_x, m_y);

    dc.SelectObject(wxNullBitmap);
};

void awxLed::UpdateStateAndColors(awxLedState state, awxLedColor color1,
                                  awxLedColor color2) 
{
    bool stopTimer = true;

    if ((m_state != state) || (m_color[0] != color1) || (m_color[1] != color2))
    {
        m_state = state;
        m_color[0] = color1;
        m_color[1] = color2;

        if ((m_state == awxLED_STATE_BLINK) && !m_timer->IsRunning())
	        m_timer->Start(500);
        else if ((m_state == awxLED_STATE_FIX) && m_timer->IsRunning())
        {
            for (wxVector<awxLed *>::iterator it = m_allLeds.begin();
                it != m_allLeds.end();
                ++it)
            {
                if ((*it)->m_state == awxLED_STATE_BLINK)
                {
                    stopTimer = false;
                    break;
                }
            }

            if (stopTimer)
                m_timer->Stop();
        }

        Redraw();
    }
};

void awxLed::UpdateStateAndColors(int stateColorsPair)
{
    awxLedState state;
    awxLedColor color[2];
    GetStateColorsPair(stateColorsPair, &state, &color[0], &color[1]);
    UpdateStateAndColors(state, color[0], color[1]);
}

