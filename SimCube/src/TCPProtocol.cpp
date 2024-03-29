#include <wx/wx.h>
#include <wx/wfstream.h>
#include "SimCubeApp.h"
#include "NetAdapter.h"
#include "TCPProtocol.h"

/////////////////////////////////////////////////////////////////////////
// Definition for SOI Boot protocol
/////////////////////////////////////////////////////////////////////////
#define SOI_BOOT_CTRL_REQUEST       'S'
#define SOI_BOOT_CTRL_RESPONSE      's'
#define SOI_BOOT_CTRL_CONNECT       'C'
#define SOI_BOOT_CTRL_ACK_CONNECT   'c'
#define SOI_BOOT_LOAD_REQUEST       'T'
#define SOI_BOOT_LOAD_RESPONSE      't'
#define SOI_BOOT_LOAD_APPTYPE       'B'

#define SOI_BOOT_MAX_RX_SIZE        (1024 * 1024)

enum
{
    MODE_DOWNLOAD_FW_TO_FLASH = 1,
    MODE_RUN_FW_FROM_FLASH = 2,
    MODE_DOWNLOAD_FW_TO_RAM = 3,
    MODE_RUN_FW_FROM_RAM = 4,
    MODE_DOWNLOAD_BOOTLOADER_TO_FLASH = 5,
    MODE_RESET_RESERVED_FLASH = 6,
    MODE_UPDATE_MAC_ADDRESS = 0xF,
};

enum
{
    ERR_BL_SYNTAX = 2,
    ERR_BL_RAM_CHECKSUM,
    ERR_BL_BREC_CHECKSUM,
    ERR_BL_LOAD_MODE,
    ERR_BL_VER,
    ERR_BL_MALLOC,
    ERR_BL_ERASE_FLASH,
    ERR_BL_READ_FLASH,
    ERR_BL_WRITE_FLASH,
    ERR_BL_FW_SIZE,
    ERR_BL_BT_SIZE,
};

typedef struct st_ctrl_header
{
    unsigned char byProtocolId;
    unsigned char byControlId;
    unsigned char byAppType;
    unsigned char byAppId;
    unsigned int u32SeqNo;
    unsigned int u32Payload;
    unsigned int u32Reserved;
} tst_CTRL_HEADER;

typedef struct st_ctrl_packet
{
    tst_CTRL_HEADER *stp_ctrl_header;
    unsigned char *byp_ctrl_payload;
} tst_CTRL_PACKET;

typedef struct st_load_header
{
    unsigned char byProtocolId;
    unsigned char byAppType;
    unsigned char byAppId;
    unsigned char byModeId;
    unsigned short u16Error;
    unsigned short u16SeqNo;
    unsigned int u32Payload;
    unsigned int u32Chksum;
} tst_LOAD_HEADER;

typedef struct st_load_packet
{
    tst_LOAD_HEADER *stp_load_header;
    unsigned char *byp_load_payload;
} tst_LOAD_PACKET;

/////////////////////////////////////////////////////////////////////////
// End of SOI Boot protocol
/////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(TCPProtocol, wxEvtHandler)
    EVT_SOCKET(wxID_ANY, TCPProtocol::OnSocketEvent)
END_EVENT_TABLE()

TCPProtocol::TCPProtocol()
    : numClients(0)
{
    size_t socketId = 0;
    wxIPV4address local;

    /* create socket per app network adapter detect result. */
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++, socketId++)
    {
        local.Hostname(it->GetIp());
        local.Service(40000);
        it->tcp = new wxSocketServer(local, wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
        it->tcp->SetEventHandler(*this, socketId);
        it->tcp->SetNotify(wxSOCKET_CONNECTION_FLAG);
        it->tcp->Notify(true);
        if (!it->tcp->Ok())
        {
            wxLogError(_("Fail to bind %s:%d to tcp socket"), local.IPAddress(), local.Service());
        }
    }

    pst_buffer_load = new tst_BUFFER_LOAD;
    pst_buffer_load->byp_load_buffer = new unsigned char[SOI_MAX_DOWNLOAD_SIZE];
}

TCPProtocol::~TCPProtocol()
{
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    for (wxVector<NetAdapter>::iterator it = netAdapter.begin();
        it != netAdapter.end();
        it++)
    {
        if (it->tcp)
        {
            it->tcp->Notify(false);
            it->tcp->Destroy();
        }
    }

    wxDELETEA(pst_buffer_load->byp_load_buffer);
    wxDELETE(pst_buffer_load);
}

