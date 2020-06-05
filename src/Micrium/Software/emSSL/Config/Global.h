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
File    : Global.h
Purpose : Global types
          In case your application already has a Global.h, you should
          merge the files. In order to use Segger code, the types
          U8, U16, U32, I8, I16, I32 need to be defined in Global.h;
          additional definitions do not hurt.
Revision: $Rev: 12372 $
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H

#define U8    unsigned char
#define I8    signed char
#define U16   unsigned short
#define I16   signed short
#ifdef __x86_64__
#define U32   unsigned
#define I32   int
#else
#define U32   unsigned long
#define I32   signed long
#endif

//
// CC_NO_LONG_SUPPORT can be defined to compile test
// without long support for compilers that do not
// support C99 and its long type.
//
#ifdef CC_NO_LONG_SUPPORT
  #define PTR_ADDR  U32
#else  // Supports long type.
#if defined(_WIN32) && !defined(__clang__) && !defined(__MINGW32__)
  //
  // Microsoft VC6 compiler related
  //
  #define U64   unsigned __int64
  #define U128  unsigned __int128
  #define I64   __int64
  #define I128  __int128
  #if _MSC_VER <= 1200
    #define U64_C(x) x##UI64
  #else
    #define U64_C(x) x##ULL
  #endif
#else
  //
  // C99 compliant compiler
  //
  #define U64   unsigned long long
  #define I64   signed long long
  #define U64_C(x) x##ULL
#endif

#if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
  #define PTR_ADDR  U64
#else
  #define PTR_ADDR  U32
#endif
#endif  // Supports long type.

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
