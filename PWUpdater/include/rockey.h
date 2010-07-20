// High order characters
#ifndef _ROCKEY4_ND_32_
#define _ROCKEY4_ND_32_
//function id
/* Basic format :
(1) Search locks
    input parameters:
    function = 0
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    return:
    *lp1 hardware ID
     0 :success, others:error code

(2) search next lock
    input parameters:
    function = 1
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    return:
    *lp1 hardware  ID
     0 :success, others:error code

(3) open lock
    input parameters:
    function = 2
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    *lp1 = hardware ID
    return:
    *handle lock handle
     0 :success, others:error code


(4)close lock
    input parameters:
    function = 3
    *handle = lock handle
    return:
    0 :success, others:error code

(5) read lock
    input parameters:
    function = 4
    *handle = lock handle
    *p1 = pos
    *p2 = length
    buffer = buffer pointer
    return:
    adding read contents in buffer
     0 :success, others:error code

(6) write lock
    function = 5
    *handle = lock handle
    *p1 = pos
    *p2 = length
    buffer = buffer pointer
    return:
    0 :success, others:error code

(7) random number
    function = 6
    *handle = lock handle
    return:
    *p1,*p2,*p3,*p4 = random number
     0 :success, others:error code

(8) seeds code
    function = 7
    *handle = lock handle
    *lp2 = seeds code
    *p1 = returning code1
    *p2 = returning code2
    *p3 = returning code3
    *p4 = returning code4
     return:
     0 :success, others:error code

(9) write user's ID [*]
    function = 8
    *handle = lock handle
    *lp1 = user ID
    return:
    0 :success, others:error code

(10) read user's ID
     function = 9
     *handle = lock handle
     return:
     *lp1 = user's ID
     0 :success, others:error code

(11) set modules[*]
     function = 10
     *handle = lock handle
     *p1 = module number
     *p2 = user's module password
     *p3 = whether Permitting reduced( 1 =permit , 0 = forbid)
     return:
     0 :success, others:error code

(12) Check the validity of modules
     function = 11
     *handle = lock handle
     *p1 = module number
     return:
     *p2 = 1 this module effective
     *p3 = 1 this module can be reduced
     0 :success, others:error code

(13) write algorithm [*]
     function = 12
     *handle = lock handle
     *p1 = pos
     buffer = algorithm directive string
     return:
     0 :success, others:error code

(14) compute 1(module characters, high ID , low ID , random number)
     function = 13
     *handle = lock handle
     *lp1 = start position
     *lp2 = module number
     *p1 = input value1
     *p2 = input value2
     *p3 = input value3
     *p4 = input value4
     *p1 = return value1
     *p2 = return value2
     *p3 = return value3
     *p4 = return value4
     return:
     0 :success, others:error code


(15) compute2
     function = 14
     *handle = lock handle
     *lp1 = start position
     *lp2 = seeds code
     *p1 = input value1
     *p2 = input value2
     *p3 = input value3
     *p4 = input value4
     return :
     *p1 = return value1
     *p2 = return value2
     *p3 = return value3
     *p4 = return value4
     0 :success, others:error code


(16) compute 3
     function = 15
     *handle = lock handle
     *lp1 = start position
     *lp2 = originating address of password
     *p1 = input value 1
     *p2 = input value2
     *p3 = input value3
     *p4 = input value4
     *p1 = return value1
     *p2 = return value2
     *p3 = return value 3
     *p4 = return value4
     return:
     0 :success, others:error code

(17) Decreasing
     function = 16
     *handle = lock handle
     *p1 = module number
     0 :success, others:error code
*/

#define  RY_FIND                        1		//search lock
#define  RY_FIND_NEXT					2		//search next lock
#define  RY_OPEN                        3		//open lock
#define  RY_CLOSE                       4		//close lock
#define  RY_READ                        5		//read lock
#define  RY_WRITE                       6		//write lock
#define  RY_RANDOM                      7		//produce random numbers
#define  RY_SEED                        8		//produce seeds code
#define  RY_WRITE_USERID				9		//write user's ID
#define  RY_READ_USERID					10		//read user's ID
#define  RY_SET_MODULE					11		//set module characters
#define  RY_CHECK_MODULE				12		//check module state
#define  RY_WRITE_ARITHMETIC            13		//write algorithm
#define  RY_CALCULATE1					14		//compute 1
#define  RY_CALCULATE2					15		//compute 2
#define  RY_CALCULATE3					16		//compute 3
#define  RY_DECREASE					17		//decreasing module unit

// error code
#define ERR_SUCCESS				0 //correct
#define ERR_NO_ROCKEY			3 // no ROCKEY
#define ERR_INVALID_PASSWORD	4 //ROCKEY is exist,but the basic password is wrong.
#define ERR_INVALID_PASSWORD_OR_ID	5 //wrong password or hardware ID
#define ERR_SETID				6 //set wrong hardware ID
#define ERR_INVALID_ADDR_OR_SIZE	7 //wrong address or size in read or write
#define ERR_UNKNOWN_COMMAND		8 //no this  command
#define ERR_NOTBELEVEL3			9 //Internal error
#define ERR_READ				10 //read data error
#define ERR_WRITE				11 //write data error
#define ERR_RANDOM				12 //random code error
#define ERR_SEED				13 //seeds code error
#define ERR_CALCULATE			14 //computing error
#define ERR_NO_OPEN				15 //don't open lock before operation
#define ERR_OPEN_OVERFLOW		16 //open too many locks(>16)
#define ERR_NOMORE				17 // search locks no more
#define ERR_NEED_FIND			18 // no find,use findnext
#define ERR_DECREASE			19 //decreasing error
#define ERR_AR_BADCOMMAND		20 //algorithm command error
#define ERR_AR_UNKNOWN_OPCODE	21 //algorithm opertion symbols error
#define ERR_AR_WRONGBEGIN		22 //there is constants in the first algorithm  command
#define ERR_AR_WRONG_END		23 //there is constants in the last algorithm  command
#define ERR_AR_VALUEOVERFLOW	24 //constants value > 63
#define ERR_TOOMUCHTHREAD		25 //thread numbers with opening lock at  the same process> 100
#define ERR_RECEIVE_NULL		0x100 //receive nothing
#define ERR_UNKNOWN_SYSTEM		0x102 //unknown os
#define ERR_UNKNOWN				0xffff //unknown error

#ifdef __cplusplus
extern "C" {
#endif

short rockey(unsigned short function, unsigned short * handle, unsigned int * lp1,  unsigned int * lp2, unsigned short * p1, unsigned short * p2, unsigned short * p3, unsigned short * p4,unsigned char* buffer);

#ifdef __cplusplus
}
#endif

#endif
