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
-------------------------- END-OF-HEADER -----------------------------

File        : SSL_ConfDefaults.h
Purpose     : Defines defaults for most configurable defines used in emSSL.
              If you want to change a value, please do so in SSL_Conf.h,
              do NOT modify this file.

*/

#ifndef SSL_CONFDEFAULTS_H
#define SSL_CONFDEFAULTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SSL_Conf.h"
#include "SEGGER.h"
#include "SEGGER_UTIL.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Define SSL_DEBUG: Debug level for SSL product
//                  0: No checks                      (Smallest and fastest code)
//                  1: Warnings & Panic checks
//                  2: Warnings, logs, & panic checks (Seriously bigger code)
//
#ifndef   SSL_DEBUG
  #define SSL_DEBUG                       0
#endif

//
// Log buffer related configuration defaults.
//
#ifndef   SSL_LOG_BUFFER_SIZE
  #define SSL_LOG_BUFFER_SIZE             160
#endif

//
// Maximum supported length of a session ticket
//
#ifndef   SSL_MAX_SESSION_TICKET_LEN
  #define SSL_MAX_SESSION_TICKET_LEN      256
#endif

//
// Maximum internal fragment length of an Application Data protocol
// packet.  If the app requests a send of more than this amount,
// it will be fragmented across multiple Application Data packets.
//
#ifndef   SSL_MAX_APP_DATA_FRAGMENT_LEN
  #define SSL_MAX_APP_DATA_FRAGMENT_LEN   1024
#endif

#ifndef   SSL_SESSION_CACHE_SIZE
  #define SSL_SESSION_CACHE_SIZE          0
#endif

#ifndef   SSL_OS_DISABLE_INTERRUPT
  #define SSL_OS_DISABLE_INTERRUPT()      SSL_OS_DisableInterrupt()
#endif

#ifndef   SSL_OS_ENABLE_INTERRUPT
  #define SSL_OS_ENABLE_INTERRUPT()       SSL_OS_EnableInterrupt()
#endif

#ifndef   SSL_OS_GET_TIME
  #define SSL_OS_GET_TIME()               SSL_OS_GetTime32()
#endif

#ifndef   SSL_OS_UNLOCK
  #define SSL_OS_UNLOCK()                 SSL_OS_Unlock()
#endif

#ifndef   SSL_OS_LOCK
  #define SSL_OS_LOCK()                   SSL_OS_Lock()
#endif

#ifndef   SSL_MEMCPY
  #define SSL_MEMCPY                      memcpy
#endif

#ifndef   SSL_MEMSET
  #define SSL_MEMSET                      memset
#endif

#ifndef   SSL_MEMMOVE
  #define SSL_MEMMOVE                     memmove
#endif

#ifndef   SSL_MEMCMP
  #define SSL_MEMCMP                      memcmp
#endif

#ifndef   SSL_STRCPY
  #define SSL_STRCPY                      strcpy
#endif

#ifndef   SSL_STRLEN
  #define SSL_STRLEN                      strlen
#endif

#ifndef   SSL_STRCAT
  #define SSL_STRCAT                      strcat
#endif

#ifndef   SSL_SNPRINTF
  #define SSL_SNPRINTF                    snprintf
#endif

#ifndef   SSL_RDU16BE
  #define SSL_RDU16BE                     SEGGER_RdU16BE
#endif

#ifndef   SSL_WRU16BE
  #define SSL_WRU16BE                     SEGGER_WrU16BE
#endif

#ifndef   SSL_WRU24BE
  #define SSL_WRU24BE                     SEGGER_WrU24BE
#endif

#ifndef   SSL_WRU32BE
  #define SSL_WRU32BE                     SEGGER_WrU32BE
#endif

#ifndef   SSL_USE_PARA                                // Some compiler complain about unused parameters.
  #define SSL_USE_PARA(Para)              (void)Para  // This works for most compilers.
#endif

#if SSL_DEBUG >= 2
  #define SSL_LOG(s)                      SSL_Logf s
  #define SSL_LOG_ERR(ERROR)              ((SSL_LOG((SSL_LOG_ERROR, "SSL: Raising \"%s\" (%d)", SSL_ERROR_GetText(ERROR), ERROR)), SSL_LOG((SSL_LOG_ERROR, "     | At " __FILE__ ":%d", __LINE__))), ERROR)
#else
  #define SSL_LOG(s)
  #define SSL_LOG_ERR(ERROR)              (ERROR)
#endif

#if SSL_DEBUG >= 1
  #define SSL_WARN(s)                     SSL_Warnf s
#else
  #define SSL_WARN(s)
#endif

#if SSL_DEBUG > 0
  #define SSL_ASSERT(X)                   do { if (!(X)) { SSL_PANIC(SSL_ERROR_ASSERT_FAILED); } } while (0)
  #define SSL_PANIC(s)                    SSL_Panic(s)
#else
  #define SSL_ASSERT(X)
  #define SSL_PANIC(s)
#endif

#ifndef   SSL_MAX_NUM_CONNECTIONS
  #define SSL_MAX_NUM_CONNECTIONS         0   // 0: Dynamic number; > 0: Fixed number
#endif

#ifndef   SSL_SUPPORT_PROFILE
  #define SSL_SUPPORT_PROFILE             0
#endif

#if SSL_MAX_APP_DATA_FRAGMENT_LEN < 1 || 16384 < SSL_MAX_APP_DATA_FRAGMENT_LEN
  #error Bad setting for SSL_MAX_APP_DATA_FRAGMENT_LEN
#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