//
// This socket event handler will process all event generated by
// the server (listening) sockets (per net adapter) and new create
// (after accept) socket.
// If the event.GetId() < netAdapter.size() => server sockets
// else => new create sockets
//
void TCPProtocol::OnSocketEvent(wxSocketEvent &event)
{
    wxIPV4address remote;
    size_t id = event.GetId();
    wxVector<NetAdapter> &netAdapter = wxGetApp().m_Adapters;
    wxSocketNotify notify = event.GetSocketEvent();

    if (id < netAdapter.size())
    {
        wxSocketServer *tcpSocket = netAdapter.at(id).tcp;

        switch (notify)
        {
        default:
            wxLogError(_("Unexpect event (%d) received in server listening socket!"), notify);
            break;
        case wxSOCKET_CONNECTION:
            wxSocketBase *sock = tcpSocket->Accept(false);
            if (sock)
            {
                if (sock->GetPeer(remote))
                {
                    wxLogMessage(_("Accept new client connection from %s:%d on tcp socket 0x%p, new created socket = 0x%p"),
                        remote.IPAddress(), remote.Service(), tcpSocket, sock);
                }
                else
                    wxLogMessage(_("Accept new connection from unknown client on tcp socket 0x%p, new created socket = 0x%p"),
                        tcpSocket, sock);
            }
            else
            {
                wxLogError(_("Fail to accept new connection on tcp socket 0x%p!"), tcpSocket);
                return;
            }
            sock->SetEventHandler(*this, netAdapter.size());
            sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
            sock->Notify(true);
            numClients++;
            break;
        }
    }
    else
    {
        wxSocketBase *tcpSocket = event.GetSocket();
        unsigned char *rxBuf = new unsigned char[SOI_BOOT_MAX_RX_SIZE + sizeof(tst_LOAD_HEADER)];
        unsigned char *txBuf = rxBuf + SOI_BOOT_MAX_RX_SIZE;
        tst_LOAD_HEADER *stp_load_header;
        long remainedPayloadSize = 0, totalPayloadSize = 0, singleReadCount = 0;

        switch (notify)
        {
        case wxSOCKET_INPUT:
            tcpSocket->Read(rxBuf, sizeof(tst_LOAD_HEADER));
            if (tcpSocket->Error())
            {
                wxLogError(_("Fail to read load header via tcp socket 0x%p, error = %d."), tcpSocket, tcpSocket->LastError());
            }
            else
            {
                stp_load_header = (tst_LOAD_HEADER *)rxBuf;
                remainedPayloadSize = totalPayloadSize = wxUINT32_SWAP_ON_LE(stp_load_header->u32Payload);
                wxLogVerbose(_("Success to read load header, optional payload size = %lu."), totalPayloadSize);
                while (remainedPayloadSize > 0)
                {
                    tcpSocket->Read(rxBuf + sizeof(tst_LOAD_HEADER) + totalPayloadSize - remainedPayloadSize, remainedPayloadSize);
                    if (tcpSocket->Error())
                    {
                        singleReadCount = 0;
                        wxLogError(_("Fail to read optional payload via tcp socket 0x%p, error = %d."), tcpSocket, tcpSocket->LastError());
                    }
                    else
                    {
                        singleReadCount = tcpSocket->LastCount();
                        wxLogVerbose(_("Success to read optional payload (%lu bytes) vai tcp socket 0x%p."), singleReadCount, tcpSocket);
                    }
                    remainedPayloadSize -= singleReadCount;
                }

                // Default response: same header as request
                memcpy(txBuf, rxBuf, sizeof(tst_LOAD_HEADER));

                // Process data
                ProcessDownloadModeProtocol((void *)rxBuf, (void *)txBuf);

                // Send response
                tcpSocket->Write((void *)txBuf, sizeof(tst_LOAD_HEADER));
                if (tcpSocket->Error())
                    wxLogError(_("Fail to write data via tcp socket 0x%p, error = %d."), tcpSocket, tcpSocket->LastError());
                else
                    wxLogVerbose(_("Success write data (%d bytes) via tcp socket 0x%p."), tcpSocket->LastCount(), tcpSocket);
            }
            break;

        case wxSOCKET_LOST:
            wxLogVerbose(_("Receive socket lost event on tcp socket 0x%p."), tcpSocket);
            tcpSocket->Destroy();
            numClients--;
            break;

        default:
            wxLogError(_("Unexpect event (%d) received in server accepted socket!"), notify);
            break;
        }

        delete [] rxBuf;
    }
}

