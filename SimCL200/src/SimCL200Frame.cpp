// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/ctb-0.13/serport.h"
#include "SimCL200Frame.h"

#include "port_32_16.xpm"
#include "baud_32_16.xpm"
#include "csize_32_16.xpm"
#include "parity_32_16.xpm"
#include "stopb_32_16.xpm"

#define RING_BUFFER_SIZE    1024
#define CL200_PROTOCOL_STX  0x02
#define CL200_PROTOCOL_ETX  0x03
#define CL200_PROTOCOL_CR   0x0D
#define CL200_PROTOCOL_LF   0x0A

enum
{
    myID_PORT = wxID_HIGHEST + 1,
    myID_BAUDRATE,
    myID_CHARSIZE,
    myID_STOPBITS,
    myID_PARITY,

    // timer
    myID_READ_SERIAL_PORT_TIMER,
};

IMPLEMENT_CLASS(SimCL200Frame, wxFrame)

BEGIN_EVENT_TABLE(SimCL200Frame, wxFrame)
    EVT_TIMER(myID_READ_SERIAL_PORT_TIMER, SimCL200Frame::OnReadSerialPortTimer)
    EVT_RIBBONBUTTONBAR_CLICKED(myID_BAUDRATE, SimCL200Frame::OnBaudRateClicked)
    EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(myID_BAUDRATE, SimCL200Frame::OnBaudRateDropDown)
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
    if (m_readTimer)
    {
        if (m_readTimer->IsRunning())
            m_readTimer->Stop();
        delete m_readTimer;
    }

    if (m_port)
    {
        if (m_port->IsOpen())
            m_port->Close();
        delete m_port;
    }

    if (m_ringBuffer)
        delete [] m_ringBuffer;
}

void SimCL200Frame::Init()
{
    /* ring buffer */
    m_ringBuffer = new unsigned char[RING_BUFFER_SIZE];
    m_parseIndex = m_appendIndex = 0;

    /* serial port */
    m_port = new wxSerialPort();
    wxSerialPort_DCS dcs;
    dcs.baud = wxBAUD_9600;
    dcs.parity = wxPARITY_EVEN;
    dcs.wordlen = 7;
    dcs.stopbits = 1;
    m_port->Open(wxCOM8, &dcs);

    /* read timer */
    m_readTimer = new wxTimer(this, myID_READ_SERIAL_PORT_TIMER);
}

