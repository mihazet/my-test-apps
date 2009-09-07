/////////////////////////////////////////////////////////////////////////////
// Based on led.h implementation in awxLed developed by Joachim Buermann.
// Copyright (c) 2001 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////
#ifndef _LED_H_
#define _LED_H_

#include <wx/wx.h>

enum awxLedState
{
    awxLED_STATE_FIX,
    awxLED_STATE_BLINK
};

enum awxLedColor
{
    awxLED_COLOR_BLANK,
    awxLED_COLOR_RED,
    awxLED_COLOR_GREEN,
    awxLED_COLOR_YELLOW,
    awxLED_COLOR_INVALID
};

class BlinkTimer;

class awxLed : public wxWindow
{
public:
    awxLed(wxWindow *parent, wxWindowID id, awxLedState state,
        awxLedColor color1, awxLedColor color2 = awxLED_COLOR_BLANK);
    awxLed(wxWindow *parent, wxWindowID id, int stateColorsPair);
    ~awxLed();
    awxLedState GetState() { return m_state; }
    void Blink() { Redraw(); }
    void UpdateStateAndColors(awxLedState state, awxLedColor color1,
        awxLedColor color2 = awxLED_COLOR_BLANK);
    void UpdateStateAndColors(int stateColorsPair);
    
    static wxVector<awxLed *> m_allLeds;
    static BlinkTimer *m_timer;
    static wxIcon *m_icons[awxLED_COLOR_INVALID];
    static int m_blink;

private:
    wxBitmap  *m_bitmap;
    awxLedState m_state;
    awxLedColor m_color[2];
    int m_x;
    int m_y;

    // helper functions
    bool Create(wxWindow *parent, wxWindowID id, awxLedState state,
        awxLedColor color1, awxLedColor color2);
    void DrawOnBitmap();
    void GetStateColorsPair(int pair, awxLedState *state,
        awxLedColor *color1, awxLedColor *color2)
    {
        *state = awxLED_STATE_FIX;
        *color1 = awxLED_COLOR_BLANK;
        *color2 = awxLED_COLOR_BLANK;
        switch (pair)
        {
        default:
        case 0: break;
        case 1: *color1 = awxLED_COLOR_RED; break;
        case 2: *color1 = awxLED_COLOR_GREEN; break;
        case 3: *color1 = awxLED_COLOR_YELLOW; break;
        case 4: *state = awxLED_STATE_BLINK; *color1 = awxLED_COLOR_GREEN; break;
        case 5: *state = awxLED_STATE_BLINK; *color2 = awxLED_COLOR_GREEN; break;
        case 6: *state = awxLED_STATE_BLINK; *color1 = awxLED_COLOR_RED; *color2 = awxLED_COLOR_GREEN; break;
        case 7: *state = awxLED_STATE_BLINK; *color1 = awxLED_COLOR_GREEN; *color2 = awxLED_COLOR_YELLOW; break;
        }
    }
    void OnErase(wxEraseEvent &WXUNUSED(event))
    {
	   Redraw();
    }
    void OnPaint(wxPaintEvent &WXUNUSED(event))
    {
	   wxPaintDC dc(this);
	   dc.DrawBitmap(*m_bitmap, 0, 0, false);
    }
    void OnSize(wxSizeEvent &event)
    {
	   wxSize size = event.GetSize();
	   m_x = (size.GetX() - m_icons[0]->GetWidth()) >> 1;
	   m_y = (size.GetY() - m_icons[0]->GetHeight()) >> 1;
	   if (m_x < 0) m_x = 0;
	   if (m_y < 0) m_y = 0;
	   DrawOnBitmap();
    }
    void Redraw()
    {
	   wxClientDC dc(this);
	   DrawOnBitmap();
	   dc.DrawBitmap(*m_bitmap, 0, 0, false);
    }

    DECLARE_EVENT_TABLE()
};

class BlinkTimer : public wxTimer
{
private:
    wxVector<awxLed *> &_allLeds;
    int &_blink;
public:
    BlinkTimer(awxLed *led) 
        : wxTimer(),
        _allLeds(led->m_allLeds),
        _blink(led->m_blink)
    {
        wxUnusedVar(led);
    }
    void Notify()
    {
        _blink ^= 1;
        for (wxVector<awxLed *>::iterator it = _allLeds.begin();
            it != _allLeds.end();
            ++it)
        {
            if ((*it)->GetState() == awxLED_STATE_BLINK)
                (*it)->Blink();
        }
    }
};

#endif
