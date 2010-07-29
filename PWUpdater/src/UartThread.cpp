/*
 *  UartThread.cpp - A thread to communicate with U-Boot on target board.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/stopwatch.h>
#ifdef __WXGTK__
#include <glob.h>
#endif
#include "PWUpdater.h"
#include "UartThread.h"

#define wxLOG_COMPONENT "PWUpdater/uart"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
#define RX_BUF_SIZE     4096
#define UBOOT_BOOTUP_MESSAGE    "Hit any key to stop autoboot:"

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
UartThread::UartThread(wxEvtHandler *handler, const int id)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _threadEventId(id)
{
}

UartThread::~UartThread()
{
    wxCriticalSection &cs = wxGetApp().m_uartCS;
    UartThread *&pUart = wxGetApp().m_pUartThread;

    cs.Enter();
    pUart = NULL;
    cs.Leave();
}

wxThread::ExitCode UartThread::Entry()
{
    ThreadSafeQueue<UartMessage> *&pQueue = wxGetApp().m_pUartQueue;
    UartMessage message;
    bool quit = false;

    /* register as queue consumer */
    pQueue->SetConsumer();

    /* detect serial and notify main thread the result */
    DetectSerialPort(true);

    while (!quit)
    {
        pQueue->Wait();
        message = pQueue->DeQueue();

        quit = ProcessMessage(message);

        while (!quit)
        {
            if (!pQueue->Empty())
            {
                message = pQueue->DeQueue();

                quit = ProcessMessage(message);
            }
            else
                break;
        }
    }

    return (wxThread::ExitCode)0;
}

bool UartThread::ProcessMessage(const UartMessage &message)
{
    bool result = false;
    int evt = message.event;
    UartMessage response;
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    wxString paramString;
    long longTemp;
    char *rxBuf = NULL;
    int readCnt, readTotal, parseIndex, downloadError = UART_ERR_NO_ERROR;
    size_t parseThreshold;
    bool foundBootUpMessage = false;
    unsigned long dbg_read_cnt, dbg_cmp_cnt;
    wxStopWatch stopWatch;

    switch (evt)
    {
    case UART_EVENT_QUIT:

        result = true;
        break;

    case UART_EVENT_CONNECT:

        /* verify parameters */
        if (message.payload.size() != 1)
            break;

        /* do connect */
        paramString = message.payload.at(0);
#if defined (__WXMSW__)
        if (paramString.length() > 4)
        {
            wxString numString = paramString.Right(paramString.length() - 3);
            numString.ToLong(&longTemp);
            paramString.Printf(wxT("\\\\.\\COM%ld"), longTemp);
        }
#elif defined (__WXGTK__)
        paramString.Prepend(wxT("/dev/"));
#endif
        _comPort.Open(paramString.ToAscii());
        _comPort.SetBaudRate(wxBAUD_115200);

        /* generate feedback to main thread */
        response.event = UART_EVENT_CONNECTED;
        response.payload.push_back(message.payload.at(0));
        event.SetPayload<UartMessage>(response);
        wxQueueEvent(_pHandler, event.Clone());        
        break;

    case UART_EVENT_DISCONNECT:

        /* do disconnect */
        if (_comPort.IsOpen())
            _comPort.Close();

        /* generate feedback to main thread */
        response.event = UART_EVENT_DISCONNECTED;
        event.SetPayload<UartMessage>(response);
        wxQueueEvent(_pHandler, event.Clone());
        break;

    case UART_EVENT_PORT_SCAN:

        DetectSerialPort(true);
        break;

    case UART_EVENT_DOWNLOAD_FIRST:

        /* verify parameters */
        if (message.payload.size() != 4)
            break;

        /* wait for u-boot reboot message */
        rxBuf = new char[RX_BUF_SIZE];
        parseThreshold = strlen(UBOOT_BOOTUP_MESSAGE);
        readTotal = 0;
        parseIndex = 0;
        dbg_read_cnt = dbg_cmp_cnt = 0;
        stopWatch.Start();
        while (true)
        {
            readCnt = _comPort.Readv(&rxBuf[readTotal], RX_BUF_SIZE - readTotal, 10);
            dbg_read_cnt++;
            if (readCnt > 0)
            {
                readTotal += readCnt;
                while ((readTotal - parseIndex) >= (int)parseThreshold)
                {
                    dbg_cmp_cnt++;
                    if (memcmp(&rxBuf[parseIndex], UBOOT_BOOTUP_MESSAGE, parseThreshold))
                        parseIndex++;
                    else
                    {
                        foundBootUpMessage = true;
                        break;
                    }
                }

                if (foundBootUpMessage)
                    break;

                if (readTotal >= RX_BUF_SIZE)
                {
                    downloadError = UART_ERR_NO_UBOOT;
                    break;
                }
            }
            else if (readCnt == -1)
            {
                wxLogError(_("Fail to read data from serial port"));
                downloadError = UART_ERR_READ;
                break;
            }
            // data is not available or enough, try again if not timeout
            if (stopWatch.Time() >= 30 * 1000)
            {
                downloadError = UART_ERR_NO_UBOOT;
                break;
            }
        }
        wxLogMessage(wxT("read cnt = %lu, cmp cnt = %lu"), dbg_read_cnt, dbg_cmp_cnt);
        
        if (foundBootUpMessage)
        {
            /* send any key to stop autoboot */
            _comPort.Write("\r\n", 2);

            /* fflush serial buffer */
            do
            {
                readCnt = _comPort.Readv(&rxBuf[0], RX_BUF_SIZE, 100);
            } while (readCnt == RX_BUF_SIZE);
        }
        delete [] rxBuf;

        /* generate feedback to main thread */
        response.event = UART_EVENT_DOWNLOAD_RESULT;
        response.payload.push_back(message.payload.at(1));
        response.payload.push_back(wxString::Format(wxT("%d"), downloadError));
        event.SetPayload<UartMessage>(response);
        wxQueueEvent(_pHandler, event.Clone());
        break;

    case UART_EVENT_DOWNLOAD_NEXT:

        /* verify parameters */
        if (message.payload.size() != 4)
            break;



        break;

    default:
        wxLogError(wxT("Undefined uart event: %d"), evt);
        break;
    }

    return result;
}

