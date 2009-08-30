#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/socket.h>
#include <wx/wxsqlite3.h>
#include <wx/datetime.h>
#include "SimCubeApp.h"
#include "NetAdapter.h"
#include "PeerPane.h"
#include "HistoryPane.h"
#include "UDPProtocol.h"

#define MSG_KEYWORD_SET_REQUEST     wxT("=")
#define MSG_KEYWORD_GET_REQUEST     wxT("?")
#define MSG_KEYWORD_GET_RESPONSE    wxT("!")
#define MSG_KEYWORD_TRAP            wxT("#")
#define MSG_KEYWORD_INVALID         wxT(" ")

BEGIN_EVENT_TABLE(UDPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, UDPProtocol::OnSocketEvent)
END_EVENT_TABLE()

UDPProtocol::UDPProtocol() :
    _needDeallocateSafeBuf(false)
{
    size_t socketId = 0;
    wxIPV4address local;

    /* init function pointer */
    _normalHandler = new NormalHandler[3];
    _normalHandler[0].handler = &UDPProtocol::set_request_handler;
    _normalHandler[0].keyword = MSG_KEYWORD_SET_REQUEST;
    _normalHandler[1].handler = &UDPProtocol::get_request_handler;
    _normalHandler[1].keyword = MSG_KEYWORD_GET_REQUEST;
    _normalHandler[2].handler = &UDPProtocol::null_handler;
    _normalHandler[2].keyword = MSG_KEYWORD_INVALID;

    /* create socket per app network adapter detect result. */
    // wxVector& operator=(const wxVector& vb)
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++, socketId++)
    {
        local.Hostname(it->GetIp());
        local.Service(40000);
        it->udp = new wxDatagramSocket(local);
        if (it->udp->Ok())
        {
            it->udp->SetEventHandler(*this, socketId);
            it->udp->SetNotify(wxSOCKET_INPUT_FLAG);
            it->udp->Notify(true);
        }
    }
}

UDPProtocol::~UDPProtocol()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++)
    {
        if (it->udp)
        {
            it->udp->Notify(false);
            it->udp->Destroy();
        }
    }

    delete [] _normalHandler;
}

void UDPProtocol::OnSocketEvent(wxSocketEvent& event)
{
    wxIPV4address remote;
    char unsafeBuf[1500];
    char *safeBuf;
    size_t id = event.GetId(), numByte, numByteInSafeBuf;
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxDatagramSocket *udpSocket = netAdapter.at(id).udp;

    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        memset(unsafeBuf, 0, sizeof(unsafeBuf));
        udpSocket->RecvFrom(remote, unsafeBuf, sizeof(unsafeBuf));
        if (udpSocket->Error())
        {
            wxLogError(_("NetAdapter[%d]: Fail to receive something from UDP socket. Error = %d"),
                id, udpSocket->LastError());
        }
        else
        {
            numByte = udpSocket->LastCount();
            wxLogVerbose(_("NetAdapter[%d]: Received %d byte(s) from %s:%d."),
                id, numByte, remote.IPAddress(), remote.Service());

            /* make sure buffer is safe for later process */
            AllocateSafeBuf(&safeBuf, &numByteInSafeBuf, unsafeBuf, numByte);

            /* add buffer content into history */
            HistoryDataModel *data = wxGetApp().m_HistoryData;
            data->AddData(remote.IPAddress(), remote.Service(),
                wxString::FromAscii(safeBuf, numByteInSafeBuf), numByteInSafeBuf);

            /* process message */
            if (IsBufferUnsafe(unsafeBuf, numByte))
                ProcessDownloadModeProtocol(unsafeBuf, numByte, remote, udpSocket);
            else
                ProcessNormalModeProtocol(safeBuf, numByteInSafeBuf, remote, udpSocket);

            /* clean up safe buf */
            DeallocateSafeBuf(safeBuf);
        }
    default:
        break;
    }
}

