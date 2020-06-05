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

File        : SSL_Conf.h
Purpose     : Configuration file for configurable defines in SSL module

*/

#ifndef SSL_CONF_H
#define SSL_CONF_H

//
// Define SSL_DEBUG: Debug level for SSL product
//                  0: No checks                      (Smallest and fastest code)
//                  1: Warnings & Panic checks
//                  2: Warnings, logs, & panic checks (Seriously bigger code)
//
#if defined(DEBUG) && (DEBUG > 0)
  #ifndef   SSL_DEBUG
    #define SSL_DEBUG      2      // Default for debug builds
  #endif
#else
  #ifndef   SSL_DEBUG
    #define SSL_DEBUG      0      // Default for release builds
  #endif
#endif

//
// Configure maximum session ticket length.
//
#ifndef   SSL_MAX_SESSION_TICKET_LEN
  #define SSL_MAX_SESSION_TICKET_LEN      256
#endif

//
// Configure profiling support.
//
#if defined(SUPPORT_PROFILE) && SUPPORT_PROFILE
  #ifndef   SSL_SUPPORT_PROFILE
    #define SSL_SUPPORT_PROFILE           1                   // Define as 1 to enable profiling via SystemView.
  #endif
#endif

//
// Inline OS function calls for higher performance in release builds
//
#if !defined(SSL_OS_DO_NOT_INLINE_CALLS) && !defined(WIN32) && !defined(__linux__)
  #if SSL_DEBUG > 0
    #include "RTOS.h"
    extern OS_RSEMA SSL_OS_RSema;
    #define SSL_OS_DISABLE_INTERRUPT()   OS_DI()
    #define SSL_OS_ENABLE_INTERRUPT()    OS_RestoreI()
    #define SSL_OS_GET_TIME()            OS_GetTime32()
    #define SSL_OS_LOCK()                OS_Use(&SSL_OS_RSema)
    #define SSL_OS_UNLOCK()              OS_Unuse(&SSL_OS_RSema)
  #endif
#endif

#endif

/*
*********************************************************************************************************
*********************************************************************************************************
*                   MICRIUM uC/LIB MEMORY AND STRING MANIPULATION API REDEFINITION
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_mem.h>

#ifdef   SSL_MEMCPY
#undef   SSL_MEMCPY
#endif
#define  SSL_MEMCPY                           Mem_Copy


#ifdef   SSL_MEMSET
#undef   SSL_MEMSET
#endif
#define  SSL_MEMSET                           Mem_Set


#if 0                                                           /* uC/LIB's Mem_Cmp() incompatible with POSIX memcmp(). */
#ifdef   SSL_MEMCMP
#undef   SSL_MEMCMP
#endif
#define  SSL_MEMCMP                           Mem_Cmp
#endif


#ifdef   SSL_MEMMOVE
#undef   SSL_MEMMOVE
#endif
#define  SSL_MEMMOVE                          Mem_Move


#ifdef   SSL_STRCPY
#undef   SSL_STRCPY
#endif
#define  SSL_STRCPY                           Str_Copy


#ifdef   SSL_STRLEN
#undef   SSL_STRLEN
#endif
#define  SSL_STRLEN                           Str_Len


#ifdef   SSL_STRCAT
#undef   SSL_STRCAT
#endif
#define  SSL_STRCAT                           Str_Cat


#ifdef   SSL_SNPRINTF
#undef   SSL_SNPRINTF
#endif
#define  SSL_SNPRINTF                         Str_FmtPrint
    

/*************************** End of file ****************************/
