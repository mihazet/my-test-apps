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
#define UBOOT_TFTP_BLOCK_SIZE   (1468)
#define FLASH_SECTOR_SIZE       (0x20000)
#define RX_BUF_SIZE             (16 * 1024)
#define UBOOT_BOOTUP_MESSAGE    "Hit any key to stop autoboot:"
#define UBOOT_PROMPT_MESSAGE    "U-Boot> "

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
    wxString paramString, tempString;
    int searchResult;
    wxStopWatch stopWatch;
    long imageSize;
    unsigned long sectorStart, sectorEnd, sectorSize;

    switch (evt)
    {
    case UART_EVENT_QUIT:

        //
        // no payload
        //
        /* verify parameters */
        if (message.payload.size() != 0)
            break;

        result = true;

        break;

    case UART_EVENT_CONNECT:

        //
        // payload.at(0): uart port (COM* for Windows; ttyS* or ttyUSB* for Linux)
        //
        /* verify parameters */
        if (message.payload.size() != 1)
            break;

        ComPortConnect(message.payload.at(0), true);

        break;

    case UART_EVENT_DISCONNECT:

        //
        // no payload
        //
        /* verify parameters */
        if (message.payload.size() != 0)
            break;

        ComPortDisconnect(true);

        break;

    case UART_EVENT_PORT_SCAN:

        //
        // no payload
        //
        /* verify parameters */
        if (message.payload.size() != 0)
            break;

        DetectSerialPort(true);

        break;

    case UART_EVENT_DOWNLOAD_FIRST:

        //
        // payload.at(0): uart port (COM* for Windows; ttyS* or ttyUSB* for Linux)
        // payload.at(1): tftp server ip
        // payload.at(2): ddr memory address (hex string)
        // payload.at(3): image file
        // payload.at(4): image offset (hex string)
        // payload.at(5): image end (hex string)
        // payload.at(6): image size (decimal string)
        //
        /* verify parameters */
        if (message.payload.size() != 7)
            break;

        if (!ComPortConnect(message.payload.at(0), true))
        {
            NotifyDownloadResult(message.payload.at(3), UART_ERR_CONNECTION);
            break;
        }

        searchResult = WaitUartMessage(UBOOT_BOOTUP_MESSAGE, 30);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }

        SendUartMessage(" ");

        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, 3);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }

        SendUartMessage("setenv serverip %s\r", (const char *)message.payload.at(1).ToAscii());

        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, 5);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }

        SendUartMessage("tftpboot %s %s\r",
            (const char *)message.payload.at(2).ToAscii(),
            (const char *)message.payload.at(3).ToAscii());

        message.payload.at(6).ToLong(&imageSize);
        searchResult = WaitUartMessageAndCountHashSign(message.payload.at(3),
            UBOOT_PROMPT_MESSAGE, imageSize / (UBOOT_TFTP_BLOCK_SIZE * 10),
            imageSize / 10000);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }

        SendUartMessage("erase %s %s\r",
            (const char *)message.payload.at(4).ToAscii(),
            (const char *)message.payload.at(5).ToAscii());

        message.payload.at(4).ToULong(&sectorStart, 16);
        message.payload.at(5).ToULong(&sectorEnd, 16);
        sectorSize = sectorEnd - sectorStart + 1;
        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, sectorSize / FLASH_SECTOR_SIZE);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }
        else
        {
            NotifyDownloadProgress(message.payload.at(3), 75);
        }

        SendUartMessage("cp.b %s %s 0x%x\r",
            (const char *)message.payload.at(2).ToAscii(),
            (const char *)message.payload.at(4).ToAscii(),
            imageSize);

        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, (imageSize / FLASH_SECTOR_SIZE) * 6);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(3), searchResult);
            break;
        }
        else
        {
            NotifyDownloadProgress(message.payload.at(3), 100);
        }

        NotifyDownloadResult(message.payload.at(3), searchResult);

        break;

    case UART_EVENT_DOWNLOAD_NEXT:

        //
        // payload.at(0): ddr memory address (hex string)
        // payload.at(1): image file
        // payload.at(2): image offset (hex string)
        // payload.at(3): image end (hex string)
        // payload.at(4): image size (decimal string)
        //
        /* verify parameters */
        if (message.payload.size() != 5)
            break;

        SendUartMessage(" \r");

        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, 3);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(1), searchResult);
            break;
        }

        SendUartMessage("tftpboot %s %s\r",
            (const char *)message.payload.at(0).ToAscii(),
            (const char *)message.payload.at(1).ToAscii());

        message.payload.at(4).ToLong(&imageSize);
        searchResult = WaitUartMessageAndCountHashSign(message.payload.at(1),
            UBOOT_PROMPT_MESSAGE, imageSize / (UBOOT_TFTP_BLOCK_SIZE * 10),
            imageSize / 10000);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(1), searchResult);
            break;
        }

        SendUartMessage("erase %s %s\r",
            (const char *)message.payload.at(2).ToAscii(),
            (const char *)message.payload.at(3).ToAscii());

        message.payload.at(2).ToULong(&sectorStart, 16);
        message.payload.at(3).ToULong(&sectorEnd, 16);
        sectorSize = sectorEnd - sectorStart + 1;
        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, sectorSize / FLASH_SECTOR_SIZE);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(1), searchResult);
            break;
        }
        else
        {
            NotifyDownloadProgress(message.payload.at(1), 75);
        }

        SendUartMessage("cp.b %s %s 0x%x\r",
            (const char *)message.payload.at(0).ToAscii(),
            (const char *)message.payload.at(2).ToAscii(),
            imageSize);

        searchResult = WaitUartMessage(UBOOT_PROMPT_MESSAGE, (imageSize / FLASH_SECTOR_SIZE) * 6);
        if (searchResult != UART_ERR_MSG_FOUND)
        {
            NotifyDownloadResult(message.payload.at(1), searchResult);
            break;
        }
        else
        {
            NotifyDownloadProgress(message.payload.at(1), 100);
        }

        NotifyDownloadResult(message.payload.at(1), searchResult);

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