void UDPProtocol::ProcessDownloadModeProtocol(const char *buf, size_t len,
                                              wxIPV4address &peer,
                                              wxDatagramSocket *local)
{
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    wxSQLite3ResultSet set;
    wxString sqlQuery, name;
    size_t nameLen;
    unsigned char updaterMessage[36] = 
    {
        0x53, 0x51, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    wxASSERT_MSG(db, wxT("Null Database"));
    wxASSERT_MSG(db->IsOpen(), wxT("Database closed"));

    /* load our board name into */
    sqlQuery = wxT("SELECT CurrentValue FROM PropTbl WHERE DisplayedName = 'BoardName'");
    set = db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
    {
        name = set.GetAsString(0);
        nameLen = name.length();
        strcpy((char *)&updaterMessage[16], name.ToAscii());
    }
    else
        nameLen = (size_t)-1;
    set.Finalize();

    /* check if match updater unname search */
    if (!memcmp(updaterMessage, buf, len > 36 ? 36 : len))
    {
        updaterMessage[0] += 0x20;
        updaterMessage[1] += 0x20;
        local->SendTo(peer, updaterMessage, 16 + nameLen + 1);
        return;
    }

    /* check if match updater name search */
    updaterMessage[11] = nameLen + 1;
    if (!memcmp(updaterMessage, buf, len > 36 ? 36 : len))
    {
        updaterMessage[0] += 0x20;
        updaterMessage[1] += 0x20;
        local->SendTo(peer, updaterMessage, 16 + nameLen + 1);
        return;
    }
}

void UDPProtocol::ProcessNormalModeProtocol(const char *buf, size_t len,
                                            wxIPV4address &peer,
                                            wxDatagramSocket *local)
{
    bool handled = false;
    int msg_keyword_checker, handler, rIdx, lIdx;
    wxString token;
    wxStringTokenizer tkz(wxString::FromAscii(buf, len), wxT(";"));
    while (tkz.HasMoreTokens())
    {
        token = tkz.GetNextToken();
        if (token.IsEmpty())
            continue;
        msg_keyword_checker = 0;
        for (handler = 0;
            _normalHandler[handler].keyword != MSG_KEYWORD_INVALID;
            handler++)
        {
            lIdx = token.find(_normalHandler[handler].keyword);
            if (lIdx != wxNOT_FOUND)
            {
                /* found keyword for current handler */
                msg_keyword_checker |= 0x01 << (2 * handler);
                rIdx = token.rfind(_normalHandler[handler].keyword);
                if (lIdx != rIdx)
                {
                    /* found multiple keywords for handler in current token */
                    msg_keyword_checker |= 0x02 << (2 * handler);
                }
            }
        }

        /* skip if no keyword */
        if (msg_keyword_checker == 0)
        {
            wxLogVerbose(_("No keyword in token (%s)"), token);
            continue;
        }

        /* process token if only single keyword */
        if (msg_keyword_checker & (msg_keyword_checker - 1))
        {
            wxLogVerbose(_("Multiple keywords in token (%s)"), token);
            continue;
        }
        else
        {
            /* find handler type */
            for (handler = 0; msg_keyword_checker != 0x01; handler++, msg_keyword_checker >>= 2);
            /* process token by handler type */
            handled = (this->*_normalHandler[handler].handler)(token.ToAscii(), token.length(), peer, local);

            if (!handled)
                wxLogVerbose(_("Token (%s) didn't handle by handler"), token);
        }
    }
}

void UDPProtocol::AllocateSafeBuf(char **safe, size_t *safeLen,
                                  const char *unsafe, size_t unsafeLen)
{
    if (IsBufferUnsafe(unsafe, unsafeLen))
        MakeBufferSafe(safe, safeLen, unsafe, unsafeLen);
    else
    {
        *safe = (char *)unsafe;
        *safeLen = unsafeLen;
    }
}

void UDPProtocol::DeallocateSafeBuf(char *safe)
{
    if (_needDeallocateSafeBuf)
    {
        delete [] safe;
        _needDeallocateSafeBuf = false;
    }
}

bool UDPProtocol::IsBufferUnsafe(const char *buf, size_t len)
{
    for (size_t idx = 0; idx < len; ++idx)
    {
        if (((buf[idx] > 0) && (buf[idx] < 0x20)) || (buf[idx] < 0))
            return true;
    }

    return false;
}

void UDPProtocol::MakeBufferSafe(char **safe, size_t *safeLen,
                                 const char *unsafe, size_t unsafeLen)
{
    char *_safe = new char [unsafeLen * 3 + 9];
    memset(_safe, 0, unsafeLen * 3 + 9);
    sprintf(_safe, "SafeBuf:");
    for (size_t idx = 0; idx < unsafeLen; ++idx)
        sprintf(&_safe[strlen(_safe)], " %02X", (unsafe[idx] & 0xFF));
    *safe = _safe;
    *safeLen = unsafeLen * 3 + 9;
    _needDeallocateSafeBuf = true;
}

bool UDPProtocol::set_request_handler(const char *buf, size_t len,
                                      wxIPV4address &peer,
                                      wxDatagramSocket *WXUNUSED(local))
{
    bool handled = true;
    wxStringTokenizer request(wxString::FromAscii(buf, len), MSG_KEYWORD_SET_REQUEST);
    wxString name, value, sqlQuery, sqlUpdate;
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    wxSQLite3ResultSet set;

    wxASSERT_MSG(db, wxT("Null Database"));
    wxASSERT_MSG(db->IsOpen(), wxT("Database closed"));

    /* property name */
    if (request.HasMoreTokens())
        name = request.GetNextToken().Upper();

    /* property value */
    if (request.HasMoreTokens())
        value = request.GetNextToken();

    /* list all known set request messages no need to be handled */
    if (name.IsSameAs(wxT("CHECK_BOARD_CONFIG"))
        || name.IsSameAs(wxT("CHECK_CONNECTION"))
        || name.IsSameAs(wxT("CONNECT"))
        || name.IsSameAs(wxT("DISCOVER"))
        || name.IsSameAs(wxT("LAMP_A_STATUS"))
        || name.IsSameAs(wxT("LAMP_A_HOURS"))
        || name.IsSameAs(wxT("LAMP_A_LIT_COUNT"))
        || name.IsSameAs(wxT("LAMP_A_TEMP_COND"))
        || name.IsSameAs(wxT("LAMP_B_STATUS"))
        || name.IsSameAs(wxT("LAMP_B_HOURS"))
        || name.IsSameAs(wxT("LAMP_B_LIT_COUNT"))
        || name.IsSameAs(wxT("LAMP_B_TEMP_COND"))
        || name.IsSameAs(wxT("LEDSTATUS"))
        || name.IsSameAs(wxT("SVN_REV"))
        )
    {
        handled = false;
    }
    /* list all set request messages need to be secial handled */
    else if (name.IsSameAs(wxT("DISCONNECT")))
    {
        /* check request board name is matched or not */
        bool match = false;
        sqlQuery << wxT("SELECT CurrentValue FROM PropTbl WHERE DisplayedName = 'BoardName'");
        set = db->ExecuteQuery(sqlQuery);
        if (set.NextRow())
        {
            if (value == set.GetAsString(0))
                match = true;
        }
        else
            handled = false;
        set.Finalize();

        if (match)
        {
            PeerDataModel *data = wxGetApp().m_PeerData;
            if (data->IsContain(peer))
                data->RemoveData(peer);
        }
    }
    else if (name.IsSameAs(wxT("DISCOVER_DEVICE_SELECT")))
    {

    }
    else if (name.IsSameAs(wxT("MONITOR")))
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (value.IsSameAs(wxT("ENABLE"), false))
            data->SetMonitor(peer, true);
        else if (value.IsSameAs(wxT("DISABLE"), false))
            data->SetMonitor(peer, false);
    }
    else if (name.IsSameAs(wxT("RESET_ALL")))
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {

        }
        else
            handled = false;
    }
    /* general set request messages - handled by database update */
    else
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {
            sqlUpdate << wxT("UPDATE PropTbl SET CurrentValue = '");
            if (name.IsSameAs(wxT("BOARD_NAME")))
                sqlUpdate << value;
            else
                sqlUpdate << value.Upper();
            sqlUpdate << wxT("' WHERE ProtocolName = '") << name << wxT("'");
            if (db->ExecuteUpdate(sqlUpdate) != 1)
            {
                wxLogError(_("This may not be an error. Set Request (%s) is not handled by database update."), name);
                handled = false;
            }
        }
        else
            handled = false;
    }

    return handled;
}

