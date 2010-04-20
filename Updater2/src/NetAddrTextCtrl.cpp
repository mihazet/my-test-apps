#include <wx/wx.h>
#include <wx/event.h>
#include <wx/tokenzr.h>
#include "NetAddrTextCtrl.h"


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
                                 const wxString &value,
                                 const wxPoint &pos, const wxSize &size)
    : wxControl(parent, id, pos, size, wxDOUBLE_BORDER | wxWANTS_CHARS),
    _type(type)
{
    Init();
    SetValue(value);
    Layout();
    SetInitialSize(wxSize(120, 20));
}

NetAddrTextCtrl::~NetAddrTextCtrl()
{

}

void NetAddrTextCtrl::Init()
{
    _digitBoxW = 10;
    _digitBoxH = 16;

}

wxString NetAddrTextCtrl::GetValue()
{
    if (_type == NETADDR_TYPE_IP)
    {
        return wxString::Format(wxT("%d.%d.%d.%d"), 
            (_internalValue >> 24) & 0xFF,
            (_internalValue >> 16) & 0xFF,
            (_internalValue >> 8) & 0xFF,
            _internalValue & 0xFF);
    }
    else if (_type == NETADDR_TYPE_MAC)
    {
        return wxString::Format(wxT("%02X:%02X:%02X:%02X:%02X:%02X"),
            (_internalValue >> 40) & 0xFF,
            (_internalValue >> 32) & 0xFF,
            (_internalValue >> 24) & 0xFF,
            (_internalValue >> 16) & 0xFF,
            (_internalValue >> 8) & 0xFF,
            _internalValue & 0xFF);
    }
    else if (_type == NETADDR_TYPE_HALF_MAC)
    {
        return wxString::Format(wxT("%02X:%02X:%02X"),
            (_internalValue >> 16) & 0xFF,
            (_internalValue >> 8) & 0xFF,
            _internalValue & 0xFF);
    }

    return wxEmptyString;
}

void NetAddrTextCtrl::SetValue(const wxString &newValue)
{
    wxStringTokenizer tokenizer;
    wxString token;
    int count = 0;
    wxLongLong longlongValue, longlongTemp = 0;
    long longValue;

    if (newValue.empty())
    {
        _internalValue = 0;
        return;
    }

    if (_type == NETADDR_TYPE_IP)
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
            wxASSERT_MSG((count == 6), wxT("Set wrong format value to NetAddrTextCtrl with MAC type."));
        }
        else if (_type == NETADDR_TYPE_HALF_MAC)
        {
            wxASSERT_MSG((count == 3), wxT("Set wrong format value to NetAddrTextCtrl with half MAC type."));
        }
    }
    else
    {
        wxASSERT_MSG(false, wxT("NetAddrTextCtrl with invalid type!"));
        return;
    }

    _internalValue = longlongTemp;
}

//
// Helper functions
//
bool NetAddrTextCtrl::Layout()
{
    return true;
}

//
// Event handlers
//
void NetAddrTextCtrl::OnKeyDown(wxKeyEvent &event)
{
    int keyCode = event.GetKeyCode();

    wxLogMessage(wxT("Key %d down"), keyCode);

    switch (keyCode)
    {
    case WXK_TAB:
        /* simulate a navigation tab key */
        {
            wxNavigationKeyEvent navEvt;
            navEvt.SetWindowChange(event.ControlDown());
            navEvt.SetDirection(!event.ShiftDown());
            navEvt.SetEventObject(GetParent());
            navEvt.SetCurrentFocus(GetParent());
            GetParent()->GetEventHandler()->ProcessEvent(navEvt);
        }
        break;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        if (_type == NETADDR_TYPE_IP)
            break;
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
        break;

    default: 
        /* for key that we are not interesed, just skip it */
        event.Skip();
        break;
    }
}

void NetAddrTextCtrl::OnMouse(wxMouseEvent &event)
{
    //wxLogMessage(wxT("Mouse %d.%d"), event.GetX(), event.GetY());
}

void NetAddrTextCtrl::OnErase(wxEraseEvent &WXUNUSED(event))
{
    // intent to do nothing
}
    
void NetAddrTextCtrl::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    bool focus = (FindFocus() == this);
    dc.SetFont(wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(wxT("192.168.001.012"), 0, 0);
    wxLogMessage(wxT("OnPaint"));
}

void NetAddrTextCtrl::OnFocus(wxFocusEvent &event)
{
    if (event.GetEventType() == wxEVT_KILL_FOCUS)
    {
        wxLogMessage(wxT("Kill Focus"));
    }
    else
    {
        wxLogMessage(wxT("Get Focus"));
    }
}

