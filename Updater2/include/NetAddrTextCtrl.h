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
        const wxString &value = wxEmptyString);
    /* dtor */
    virtual ~NetAddrTextCtrl();

    wxString GetValue();
    void SetValue(const wxString& value);

private:
    /* helper functions */
    void Init();
    virtual bool Layout();
    void HighLightShift(bool right);
    int GetSpecificDigitValue(const int fieldIdx, const int digitIdx);
    long GetSpecificFieldValue(const int fieldIdx);
    int GetHighLightDigitValue();
    void SetSpecificDigitValue(const int fieldIdx, const int digitIdx, const int newDigitValue);

    /* event handlers */
    void OnKeyDown(wxKeyEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnErase(wxEraseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnFocus(wxFocusEvent &event);

    /* data members */
    const NetAddrType _type;
    wxLongLong _internalValue;
    const int _digitBoxW; // a single digit width
    const int _digitBoxH; // a single digit height
    int _hlField; // hightlighted field
    int _hlDigit; // hightlighted digit
    bool _hasFocused;
    int _bitmapWidth;
    int _bitmapHeight;
    wxBitmap *_displayBitmap;
    const wxFontFamily _family;
    wxFont *_displayFont;
    const int _outsideBorderTop, _outsideBorderBottom, _outsideBorderLeft, _outsideBorderRight;
    const int _insideBorderTop, _insideBorderBottom, _insideBorderLeft, _insideBorderRight;

    DECLARE_EVENT_TABLE()
};

//
// Information about the border parameters
//
// +--------------------------------
// | +-------+-------+-------+------
// | | +---+ | +---+ | +---+ |
// | | |   | | |   | | | . | |
// | | |   | | |   | | | . | | ...
// | | +---+ | +---+ | +---+ |
// | +-------+-------+-------+------
// +--------------------------------
//  ^ ^
//  | +-- inside border left
//  +---- outside border left
//

#endif /* _NET_ADDR_TEXT_CTRL_H_ */