bool UartThread::ComPortConnect(const wxString &port, bool notify)
{
    int error;
    wxString portString = port;
#if defined (__WXMSW__)
    long longTemp;
    if (portString.length() > 4)
    {
        wxString numString = portString.Right(portString.length() - 3);
        numString.ToLong(&longTemp);
        portString.Printf(wxT("\\\\.\\COM%ld"), longTemp);
    }
#elif defined (__WXGTK__)
    portString.Prepend(wxT("/dev/"));
#endif

    if (_comPort.IsOpen())
        _comPort.Close();
    error = _comPort.Open(portString.ToAscii());
    if (_comPort.IsOpen())
        _comPort.SetBaudRate(wxBAUD_115200);

    /* generate feedback to main thread */
    if (notify && (error == 0))
    {
        wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
        UartMessage response(UART_EVENT_CONNECTED);
        response.payload.push_back(port);
        evt.SetPayload<UartMessage>(response);
        wxQueueEvent(_pHandler, evt.Clone());    
    }

    return (error == 0);
}

bool UartThread::ComPortDisconnect(bool notify)
{
    if (_comPort.IsOpen())
        _comPort.Close();

    /* generate feedback to main thread */
    if (notify)
    {
        wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
        UartMessage response(UART_EVENT_DISCONNECTED);
        evt.SetPayload<UartMessage>(response);
        wxQueueEvent(_pHandler, evt.Clone());
    }

    return true;
}

int UartThread::WaitUartMessage(const char *msg, int timeout_second)
{
    int result = UART_ERR_MSG_NOT_FOUND;
    char *rxBuf = NULL;
    size_t msgLength;
    bool msgFound = false;
    int readTotal, readCnt, compareIndex;
    unsigned long dbg_read_no_data_cnt, dbg_read_data_cnt, dbg_cmp_cnt;
    wxStopWatch sw;

    /* parameters check */
    if (!msg)
        return UART_ERR_PARAMS;
    if (!_comPort.IsOpen())
        return UART_ERR_CONNECTION;

    /* allocate internal buffer */
    rxBuf = new char [RX_BUF_SIZE];

    /* start to read data and search message byte by byte */
    msgLength = strlen(msg);
    readTotal = compareIndex = 0;
    dbg_read_no_data_cnt = dbg_read_data_cnt = dbg_cmp_cnt = 0;
    sw.Start();
    while (true)
    {
        readCnt = _comPort.Readv(&rxBuf[readTotal], RX_BUF_SIZE - readTotal, 10);
        if (readCnt > 0)
        {
            dbg_read_data_cnt++;
            readTotal += readCnt;
            while ((readTotal - compareIndex) >= (int)msgLength)
            {
                dbg_cmp_cnt++;
                if (memcmp(&rxBuf[compareIndex], msg, msgLength))
                    compareIndex++;
                else
                {
                    result = UART_ERR_MSG_FOUND;
                    msgFound = true;
                    break;
                }
            }

            if (msgFound)
                break;

            if (readTotal >= RX_BUF_SIZE)
            {
                result = UART_ERR_MSG_NOT_FOUND;
                break;
            }
        }
        else if (readCnt == -1)
        {
            result = UART_ERR_READ;
            wxLogError(wxT("Fail to read data from serial port"));
            break;
        }
        else
        {
            dbg_read_no_data_cnt++;
        }

        if (sw.Time() >= (timeout_second * 1000))
        {
            result = UART_ERR_TIMEOUT;
            break;
        }
    }

    wxLogMessage(wxT("Wait [%s], Read cnt = %lu/%lu, Compare cnt = %lu, Used time = %ld"),
        msg, dbg_read_data_cnt, dbg_read_no_data_cnt, dbg_cmp_cnt, sw.Time());

    delete [] rxBuf;

    return result;
}