void SimCL200Frame::CreateControls()
{
    SetStatusBar(CreateStatusBar());

    m_ribbon = new wxRibbonBar(this);

    /* serial port page */
    wxRibbonPage *portPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("SerialPort"));
    wxRibbonPanel *connectionPanel = new wxRibbonPanel(portPage, wxID_ANY, wxT("Connection"), wxNullBitmap,
        wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
    wxRibbonToolBar *connectionToolBar = new wxRibbonToolBar(connectionPanel);
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Serial port selection"), wxRIBBON_BUTTON_HYBRID);
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Scan all not used port on system")); // scan
    connectionToolBar->AddSeparator();
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Connect to specific port")); // connect
    connectionToolBar->AddTool(wxID_ANY, port_32_16_xpm, wxT("Disconnect with specific port")); // disconnect
    connectionToolBar->SetRows(2, 3);
    
    wxRibbonPanel *configPanel = new wxRibbonPanel(portPage, wxID_ANY, wxT("Configuration"), wxNullBitmap,
        wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
    wxRibbonButtonBar *configBtnBar = new wxRibbonButtonBar(configPanel);
    configBtnBar->AddButton(myID_BAUDRATE, wxT("Baud Rate"), baud_32_16_xpm, wxT("Baud Rate Selection"), wxRIBBON_BUTTON_HYBRID);
    configBtnBar->AddButton(myID_CHARSIZE, wxT("Char Size"), csize_32_16_xpm, wxT("Character Size Selection"));
    configBtnBar->AddButton(myID_PARITY, wxT("Parity"), parity_32_16_xpm, wxT("Parity Selection"), wxRIBBON_BUTTON_DROPDOWN);
    configBtnBar->AddButton(myID_STOPBITS, wxT("Stop Bit"), stopb_32_16_xpm, wxT("Stop Bit Selection"));

    /* dummy page */
    new wxRibbonPage(m_ribbon, wxID_ANY, wxT("CL-200"));

    m_ribbon->Realize();

    wxTextCtrl *log = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);

    wxSizer *frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(m_ribbon, 0, wxEXPAND);
    frameSizer->Add(log, 1, wxEXPAND);

    SetSizerAndFit(frameSizer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers
///////////////////////////////////////////////////////////////////////////////////////////////////
void SimCL200Frame::OnBaudRateClicked(wxRibbonButtonBarEvent &WXUNUSED(event))
{
    wxLogDebug(wxT("Baud rate button clicked"));

    if (m_readTimer->IsRunning())
        m_readTimer->Stop();
    else
        m_readTimer->Start(100);
}

void SimCL200Frame::OnBaudRateDropDown(wxRibbonButtonBarEvent &WXUNUSED(event))
{
    wxLogDebug(wxT("Baud rate drop down clicked"));
}

void SimCL200Frame::AppendDataToRingBuffer(unsigned char *buf, size_t len)
{
    unsigned long len1, len2;

    if (m_appendIndex + len >= RING_BUFFER_SIZE)
    {
        len2 = RING_BUFFER_SIZE - m_appendIndex;
        len1 = len - len2;
        memcpy(&m_ringBuffer[m_appendIndex], &buf[0], len2);
        memcpy(&m_ringBuffer[0], &buf[len2], len1);
        m_appendIndex = len1;
    }
    else
    {
        memcpy(&m_ringBuffer[m_appendIndex], buf, len);
        m_appendIndex += len;
    }
}

void SimCL200Frame::ParseDataLengthFinish(unsigned long len)
{
    m_parseIndex += len;
    if (m_parseIndex >= RING_BUFFER_SIZE)
        m_parseIndex -= RING_BUFFER_SIZE;
}

unsigned char SimCL200Frame::GetDataAtRingBufferOffset(unsigned long offset)
{
    unsigned long pos = m_parseIndex + offset;
    if (pos >= RING_BUFFER_SIZE)
        return m_ringBuffer[pos - RING_BUFFER_SIZE];
    else
        return m_ringBuffer[pos];
}

void SimCL200Frame::ProcessCL200ShortMessage(unsigned char *msg)
{
    wxLogDebug(wxT("Short Message: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"),
        msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], msg[11], msg[12], msg[13]);
}

void SimCL200Frame::ProcessCL200LongMessage(unsigned char *msg)
{
    wxLogDebug(wxT("Long Message"));
}

void SimCL200Frame::OnReadSerialPortTimer(wxTimerEvent &event)
{
    unsigned char buf[256];
    size_t nRead, unparsedLen;
    wxDateTime current;
    bool fragmented = false;

    memset(buf, 0, sizeof(buf));
    nRead = m_port->Read((char *)&buf[0], sizeof(buf));
    if (nRead > 0)
    {
        /* record current date and time */
        current = wxDateTime::UNow();

        /* append data into ring buffer */
        if (m_parseIndex == m_appendIndex)
            AppendDataToRingBuffer(buf, nRead);
        else
        {
            if (current.Subtract(m_previousAppendTime).GetMilliseconds() > 1000)
            {
                wxLogDebug(wxT("Discard origin data in ring buffer"));
                m_parseIndex = m_appendIndex = 0;
            }
            AppendDataToRingBuffer(buf, nRead);
        }

        /* update received date and time */
        m_previousAppendTime = current;

        /* parse ring buffer */
        while (m_parseIndex != m_appendIndex)
        {
            if (m_appendIndex > m_parseIndex)
                unparsedLen = m_appendIndex - m_parseIndex;
            else
                unparsedLen = RING_BUFFER_SIZE - (m_parseIndex - m_appendIndex);

            if (unparsedLen < 14)
            {
                // CL-200 only has two kind of length message, short for (14) and long for (32) bytes, respectively.
                fragmented = true;
            }
            else if (GetDataAtRingBufferOffset(0) == CL200_PROTOCOL_STX)
            {
                if ((GetDataAtRingBufferOffset(1) == '9')
                    || ((GetDataAtRingBufferOffset(1) >= '0') 
                        && (GetDataAtRingBufferOffset(1) <= '2')))
                {
                    if ((GetDataAtRingBufferOffset(2) >= '0') 
                        && (GetDataAtRingBufferOffset(2) <= '9'))
                    {
                        if ((GetDataAtRingBufferOffset(3) >= '0') 
                            && (GetDataAtRingBufferOffset(3) <= '9'))
                        {
                            if ((GetDataAtRingBufferOffset(4) >= '0') 
                                && (GetDataAtRingBufferOffset(4) <= '9'))
                            {
                                /* check short message format first */
                                if ((GetDataAtRingBufferOffset(9) == CL200_PROTOCOL_ETX)
                                    && (GetDataAtRingBufferOffset(12) == CL200_PROTOCOL_CR)
                                    && (GetDataAtRingBufferOffset(13) == CL200_PROTOCOL_LF))
                                {
                                    ProcessCL200ShortMessage(&m_ringBuffer[m_parseIndex]);
                                    ParseDataLengthFinish(14);
                                }
                                /* check long messge format then */
                                else if (unparsedLen >= 32)
                                {
                                    if ((GetDataAtRingBufferOffset(9) == CL200_PROTOCOL_ETX)
                                        && (GetDataAtRingBufferOffset(30) == CL200_PROTOCOL_CR)
                                        && (GetDataAtRingBufferOffset(31) == CL200_PROTOCOL_LF))
                                    {
                                        ProcessCL200LongMessage(&m_ringBuffer[m_parseIndex]);
                                        ParseDataLengthFinish(32);
                                    }
                                    else
                                        ParseDataLengthFinish(5);
                                }
                                else
                                {
                                    fragmented = true;
                                }
                            }
                            else
                                ParseDataLengthFinish(4);
                        }
                        else
                            ParseDataLengthFinish(3);
                    }
                    else
                        ParseDataLengthFinish(2);
                }
                else
                    ParseDataLengthFinish(1);
            }
            else
            {
                ParseDataLengthFinish(1);
            }

            /* stop to parse and continue to receive if data is fragmented */
            if (fragmented)
                break;
        }
    }
}
