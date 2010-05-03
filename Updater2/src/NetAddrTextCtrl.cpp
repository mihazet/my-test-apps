#include <wx/wx.h>
#include <wx/event.h>
#include <wx/tokenzr.h>
#include <cmath>
#include "NetAddrTextCtrl.h"


const int gDigitInField[NetAddrTextCtrl::NETADDR_TYPE_INVALID]      = {  3,  2,  2 };
const int gFieldMax[NetAddrTextCtrl::NETADDR_TYPE_INVALID]          = {  4,  6,  3 };
const int gFieldBase[NetAddrTextCtrl::NETADDR_TYPE_INVALID]         = { 10, 16, 16 };
const wxString gFieldDelimit[NetAddrTextCtrl::NETADDR_TYPE_INVALID] = { wxT("."), wxT(":"), wxT(":") };


BEGIN_EVENT_TABLE(NetAddrTextCtrl, wxControl)
    EVT_ERASE_BACKGROUND(NetAddrTextCtrl::OnErase)
    EVT_PAINT(NetAddrTextCtrl::OnPaint)
    EVT_MOUSE_EVENTS(NetAddrTextCtrl::OnMouse)
    EVT_KEY_DOWN(NetAddrTextCtrl::OnKeyDown)
    EVT_SET_FOCUS(NetAddrTextCtrl::OnFocus)
    EVT_KILL_FOCUS(NetAddrTextCtrl::OnFocus)
END_EVENT_TABLE()

NetAddrTextCtrl::NetAddrTextCtrl(wxWindow *parent, wxWindowID id,
                                 const NetAddrType type, 
                                 const wxString &value)
    : wxControl(),
    _type(type),
    _digitBoxW(10),
    _digitBoxH(16),
    _outsideBorderTop(1),
    _outsideBorderBottom(1),
    _outsideBorderLeft(1),
    _outsideBorderRight(1),
    _insideBorderTop(2),
    _insideBorderBottom(2),
    _insideBorderLeft(1),
    _insideBorderRight(1),
    _family(wxFONTFAMILY_MODERN)
{
    Init();
    SetValue(value);
    Layout();
    wxControl::Create(parent, id, wxDefaultPosition, wxSize(_bitmapWidth, _bitmapHeight), wxDOUBLE_BORDER | wxWANTS_CHARS);
}

NetAddrTextCtrl::~NetAddrTextCtrl()
{
    wxDELETE(_displayBitmap);
    wxDELETE(_displayFont);
}

void NetAddrTextCtrl::Init()
{
    _displayBitmap = NULL;
    _displayFont = NULL;
    _hlField = 0;
    _hlDigit = 0;
    _hasFocused = false;
}

wxString NetAddrTextCtrl::GetValue()
{
    wxString valueString;
    int idx;

    for (idx = 0; idx < gFieldMax[_type]; idx++)
    {
        valueString << wxString::Format((gFieldBase[_type] == 10) ? wxT("%ld") : wxT("%02lX"), GetSpecificFieldValue(idx));
        if (idx != (gFieldMax[_type] - 1))
            valueString << gFieldDelimit[_type];
    }

    return valueString;
}

void NetAddrTextCtrl::SetValue(const wxString &newValue)
{
    wxStringTokenizer tokenizer;
    wxString token;
    int count = 0;
    wxLongLong longlongValue, longlongTemp = 0;
    long longValue;
    bool redraw = true;

    if (newValue.empty())
    {
        longlongTemp = 0;
    }
    else if (_type == NETADDR_TYPE_IP)
    {
        tokenizer.SetString(newValue, wxT("."));
        while (tokenizer.HasMoreTokens())
        {
            token = tokenizer.GetNextToken();
            switch (count)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                longValue = 0;
                token.ToLong(&longValue);
                longlongTemp += ((longValue & 0xFF) << (24 - 8 * count));
                break;
            default:
                redraw = false;
                break;
            }
            count++;
        }

        wxASSERT_MSG((count == 4), wxT("Set wrong format value to NetAddrTextCtrl with IP type."));
    }
    else if ((_type == NETADDR_TYPE_MAC) || (_type == NETADDR_TYPE_HALF_MAC))
    {
        tokenizer.SetString(newValue, wxT(":-"));
        while (tokenizer.HasMoreTokens())
        {
            token = tokenizer.GetNextToken();
            switch (count)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                longValue = 0;
                token.ToLong(&longValue, 16);
                if (_type == NETADDR_TYPE_MAC)
                {
                    longlongValue = longValue;
                    longlongTemp += ((longlongValue & 0xFF) << (40 - 8 * count));
                }
                else if ((_type == NETADDR_TYPE_HALF_MAC) && (count <= 2))
                    longlongTemp += ((longValue & 0xFF) << (16 - 8 * count));
                break;
            default:
                break;
            }
            count++;
        }

        if (_type == NETADDR_TYPE_MAC)
        {
            if (count != 6) redraw = false;
            wxASSERT_MSG((count == 6), wxT("Set wrong format value to NetAddrTextCtrl with MAC type."));
        }
        else if (_type == NETADDR_TYPE_HALF_MAC)
        {
            if (count != 3) redraw = false;
            wxASSERT_MSG((count == 3), wxT("Set wrong format value to NetAddrTextCtrl with half MAC type."));
        }
    }
    else
    {
        wxASSERT_MSG(false, wxT("NetAddrTextCtrl with invalid type!"));
        redraw = false;
    }

    if (redraw)
    {
        _internalValue = longlongTemp;
        Layout();
        Refresh();
    }
}