int UartThread::WaitUartMessageAndCountHashSign(const wxString &image,
                                                const char *msg,
                                                int hash_total,
                                                int timeout_second)
{
    int result = UART_ERR_MSG_NOT_FOUND;
    char *rxBuf = NULL;
    size_t msgLength;
    bool msgFound = false;
    int readTotal, readCnt, compareIndex, hashSignCnt, intProgress, intTemp;
    float fProgress;
    unsigned long dbg_read_no_data_cnt, dbg_read_data_cnt, dbg_cmp_cnt;
    wxStopWatch sw;

    /* parameters check */
    if (!msg)
        return UART_ERR_PARAMS;
    if (!_comPort.IsOpen())
        return UART_ERR_CONNECTION;

    /* allocate internal buffer */
    rxBuf = new char [RX_BUF_SIZE];

    /* start to read data and search message byte by byte */
    msgLength = strlen(msg);
    readTotal = compareIndex = hashSignCnt = intProgress = 0;
    dbg_read_no_data_cnt = dbg_read_data_cnt = dbg_cmp_cnt = 0;
    sw.Start();
    while (true)
    {
        readCnt = _comPort.Readv(&rxBuf[readTotal], RX_BUF_SIZE - readTotal, 10);
        if (readCnt > 0)
        {
            dbg_read_data_cnt++;
            readTotal += readCnt;
            
            /* count for hash sign */
            hashSignCnt = 0;
            for (intTemp = 0; intTemp < readTotal; intTemp++)
            {
                if (rxBuf[intTemp] == '#')
                    hashSignCnt++;
            }
            fProgress = (50.0 * hashSignCnt) / hash_total;
            if ((fProgress - intProgress) > 10.0)
            {
                intProgress += 10;
                NotifyDownloadProgress(image, intProgress);
            }

            /* check message availablity */
            while ((readTotal - compareIndex) >= (int)msgLength)
            {
                dbg_cmp_cnt++;
                if (memcmp(&rxBuf[compareIndex], msg, msgLength))
                    compareIndex++;
                else
                {
                    result = UART_ERR_MSG_FOUND;
                    msgFound = true;
                    break;
                }
            }

            if (msgFound)
                break;

            if (readTotal >= RX_BUF_SIZE)
            {
                result = UART_ERR_MSG_NOT_FOUND;
                break;
            }
        }
        else if (readCnt == -1)
        {
            result = UART_ERR_READ;
            wxLogError(wxT("Fail to read data from serial port"));
            break;
        }
        else
        {
            dbg_read_no_data_cnt++;
        }

        if (sw.Time() >= (timeout_second * 1000))
        {
            result = UART_ERR_TIMEOUT;
            break;
        }
    }

    wxLogMessage(wxT("Wait [%s], Read cnt = %lu/%lu, Compare cnt = %lu, Used time = %ld"),
        msg, dbg_read_data_cnt, dbg_read_no_data_cnt, dbg_cmp_cnt, sw.Time());

    delete [] rxBuf;

    return result;
}

int UartThread::SendUartMessage(const char *msg, ...)
{
    if (msg && _comPort.IsOpen())
    {
        size_t len;
        char txBuf[256];
        memset(txBuf, 0, sizeof(txBuf));
        va_list var;
        va_start(var, msg);
        vsprintf(txBuf, msg, var);
        va_end(var);
        len = strlen(txBuf);
        
        return _comPort.Write((char *)txBuf, len < 256 ? len : 256);
    }

    return 0;
}

void UartThread::NotifyDownloadResult(const wxString &image, int error_code)
{
    wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
    UartMessage response(UART_EVENT_DOWNLOAD_RESULT);
    response.payload.push_back(image);
    response.payload.push_back(wxString::Format(wxT("%d"), error_code));
    evt.SetPayload<UartMessage>(response);
    wxQueueEvent(_pHandler, evt.Clone());
}

void UartThread::NotifyDownloadProgress(const wxString &image, int progress)
{
    wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
    UartMessage response(UART_EVENT_DOWNLOAD_PROGRESS);
    response.payload.push_back(image);
    response.payload.push_back(wxString::Format(wxT("%d"), progress));
    evt.SetPayload<UartMessage>(response);
    wxQueueEvent(_pHandler, evt.Clone());
}