//
// Detect free serial port and send the result to main thread
//
int UartThread::DetectSerialPort(bool notify)
{
    wxThreadEvent event(wxEVT_COMMAND_THREAD, _threadEventId);
    UartMessage message(UART_EVENT_PORT_DETECTION);
    char port[16];
    wxSerialPort com;
    unsigned int id;

#if defined (__WXMSW__)
    for (id = 0; id < 100; id++)
    {
        COMMCONFIG cc;
        DWORD dwSize = sizeof(cc);

        sprintf(&port[0], "COM%u", id);
        if (::GetDefaultCommConfigA(port, &cc, &dwSize))
        {
            if (cc.dwProviderSubType == PST_RS232)
            {
                if (id >= 10)
                    sprintf(&port[0], "\\\\.\\COM%u", id);
                if (com.Open(port) < 0)
                    continue;
                com.Close();

                message.payload.push_back(wxString::Format(wxT("COM%u"), id));
            }
        }
    }
#elif defined (__WXGTK__)
    glob_t globbuf;

    // search standard serial port
    strcpy(&port[0], "/dev/ttyS*");
    if (0 == glob(port, GLOB_ERR, NULL, &globbuf))
    {
        // no error, glob was successful
        for (id = 0; id < globbuf.gl_pathc; id++)
        {
            if (com.Open(globbuf.gl_pathv[id]) < 0)
                continue;
            com.Close();

            message.payload.push_back(wxString::Format(wxT("ttyS%u"), id));
        }
    }
    globfree(&globbuf);

    // search for USB to RS232 converters
    strcpy(&port[0], "/dev/ttyUSB*");
    if (0 == glob(port, GLOB_ERR, NULL, &globbuf))
    {
        for (id = 0; id < globbuf.gl_pathc; id++)
        {
            if (com.Open(globbuf.gl_pathv[id]) < 0)
                continue;
            com.Close();

            message.payload.push_back(wxString::Format(wxT("ttyUSB%u"), id));
        }
    }
    globfree(&globbuf);
#endif

    /* notify the main thread */
    if (notify)
    {
        event.SetPayload<UartMessage>(message);
        wxQueueEvent(_pHandler, event.Clone());
    }

    return message.payload.size();
}
