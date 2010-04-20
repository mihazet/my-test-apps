#ifndef _NET_ADDR_TEXT_CTRL_H_
#define _NET_ADDR_TEXT_CTRL_H_

#include <wx/wx.h>

class NetAddrTextCtrl : public wxControl
{
public:
    typedef enum
    {
        NETADDR_TYPE_IP,
        NETADDR_TYPE_MAC,
        NETADDR_TYPE_HALF_MAC,
        NETADDR_TYPE_INVALID
    } NetAddrType;

    /* ctor */
    NetAddrTextCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
        const NetAddrType type = NETADDR_TYPE_HALF_MAC,
        const wxString &value = wxEmptyString,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize);
    /* dtor */
    virtual ~NetAddrTextCtrl();

    wxString GetValue();
    void SetValue(const wxString& value);

private:
    /* helper functions */
    void Init();
    virtual bool Layout();

    /* event handlers */
    void OnKeyDown(wxKeyEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnErase(wxEraseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnFocus(wxFocusEvent &event);

    /* data members */
    NetAddrType _type;
    wxLongLong _internalValue;
    int _digitBoxW; // a single digit width
    int _digitBoxH; // a single digit height

    DECLARE_EVENT_TABLE()
};

#endif /* _NET_ADDR_TEXT_CTRL_H_ */
