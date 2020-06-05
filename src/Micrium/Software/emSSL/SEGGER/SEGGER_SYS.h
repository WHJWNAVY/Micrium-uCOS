/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2014 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL * Embedded Transport Layer Security                    *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL version: V2.54a                                        *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller GmbH
Licensed to:              Silicon Laboratories Inc., 400 W. Cesar Chavez, Austin, TX 78701, USA
Licensed SEGGER software: emSSL
License number:           SSL-00158
License model:            OEM LICENSE DISTRIBUTION AGREEMENT, signed June 3rd, 2018 and June 5th, 2018
----------------------------------------------------------------------
File    : SEGGER_SYS.h
Purpose : System portability API.
Revision: $Rev: 11360 $
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_SYS_H
#define SEGGER_SYS_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************
*
*       API types
*
**********************************************************************
*/

typedef struct {
  U8       IsFolder;       // nonzero indicates entry is a folder
  unsigned Size;           // file size, not valid for folders
} SEGGER_SYS_FS_FILE_INFO;

typedef int (*SEGGER_SYS_FS_ENUM_FUNC)(const char *sFilename, const SEGGER_SYS_FS_FILE_INFO *pInfo, void *pUserContext);  // Return >= 0 to continue enumeration, -ve to abort
typedef void *SEGGER_SYS_OS_MUTEX;

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Management
*/
void         SEGGER_SYS_Init                    (void);
void         SEGGER_SYS_Exit                    (void);
U32          SEGGER_SYS_GetProcessorSpeed       (void);
const char * SEGGER_SYS_GetCompiler             (void);

/*********************************************************************
*
*       I/O
*/
void         SEGGER_SYS_IO_Init                 (void);                                 // initialize I/O services (e.g. RTT, SystemView...)
void         SEGGER_SYS_IO_Print                (const char *sText);                    // print to standard output, no format applied.
void         SEGGER_SYS_IO_Printf               (const char *sFormat, ...);             // print to standard output, formatted.
void         SEGGER_SYS_IO_Printvf              (const char *sFormat, va_list Params);  // print to standard output, formatted.
void         SEGGER_SYS_IO_Error                (const char *sText);                    // print to standard error, no format applied.
void         SEGGER_SYS_IO_Errorf               (const char *sFormat, ...);             // print to standard error, formatted.
void         SEGGER_SYS_IO_Errorvf              (const char *sFormat, va_list Params);  // print to standard error, formatted.
void         SEGGER_SYS_IO_Debug                (const char *sText);                    // print to standard debug, no format applied.
void         SEGGER_SYS_IO_Debugf               (const char *sFormat, ...);             // print to standard debug, formatted.
void         SEGGER_SYS_IO_Debugvf              (const char *sFormat, va_list Params);  // print to standard debug, formatted.
int          SEGGER_SYS_IO_Gets                 (char *pText, unsigned TextByteCnt);    // read string from standard input, limited.
int          SEGGER_SYS_IO_Getc                 (void);                                 // read character from standard input.


/*********************************************************************
*
*       Timers
*/
U32          SEGGER_SYS_OS_GetTime              (void);                                 // ms timer
U64          SEGGER_SYS_OS_GetTimer             (void);                                 // high resolution timer counter
U64          SEGGER_SYS_OS_GetFrequency         (void);                                 // high resolution timer frequency
U64          SEGGER_SYS_OS_ConvertTicksToMicros (U64 Ticks);                            // convert high precision timer ticks to microseconds
U64          SEGGER_SYS_OS_ConvertMicrosToTicks (U64 Microseconds);                     // convert microseconds to high precision timer ticks
int          SEGGER_SYS_OS_HasCycleCounter      (void);
U32          SEGGER_SYS_OS_GetCycleCounter      (void);
void         SEGGER_SYS_OS_Delay                (unsigned Milliseconds);
const char * SEGGER_SYS_OS_GetTaskName          (void);
void         SEGGER_SYS_OS_Halt                 (int ExitCode);
void         SEGGER_SYS_OS_PauseBeforeHalt      (void);

/*********************************************************************
*
*       Critical regions
*/
void         SEGGER_SYS_OS_EnterCritical        (void);
void         SEGGER_SYS_OS_LeaveCritical        (void);

/*********************************************************************
*
*       Time of day
*/
U32          SEGGER_SYS_TIME_GetUnixTime        (void);      // seconds since 1/1/70

/*********************************************************************
*
*       Networking
*/
void         SEGGER_SYS_IP_Init                 (void);
void         SEGGER_SYS_IP_Exit                 (void);
int          SEGGER_SYS_IP_Open                 (const char *sHost, int Port);
int          SEGGER_SYS_IP_Bind                 (int Port);
int          SEGGER_SYS_IP_Accept               (int Socket);
int          SEGGER_SYS_IP_Close                (int Socket);
int          SEGGER_SYS_IP_CloseWait            (int Socket);
int          SEGGER_SYS_IP_Send                 (int Socket, const char aData[], int DataByteCnt, int Flags);
int          SEGGER_SYS_IP_Recv                 (int Socket, char aData[], int DataByteCnt, int Flags);
int          SEGGER_SYS_IP_QueryCanRead         (int Socket);

/*********************************************************************
*
*       Directory enumeration
*/
int          SEGGER_SYS_FS_Enumerate            (const char *sPattern, SEGGER_SYS_FS_ENUM_FUNC pfEnum, void *pUserContext);

/*********************************************************************
*
*       Logging
*/
void         SEGGER_SYS_Warnf                   (const char *sFormat, ...);
void         SEGGER_SYS_Logf                    (const char *sFormat, ...);
void         SEGGER_SYS_Warn                    (const char *sText);
void         SEGGER_SYS_Log                     (const char *sText);

#ifdef __cplusplus
}
#endif

#endif

/****** End Of File *************************************************/