bool TCPProtocol::ProcessDownloadModeProtocol(void *pIn, void *pOut)
{
    tst_LOAD_HEADER *stp_load_header = NULL;
    tst_LOAD_PACKET st_response_load_packet;
    tst_CTRL_PACKET st_response_ctrl_packet;
    unsigned char byaInitParam[4] =
    {
        5, // retry num
        4, // receive timer
        6, // response timer
        0 // big endian
    };
    unsigned char byMode;
    unsigned short u16Err = 0;
    static unsigned short u16SeqNo = 0;
    unsigned char *bypPayload = NULL;
    unsigned int u32Chksum = 0, u32TotalLoad;

    stp_load_header = (tst_LOAD_HEADER *)pIn;
    switch (stp_load_header->byProtocolId)
    {
    case SOI_BOOT_CTRL_REQUEST:
        st_response_ctrl_packet.stp_ctrl_header = (tst_CTRL_HEADER *)pOut;
        st_response_ctrl_packet.byp_ctrl_payload = (unsigned char *)pOut
            + sizeof(tst_CTRL_HEADER);
        st_response_ctrl_packet.stp_ctrl_header->byProtocolId = SOI_BOOT_CTRL_RESPONSE;
        st_response_ctrl_packet.stp_ctrl_header->byAppType = 0;
        st_response_ctrl_packet.stp_ctrl_header->byAppId = 0;
        st_response_ctrl_packet.stp_ctrl_header->u32Reserved = wxUINT32_SWAP_ON_LE(0);

        if (stp_load_header->byAppType == SOI_BOOT_CTRL_CONNECT)
        {
            st_response_ctrl_packet.stp_ctrl_header->byControlId
                = SOI_BOOT_CTRL_ACK_CONNECT;
            st_response_ctrl_packet.stp_ctrl_header->u32Payload
                = wxUINT32_SWAP_ON_LE(SOI_BOOT_MAX_RX_SIZE);
            memcpy(&(st_response_ctrl_packet.stp_ctrl_header->u32SeqNo),
                &byaInitParam[0], 4);
        }
        break;

    case SOI_BOOT_LOAD_REQUEST:
        bypPayload = (unsigned char *)pIn + sizeof(tst_LOAD_HEADER);
        st_response_load_packet.stp_load_header = (tst_LOAD_HEADER *)pOut;
        st_response_load_packet.byp_load_payload = (unsigned char *)pOut
            + sizeof(tst_LOAD_HEADER);

        // Default response packet
        memcpy((unsigned char *)st_response_load_packet.stp_load_header,
            (unsigned char *)stp_load_header, sizeof(tst_LOAD_HEADER));
        st_response_load_packet.stp_load_header->u32Chksum = wxUINT32_SWAP_ON_LE(0);

        if (stp_load_header->byAppType == SOI_BOOT_LOAD_APPTYPE)
        {
            byMode = stp_load_header->byModeId;
            switch (byMode)
            {
            case MODE_RESET_RESERVED_FLASH:
                wxLogMessage(_("Reset reserved flash"));
                u16Err = 0;
                break;

            case MODE_RUN_FW_FROM_FLASH:
                wxLogMessage(_("Run FW from flash"));
                u16Err = 0;
                break;

            case MODE_RUN_FW_FROM_RAM:
                wxLogMessage(_("Run FW from ram"));
                u16Err = 0;
                break;

            case MODE_UPDATE_MAC_ADDRESS:
                if (((bypPayload[0] + bypPayload[1] + bypPayload[2] + bypPayload[3] 
                    + bypPayload[4] + bypPayload[5]) & 0xFF) == bypPayload[6])
                {
                    u16Err = 0;
                    wxLogMessage(wxT("New MAC = %02X:%02X:%02X:%02X:%02X:%02X"),
                        bypPayload[0], bypPayload[1], bypPayload[2],
                        bypPayload[3], bypPayload[4], bypPayload[5]);
                }
                else
                {
                    wxLogError(wxT("%02X %02X %02X %02X %02X %02X %02X"),
                        bypPayload[0], bypPayload[1], bypPayload[2], bypPayload[3],
                        bypPayload[4], bypPayload[5], bypPayload[6]);
                    u16Err = 0;
                }
                break;

            case MODE_DOWNLOAD_FW_TO_FLASH:
            case MODE_DOWNLOAD_BOOTLOADER_TO_FLASH:
                if (u16SeqNo == 0)
                {
                    pst_buffer_load->u32_load_offset = 0;
                    pst_buffer_load->u32_total_load = 0;
                }
                if (stp_load_header->u16SeqNo == 0xFFFF)
                    u16SeqNo = 0;
                else
                    u16SeqNo++;

                if ((stp_load_header->u16SeqNo != 0)
                    && (stp_load_header->u16SeqNo != 0xFFFF))
                    u16SeqNo *= 1;

                // copy input to the load buffer
                bypPayload = (unsigned char *)pIn + sizeof(tst_LOAD_HEADER);
                memcpy(pst_buffer_load->byp_load_buffer + pst_buffer_load->u32_load_offset,
                    bypPayload, wxUINT32_SWAP_ON_LE(stp_load_header->u32Payload));
                pst_buffer_load->u32_total_load += wxUINT32_SWAP_ON_LE(stp_load_header->u32Payload);
                if (stp_load_header->u16SeqNo != 0xFFFF)
                    pst_buffer_load->u32_load_offset += wxUINT32_SWAP_ON_LE(stp_load_header->u32Payload);
                else
                {
                    // Full load is received
                    /* check sum */
                    u32TotalLoad = pst_buffer_load->u32_total_load;
                    while (u32TotalLoad--)
                        u32Chksum += *(pst_buffer_load->byp_load_buffer + u32TotalLoad);

                    /* check sum is not matched */
                    if (u32Chksum != wxUINT32_SWAP_ON_LE(stp_load_header->u32Chksum))
                    {
                        wxLogError(_("Verify checksum in memory load buffer failed!"));
                        u16Err = ERR_BL_RAM_CHECKSUM;
                    }
                    /* check sum is matched */
                    else
                    {
                        /* real cube will erase, write, read back flash */
                        u32TotalLoad = pst_buffer_load->u32_total_load;
                        while (u32TotalLoad--)
                        {
                            st_response_load_packet.stp_load_header->u32Chksum +=
                                *(pst_buffer_load->byp_load_buffer + u32TotalLoad);
                        }
                        st_response_load_packet.stp_load_header->u32Chksum 
                            = wxUINT32_SWAP_ON_LE(st_response_load_packet.stp_load_header->u32Chksum);
                        u16Err = 0;

                        /* in this simulated application, we save the whole image to a file */
                        wxDateTime now = wxDateTime::Now();
                        wxString fileName;
                        if (byMode == MODE_DOWNLOAD_FW_TO_FLASH)
                            fileName << wxT("FW-");
                        else if (byMode == MODE_DOWNLOAD_BOOTLOADER_TO_FLASH)
                            fileName << wxT("BTL-");
                        else
                            fileName << wxT("WHAT-");
                        fileName << now.Format(wxT("%y%m%d-%H%M%S")) << wxT(".brec");
                        wxFFileOutputStream ofs(fileName);
                        if (ofs.IsOk())
                        {
                            ofs.Write(pst_buffer_load->byp_load_buffer, pst_buffer_load->u32_total_load);
                            wxLogMessage(_("Write %d bytes to file: %s"), ofs.LastWrite(), fileName);
                            ofs.Close();
                        }
                    }
                }
                break;
            default:
            case MODE_DOWNLOAD_FW_TO_RAM:
                u16Err = ERR_BL_LOAD_MODE;
                break;
            }
        }
        st_response_load_packet.stp_load_header->byProtocolId = SOI_BOOT_LOAD_RESPONSE;
        st_response_load_packet.stp_load_header->u32Payload = 0;
        st_response_load_packet.stp_load_header->u16Error = wxUINT16_SWAP_ON_LE(u16Err);
        break;

    //default:
    //    wxLogError(_("Undefined SOI Updater protocol: 0x%X. Don't know how to process!"),
    //        stp_load_header->byProtocolId);
    }
    return true;
}