bool NetAddrTextCtrl::IsBiggerThan(const NetAddrTextCtrl& addr)
{
    bool bigger = false;
    long fieldSelf, fieldOther;
    int fieldIdx;

    if (_type == addr._type)
    {
        for (fieldIdx = 0; fieldIdx < gFieldMax[_type]; fieldIdx++)
        {
            fieldSelf = GetSpecificFieldValue(fieldIdx);
            fieldOther = addr.GetSpecificFieldValue(fieldIdx);
            if (fieldSelf > fieldOther)
            {
                bigger = true;
                break;
            }
            else if (fieldSelf < fieldOther)
                break;
        }
    }

    return bigger;
}

bool NetAddrTextCtrl::IsEqual(const NetAddrTextCtrl& addr)
{
    bool equal = false;
    long fieldSelf, fieldOther;
    int fieldIdx;

    if (_type == addr._type)
    {
        equal = true;

        for (fieldIdx = 0; fieldIdx < gFieldMax[_type]; fieldIdx++)
        {
            fieldSelf = GetSpecificFieldValue(fieldIdx);
            fieldOther = addr.GetSpecificFieldValue(fieldIdx);
            if (fieldSelf != fieldOther)
            {
                equal = false;
                break;
            }
        }
    }

    return equal;
}

//
// Helper functions
//
bool NetAddrTextCtrl::Layout()
{
    wxMemoryDC memDC;

    if (!_displayFont)
    {
        int fontSize = 4;
        wxCoord strW, strH;
        wxString exampleText = wxT("0");

        /* keep making the font bigger until it's too big, then subtract one */
        do
        {
            fontSize++;
            memDC.SetFont(wxFont(fontSize, _family, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            memDC.GetTextExtent(exampleText, &strW, &strH);
        } while ((strW <= _digitBoxW) && (strH <= _digitBoxH));

        _displayFont = new wxFont(--fontSize, _family, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    }
    memDC.SetFont(*_displayFont);

    if (!_displayBitmap)
    {
        _bitmapWidth = _outsideBorderLeft 
            + gDigitInField[_type] * gFieldMax[_type] * (_insideBorderLeft + _digitBoxW + _insideBorderRight)
            + (gFieldMax[_type] - 1) * (_insideBorderLeft + _digitBoxW + _insideBorderRight)
            + _outsideBorderRight;
        _bitmapHeight = _outsideBorderTop + _insideBorderTop + _digitBoxH + _insideBorderBottom + _outsideBorderBottom;

        _displayBitmap = new wxBitmap(_bitmapWidth, _bitmapHeight);
    }
    memDC.SelectObject(*_displayBitmap);

    /* start to draw on bitmap */
    // draw a focus rectangle outline
    memDC.SetBrush(*wxLIGHT_GREY_BRUSH);
    memDC.SetPen(_hasFocused ? *wxBLACK : *wxTRANSPARENT_PEN);
    memDC.DrawRectangle(0, 0, _bitmapWidth - 4, _bitmapHeight - 4); // 4 is a magic number, I don't know why? We can't see the whole bitmap, both right/bottom are clipped.

    // draw token and delimit characters digit by digit
    int digitIdx, fieldIdx;
    for (fieldIdx = 0; fieldIdx < gFieldMax[_type]; fieldIdx++)
    {
        // draw token
        for (digitIdx = 0; digitIdx < gDigitInField[_type]; digitIdx++)
        {
            if (_hasFocused && (fieldIdx == _hlField) && (digitIdx == _hlDigit))
            {
                memDC.SetTextForeground(*wxBLACK);
                memDC.SetTextBackground(*wxWHITE);
                memDC.SetBackgroundMode(wxSOLID);
            }
            else
            {
                memDC.SetTextForeground(*wxBLUE);
                memDC.SetBackgroundMode(wxTRANSPARENT);
            }

            memDC.DrawText(wxString::Format((gFieldBase[_type] == 10) ? wxT("%d") : wxT("%X"), GetSpecificDigitValue(fieldIdx, digitIdx)),
                _outsideBorderLeft + (fieldIdx * (gDigitInField[_type] + 1) + digitIdx) * (_insideBorderLeft + _digitBoxW + _insideBorderRight),
                _outsideBorderTop);
        }

        // draw delimit
        if (fieldIdx != (gFieldMax[_type] - 1))
        {
            memDC.SetTextForeground(*wxBLACK);
            memDC.SetBackgroundMode(wxTRANSPARENT);
            memDC.DrawText(gFieldDelimit[_type],
                _outsideBorderLeft + (fieldIdx * (gDigitInField[_type] + 1) + digitIdx) * (_insideBorderLeft + _digitBoxW + _insideBorderRight),
                _outsideBorderTop);
        }
    }

    memDC.SelectObject(wxNullBitmap);
    return true;
}

void NetAddrTextCtrl::HighLightShift(bool right)
{
    
    if (right)
    {
        if (++_hlDigit >= gDigitInField[_type])
        {
            _hlDigit = 0;
            _hlField++;
        }
        if (_hlField >= gFieldMax[_type])
            _hlField = 0;
    }
    else
    {
        if (--_hlDigit < 0)
        {
            _hlDigit = gDigitInField[_type] - 1;
            _hlField--;
        }
        if (_hlField < 0)
            _hlField = gFieldMax[_type] - 1;
    }
}

int NetAddrTextCtrl::GetSpecificDigitValue(const int fieldIdx, const int digitIdx) const
{
    wxASSERT_MSG((fieldIdx < gFieldMax[_type]), "filedIdx out of range");
    wxASSERT_MSG((digitIdx < gDigitInField[_type]), "digitIdx out of range");

    long fieldValue = GetSpecificFieldValue(fieldIdx);
    int denominator = (int) pow((double)gFieldBase[_type], (int)(gDigitInField[_type] - 1 - digitIdx));
    int digitValue = (fieldValue / denominator) % gFieldBase[_type];

    return digitValue;
}

long NetAddrTextCtrl::GetSpecificFieldValue(const int fieldIdx) const
{
    wxASSERT_MSG((fieldIdx < gFieldMax[_type]), "filedIdx out of range");

    wxLongLong llFieldValue = (_internalValue >> (8 * (gFieldMax[_type] - 1 - fieldIdx))) & 0xFF;
    // wxLongLong::ToLong() will trigger a runtime assert in debug mode if the conversion will lose data,
    // so, it is better to make sure the wxLongLong value above is truncated by ourself first.
    long fieldValue = llFieldValue.ToLong();
    return fieldValue;
}

int NetAddrTextCtrl::GetHighLightDigitValue() const
{
    return GetSpecificDigitValue(_hlField, _hlDigit);
}

void NetAddrTextCtrl::SetSpecificDigitValue(const int fieldIdx, const int digitIdx, const int newDigitValue)
{
    wxASSERT_MSG((fieldIdx < gFieldMax[_type]), "filedIdx out of range");
    wxASSERT_MSG((digitIdx < gDigitInField[_type]), "digitIdx out of range");
    wxASSERT_MSG((newDigitValue < gFieldBase[_type]), "newDigitValue out of range");

    long oldFieldValue = GetSpecificFieldValue(fieldIdx);
    int oldDigitValue = GetSpecificDigitValue(fieldIdx, digitIdx);
    int newFieldValue = oldFieldValue 
        + (newDigitValue - oldDigitValue) * pow((double)gFieldBase[_type], (int)(gDigitInField[_type] - 1 - digitIdx));

    /* update _internalValue */
    int shift = 8 * (gFieldMax[_type] - 1 - fieldIdx);
    wxLongLong oldVal = oldFieldValue;
    wxLongLong newVal = newFieldValue;  
    _internalValue = _internalValue + ((newVal - oldVal) << shift);
}

//
// Event handlers
//
void NetAddrTextCtrl::OnKeyDown(wxKeyEvent &event)
{
    int keyCode = event.GetKeyCode();
    bool needRepaint = true;
    int digitValue = 0;
    bool isA2F = false, isNumPad = true;

    wxLogDebug(wxT("Key %d down"), keyCode);

    switch (keyCode)
    {
    case WXK_TAB:
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        /* simulate a navigation tab key */
        {
            wxNavigationKeyEvent navEvt;
            navEvt.SetWindowChange((keyCode == WXK_TAB) ? event.ControlDown() : false);
            navEvt.SetDirection((keyCode == WXK_TAB) ? !event.ShiftDown() : true);
            navEvt.SetEventObject(GetParent());
            navEvt.SetCurrentFocus(GetParent());
            GetParent()->GetEventHandler()->ProcessEvent(navEvt);
            needRepaint = false;
        }
        break;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        if (gFieldBase[_type] == 10)
        {
            needRepaint = false;
            break;
        }
        digitValue = keyCode - 'A' + 10;
        isA2F = true;
        // intent to fall through...
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (event.ShiftDown())
        {
            needRepaint = false;
            break;
        }
        if (!isA2F)
            digitValue = keyCode - '0';
        isNumPad = false;
        // intent to fall through...
    case WXK_NUMPAD0:
    case WXK_NUMPAD1:
    case WXK_NUMPAD2:
    case WXK_NUMPAD3:
    case WXK_NUMPAD4:
    case WXK_NUMPAD5:
    case WXK_NUMPAD6:
    case WXK_NUMPAD7:
    case WXK_NUMPAD8:
    case WXK_NUMPAD9:
        if (!isA2F && isNumPad)
            digitValue = keyCode - WXK_NUMPAD0;

        SetSpecificDigitValue(_hlField, _hlDigit, digitValue);
        HighLightShift(true);
        break;

    case WXK_UP:
    case WXK_DOWN:    
        digitValue = GetSpecificDigitValue(_hlField, _hlDigit);
        if (keyCode == WXK_UP)
        {
            digitValue++;
            if (digitValue >= gFieldBase[_type])
                digitValue = 0;
            SetSpecificDigitValue(_hlField, _hlDigit, digitValue);
        }
        else
        {
            digitValue--;
            if (digitValue < 0)
                digitValue = gFieldBase[_type] - 1;
            SetSpecificDigitValue(_hlField, _hlDigit, digitValue);
        }
        break;

    case WXK_RIGHT:
        HighLightShift(true);
        break;

    case WXK_LEFT:
        HighLightShift(false);
        break;

    case WXK_HOME:
    case WXK_NUMPAD_HOME:
        _hlField = 0;
        _hlDigit = 0;
        break;

    case WXK_END:
    case WXK_NUMPAD_END:
        _hlField = gFieldMax[_type] - 1;
        _hlDigit = gDigitInField[_type] - 1;
        break;

    default: 
        /* for key that we are not interesed, just skip it */
        event.Skip();
        needRepaint = false;
        wxBell();
        break;
    }

    if (needRepaint)
    {
        Layout();
        Refresh(false);
    }
}

void NetAddrTextCtrl::OnMouse(wxMouseEvent &event)
{
    int posX, fieldIdx, digitIdx, distance;
    int shortestDistance = 0x7FFFFFFF;
    bool skip = true;
    int evtType = event.GetEventType();

    if (event.LeftDown() || event.LeftDClick())
    {
        posX = event.GetX();

        // find the nearest digit (skip delimit), and change highlight to it
        // The pos -> digit need to sync with Layout logic
        for (fieldIdx = 0; fieldIdx < gFieldMax[_type]; fieldIdx++)
        {
            for (digitIdx = 0; digitIdx < gDigitInField[_type]; digitIdx++)
            {
                distance = abs(posX - (_outsideBorderLeft + (fieldIdx * (gDigitInField[_type] + 1) + digitIdx) * (_insideBorderLeft + _digitBoxW + _insideBorderRight) + _digitBoxW / 2));
                if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    _hlField = fieldIdx;
                    _hlDigit = digitIdx;
                }
            }
        }

        if (_hasFocused)
        {
            Layout();
            Refresh(false);
            skip = false;
        }
    }
    else if (evtType == wxEVT_MOUSEWHEEL)
    {
        if ((event.GetWheelAxis() == 0)
            && (event.GetWheelRotation() != 0)
            && (event.GetModifiers() == 0))
        {
            // Translate mousewheel actions into key up/down. This is
            // the simplest way of getting native behaviour: scrolling the
            // wheel moves selection up/down by one item.
            wxKeyEvent keyEvt(wxEVT_KEY_DOWN);
            keyEvt.m_keyCode = event.GetWheelRotation() > 0
                               ? WXK_UP
                               : WXK_DOWN;
            GetEventHandler()->ProcessEvent(keyEvt);
            skip = false;
        }
    }

    event.Skip(skip);
}

void NetAddrTextCtrl::OnErase(wxEraseEvent &WXUNUSED(event))
{
    // intent to do nothing
}
    
void NetAddrTextCtrl::OnPaint(wxPaintEvent &event)
{
    wxLogDebug(wxT("OnPaint: %d"), event.GetId());
    wxPaintDC dc(this);
    dc.DrawBitmap(*_displayBitmap, 0, 0);
}

void NetAddrTextCtrl::OnFocus(wxFocusEvent &event)
{
    wxLogDebug(wxT("%s Focus: %d"), (event.GetEventType() == wxEVT_KILL_FOCUS) ? wxT("Kill") : wxT("Get"), event.GetId());
    _hasFocused = (event.GetEventType() == wxEVT_SET_FOCUS);
    Layout();
    Refresh(false);
}

