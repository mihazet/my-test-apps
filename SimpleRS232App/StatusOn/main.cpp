#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    HANDLE fd;
    DCB dcb;
    char cmdBuf[8] = { 0x02, 0x04, '*', '*', 'D', 'S', 'P', 0x03 };
    // Lamp A: 0x2B, 0x00, 0x00
    // Lamp B: 0x2B, 0x01, 0x00
    // Lamp AB: 0x2B, 0x02, 0x00
    // PowerOn: 'P', 'O', 'N'
    // StatusOn: 'D', 'S', 'P'
    // StatusOff: 'A', 'J', 'S'
    DWORD actualWriteLen;

    fd = CreateFileA("\\\\.\\COM1",
        GENERIC_WRITE | GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fd == INVALID_HANDLE_VALUE)
    {
        printf("Can't open com port\n");
        return -1;
    }

    if (!GetCommState(fd, &dcb))
    {
        printf("Can't retrieve com state\n");
        CloseHandle(fd);
        return -2;
    }

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(fd, &dcb))
    {
        printf("Can't setup com state\n");
        CloseHandle(fd);
        return -3;
    }

    if (!WriteFile(fd, cmdBuf, 8, &actualWriteLen, NULL))
    {
        printf("Can't write data to com port\n");
        CloseHandle(fd);
        return -4;
    }

    if (!CloseHandle(fd))
    {
        printf("Can't close com port\n");
        return -5;
    }

    return 0;
}