bool UDPProtocol::get_request_handler(const char *buf, size_t len,
                                      wxIPV4address &peer,
                                      wxDatagramSocket *local)
{
    bool handled = true;
    wxStringTokenizer request(wxString::FromAscii(buf, len), MSG_KEYWORD_GET_REQUEST);
    wxString name, value, sqlQuery, response;
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    wxSQLite3ResultSet set;

    wxASSERT_MSG(db, wxT("Null Database"));
    wxASSERT_MSG(db->IsOpen(), wxT("Database closed"));

    /* property name */
    if (request.HasMoreTokens())
        name = request.GetNextToken().Upper();

    /* property value - optional */
    if (request.HasMoreTokens())
        value = request.GetNextToken();

    /* list all known get request messges no need to be handled */
    if (name.IsSameAs(wxT("DISCONNECT"))
        || name.IsSameAs(wxT("DLP_TEST_PATTERN"))
        || name.IsSameAs(wxT("DUMP_STATE_STACK"))
        || name.IsSameAs(wxT("KEY"))
        || name.IsSameAs(wxT("RESET_ALL"))
        || name.IsSameAs(wxT("USER_MODE_LOAD"))
        || name.IsSameAs(wxT("USER_MODE_SAVE"))
        )
    {
        handled = false;
    }
    /* list all get request messages need to be secial handled */
    else if (name.IsSameAs(wxT("CHECK_BOARD_CONFIG")))
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {
            response << name << MSG_KEYWORD_GET_RESPONSE << wxT("SUCCESS");
            local->SendTo(peer, response.ToAscii(), response.length() + 1);
            if (local->Error())
            {
                wxLogError(_("Fail to send check board config response back to peer. (error = %d)"),
                    local->LastError());
                handled = false;
            }
        }
        else
            handled = false;
    }
    else if (name.IsSameAs(wxT("CHECK_CONNECTION")))
    {
        response << name << MSG_KEYWORD_GET_RESPONSE;
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
            response << wxT("CONNECTED");
        else
            response << wxT("NOT_CONNECTED");
        local->SendTo(peer, response.ToAscii(), response.length() + 1);
        if (local->Error())
        {
            wxLogError(_("Fail to send check connection response back to peer. (error = %d)"),
                local->LastError());
            handled = false;
        }
    }
    else if (name.IsSameAs(wxT("CONNECT")))
    {
        if (!value.empty())
        {
            /* check request board name is matched or not */
            bool match = false;
            sqlQuery << wxT("SELECT CurrentValue FROM PropTbl WHERE DisplayedName = 'BoardName'");
            set = db->ExecuteQuery(sqlQuery);
            if (set.NextRow())
            {
                if (value == set.GetAsString(0))
                    match = true;
            }
            else
                handled = false;
            set.Finalize();

            response << name << MSG_KEYWORD_GET_RESPONSE;
            if (!match)
                response << wxT("REJECT");
            else
            {
                response << wxT("ACCEPT");

                PeerDataModel *data = wxGetApp().m_PeerData;
                /* if this is new data, add it, and update ui. */
                if (!data->IsContain(peer))
                {
                    PeerData item(peer, wxDateTime::Now());
                    data->AddData(item);
                }
            }
            local->SendTo(peer, response.ToAscii(), response.length() + 1);
            if (local->Error())
            {
                wxLogError(_("Fail to send connect response back to peer. (error = %d)"),
                    local->LastError());
                handled = false;
            }
        }
        else
        {
            wxLogError(_("Empty BoardName in CONNECT get request message"));
            handled = false;
        }
    }
    else if (name.IsSameAs(wxT("DISCOVER_DEVICE_SELECT")))
    {

    }
    else if (name.IsSameAs(wxT("DISCOVER")))
    {
        sqlQuery << wxT("SELECT CurrentValue FROM PropTbl WHERE DisplayedName = 'BoardName'");
        set = db->ExecuteQuery(sqlQuery);
        if (set.NextRow())
        {
            response << name << MSG_KEYWORD_GET_RESPONSE << set.GetAsString(0);
            local->SendTo(peer, response.ToAscii(), response.length() + 1);
            if (local->Error())
            {
                wxLogError(_("Fail to send discover response back to peer. (error = %d)"),
                    local->LastError());
                handled = false;
            }
        }
        set.Finalize();
    }
    else if (name.IsSameAs(wxT("MONITOR")))
    {
        response << name << MSG_KEYWORD_GET_RESPONSE;
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {
            if (data->GetMonitor(peer))
                response << wxT("ENABLE");
            else
                response << wxT("DISABLE");
        }
        else
            response << wxT("DISABLE");
        local->SendTo(peer, response.ToAscii(), response.length() + 1);
        if (local->Error())
        {
            wxLogError(_("Fail to send monitor response back to peer. (error = %d)"),
                local->LastError());
            handled = false;
        }
    }
    else if (name.IsSameAs(wxT("SVN_REV")))
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {

        }
        else
            handled = false;
    }
    /* general get request messages - handled by database query */
    else
    {
        PeerDataModel *data = wxGetApp().m_PeerData;
        if (data->IsContain(peer))
        {
            sqlQuery << wxT("SELECT CurrentValue FROM PropTbl WHERE ProtocolName = '")
                << name << wxT("'");
            set = db->ExecuteQuery(sqlQuery);
            if (set.NextRow())
            {
                response << name << MSG_KEYWORD_GET_RESPONSE << set.GetAsString(0);
                local->SendTo(peer, response.ToAscii(), response.length() + 1);
                if (local->Error())
                {
                    wxLogError(_("Fail to send response (%s) back to peer. (error = %d)"),
                        response, local->LastError());
                    handled = false;
                }
            }
            set.Finalize();
        }
        else
            handled = false;
    }

    return handled;
}

bool UDPProtocol::null_handler(const char *WXUNUSED(buf),
                               size_t WXUNUSED(len),
                               wxIPV4address &WXUNUSED(peer),
                               wxDatagramSocket *WXUNUSED(local))
{
    return false;
}

