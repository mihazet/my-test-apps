#include <wx/wx.h>
#include <wx/socket.h>
#include "UDPProtocol.h"

BEGIN_EVENT_TABLE(UDPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, UDPProtocol::OnSocketEvent)
END_EVENT_TABLE()

UDPProtocol::UDPProtocol()
{

}

UDPProtocol::~UDPProtocol()
{

}

void UDPProtocol::OnSocketEvent(wxSocketEvent& WXUNUSED(event))
{
}

