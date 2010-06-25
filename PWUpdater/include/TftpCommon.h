/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _TFTP_COMMON_H_
#define _TFTP_COMMON_H_

enum
{
    TFTP_OPCODE_RRQ     = 1,
    TFTP_OPCODE_WRQ     = 2,
    TFTP_OPCODE_DATA    = 3,
    TFTP_OPCODE_ACK     = 4,
    TFTP_OPCODE_ERROR   = 5
};

/* Definition for TFTP_SERVER_MSG_{READ,WRITE}_REQUEST */
enum
{
    TFTP_TRANSFER_MODE_ASCII,
    TFTP_TRANSFER_MODE_BINARY,

    TFTP_TRANSFER_MODE_INVALID
};

/* Definition for TFTP_{SERVER,TRANSMISSION}_MSG_ERROR */
enum
{
    TFTP_ERROR_CODE_NOT_DEFINE              = 0,
    TFTP_ERROR_CODE_FILE_NOT_FOUND          = 1,
    TFTP_ERROR_CODE_ACCESS_VIOLATION        = 2,
    TFTP_ERROR_CODE_DISK_FULL               = 3,
    TFTP_ERROR_CODE_ILLEGAL_OPERATION       = 4,
    TFTP_ERROR_CODE_UNKNOWN_TRANSFER_ID     = 5,
    TFTP_ERROR_CODE_FILE_ALREADY_EXISTS     = 6,
    TFTP_ERROR_CODE_NO_SUCH_USER            = 7,

    TFTP_ERROR_CODE_INVALID
};

#endif /* _TFTP_COMMON_H_ */

