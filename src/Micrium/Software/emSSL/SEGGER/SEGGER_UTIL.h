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

File        : SEGGER_UTIL.c
Purpose     : Utility functions related to multi-byte load and store.
Revision    : $Rev: 12387 $
*/

#ifndef SEGGER_UTIL_H
#define SEGGER_UTIL_H

#include "SEGGER.h"

// We have three options for byte order configuration
// -1: Little endian and misaligned loads/stores never fault
//  0: Endian neutral or misaligned loads/stores may fault
// +1: Big endian and misaligned loads/stores never fault

//
// Unless otherwise directed, use endian-neutral model when analyzing with C-STAT
//
#if !defined(SEGGER_UTIL_CONFIG_BYTE_ORDER) && defined(__CSTAT__)
  #define SEGGER_UTIL_CONFIG_BYTE_ORDER 0
#endif

#ifndef   SEGGER_UTIL_CONFIG_BYTE_ORDER
  #if defined(_WIN32) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM7M__) || defined(__ARM7EM__)
    #define SEGGER_UTIL_CONFIG_BYTE_ORDER           -1
  #else
    #define SEGGER_UTIL_CONFIG_BYTE_ORDER            0
  #endif
#endif

#ifndef   SEGGER_UTIL_CONFIG_BYTE_ORDER_U16
  #define SEGGER_UTIL_CONFIG_BYTE_ORDER_U16        SEGGER_UTIL_CONFIG_BYTE_ORDER
#endif

#ifndef   SEGGER_UTIL_CONFIG_BYTE_ORDER_U32
  #define SEGGER_UTIL_CONFIG_BYTE_ORDER_U32        SEGGER_UTIL_CONFIG_BYTE_ORDER
#endif

#ifndef   SEGGER_UTIL_CONFIG_BYTE_ORDER_U64
  #if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM7M__) || defined(__ARM7EM__)
    #define SEGGER_UTIL_CONFIG_BYTE_ORDER_U64      0  /*64-bit LDRD faults on misaligned addresses*/
  #else
    #define SEGGER_UTIL_CONFIG_BYTE_ORDER_U64      SEGGER_UTIL_CONFIG_BYTE_ORDER
  #endif
#endif

#ifdef __CSTAT__

U16  SEGGER_RdU16LE(const U8 aData[2]);
U16  SEGGER_RdU16BE(const U8 aData[2]);
void SEGGER_WrU16LE(U8 aData[2], U16 Value);
void SEGGER_WrU16BE(U8 aData[2], U16 Value);
U32  SEGGER_RdU24LE(const U8 aData[3]);
U32  SEGGER_RdU24BE(const U8 aData[3]);
void SEGGER_WrU24LE(U8 aData[], U32 Value);
void SEGGER_WrU24BE(U8 aData[], U32 Value);
U32  SEGGER_RdU32LE(const U8 aData[4]);
U32  SEGGER_RdU32BE(const U8 aData[4]);
void SEGGER_WrU32LE(U8 aData[4], U32 Value);
void SEGGER_WrU32BE(U8 aData[4], U32 Value);
#ifdef U64
U64  SEGGER_RdU64LE(const U8 aData[8]);
U64  SEGGER_RdU64BE(const U8 aData[8]);
void SEGGER_WrU64LE(U8 aData[8], U64 Value);
void SEGGER_WrU64BE(U8 aData[8], U64 Value);
#endif

#else

static INLINE void SEGGER_WrU24LE(U8 aData[], U32 Value) {
  aData[0] = (U8)(Value);
  aData[1] = (U8)(Value >> 8);
  aData[2] = (U8)(Value >> 16);
}

static INLINE void SEGGER_WrU24BE(U8 aData[], U32 Value) {
  aData[2] = (U8)(Value);
  aData[1] = (U8)(Value >> 8);
  aData[0] = (U8)(Value >> 16);
}

static INLINE U32 SEGGER_RdU24LE(const U8 aData[3]) {
  return aData[0] + (U32)aData[1] * 0x100u + (U32)aData[2] * 0x10000u;
}

static INLINE U32 SEGGER_RdU24BE(const U8 aData[3]) {
  return aData[2] + (U32)aData[1]*0x100u + (U32)aData[0]*0x10000u;
}

//
// Utilize builtins for byte swaps if we have them.
//
#if defined(__GNUC__)
  #define SEGGER_BSWAP_U16             __builtin_bswap16
  #define SEGGER_BSWAP_U32             __builtin_bswap32
  #define SEGGER_BSWAP_U64             __builtin_bswap64
#endif

#if defined(__ICCARM__)
  #include <intrinsics.h>
  #define SEGGER_BSWAP_U16             __REV16
  #define SEGGER_BSWAP_U32             __REV
#endif

static INLINE U16 SEGGER_RdU16LE(const U8 aData[2]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 > 0 && defined(SEGGER_BSWAP_U16)
  return SEGGER_BSWAP_U16(*(U16*)(aData))
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 < 0
  return *(U16*)(aData);
#else
  return aData[1]*0x100U + aData[0];
#endif
}

static INLINE void SEGGER_WrU16LE(U8 aData[2], U16 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 > 0 && defined(SEGGER_BSWAP_U16)
  *(U16*)(aData) = SEGGER_BSWAP_U16(Value);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 < 0
  *(U16*)(aData) = Value;
#else
  aData[0] = (U8)(Value);
  aData[1] = (U8)(Value >> 8);
#endif
}

static INLINE U32 SEGGER_RdU32LE(const U8 aData[4]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 > 0 && defined(SEGGER_BSWAP_U32)
  return SEGGER_BSWAP_U32(*(const volatile U32 *)aData);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 < 0
  return *(volatile U32*)(aData);
#else
  return  (U32)aData[0] +
         ((U32)aData[1] <<  8) +
         ((U32)aData[2] << 16) +
         ((U32)aData[3] << 24);
#endif
}

static INLINE void SEGGER_WrU32LE(U8 aData[4], U32 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 > 0 && defined(SEGGER_BSWAP_U32)
  *(volatile U32*)(aData) = SEGGER_BSWAP_U32(Value);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 < 0
  *(volatile U32*)(aData) = Value;
#else
  aData[0] = (U8)(Value);
  aData[1] = (U8)(Value >> 8);
  aData[2] = (U8)(Value >> 16);
  aData[3] = (U8)(Value >> 24);
#endif
}

#ifdef U64
static INLINE U64 SEGGER_RdU64LE(const U8 aData[8]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 > 0 && defined(SEGGER_BSWAP_U64)
  return SEGGER_BSWAP_U64(*(const U64 *)aData);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 < 0
  return *(volatile U64*)(aData);
#else
  return ((U64)SEGGER_RdU32LE(&aData[4]) << 32) | SEGGER_RdU32LE(&aData[0]);
#endif
}

static INLINE void SEGGER_WrU64LE(U8 aData[8], U64 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 > 0 && defined(SEGGER_BSWAP_U64)
  *(volatile U64*)(aData) = SEGGER_BSWAP_U64(Value);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 < 0
  *(volatile U64*)(aData) = Value;
#else
  SEGGER_WrU32LE(&aData[4], (U32)(Value >> 32));
  SEGGER_WrU32LE(&aData[0], (U32)(Value      ));
#endif
}
#endif

static INLINE U16 SEGGER_RdU16BE(const U8 aData[2]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 < 0 && defined(SEGGER_BSWAP_U16)
  return SEGGER_BSWAP_U16(*(U16*)(aData));
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 > 0
  return *(U16*)(aData);
#else
  return aData[0]*0x100U + aData[1];
#endif
}

static INLINE void SEGGER_WrU16BE(U8 aData[2], U16 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 < 0 && defined(SEGGER_BSWAP_U16)
  *(U16*)(aData) = SEGGER_BSWAP_U16(Value);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U16 > 0
  *(U16*)(aData) = Value;
#else
  aData[0] = (U8)(Value >> 8);
  aData[1] = (U8)(Value);
#endif
}

static INLINE U32 SEGGER_RdU32BE(const U8 aData[4]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 < 0 && defined(SEGGER_BSWAP_U32)
  return SEGGER_BSWAP_U32(*(const volatile U32 *)aData);  /* I use volatile here to ensure that clang does not collapse two consecutive ldr instructions into an ldrd */
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 > 0
  return *(U32*)(aData);
#else
  return  (U32)aData[3] +
         ((U32)aData[2] <<  8) +
         ((U32)aData[1] << 16) +
         ((U32)aData[0] << 24);
#endif
}

static INLINE void SEGGER_WrU32BE(U8 aData[4], U32 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 < 0 && defined(SEGGER_BSWAP_U32)
  *(volatile U32*)(aData) = SEGGER_BSWAP_U32(Value);    /* I use volatile here to ensure that clang does not collapse two consecutive ldr instructions into an ldrd */
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U32 > 0
  *(U32*)(aData) = Value;
#else
  aData[0] = (U8)(Value >> 24);
  aData[1] = (U8)(Value >> 16);
  aData[2] = (U8)(Value >> 8);
  aData[3] = (U8)(Value);
#endif
}

#ifdef U64
static INLINE U64 SEGGER_RdU64BE(const U8 aData[8]) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 < 0 && defined(SEGGER_BSWAP_U64)
  return SEGGER_BSWAP_U64(*(const U64 *)aData);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 > 0
  return *(volatile U64*)(aData);
#else
  return ((U64)SEGGER_RdU32BE(&aData[0]) << 32) | SEGGER_RdU32BE(&aData[4]);
#endif
}

static INLINE void SEGGER_WrU64BE(U8 aData[8], U64 Value) {
#if SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 < 0 && defined(SEGGER_BSWAP_U64)
  *(volatile U64*)(aData) = SEGGER_BSWAP_U64(Value);
#elif SEGGER_UTIL_CONFIG_BYTE_ORDER_U64 > 0
  *(volatile U64*)(aData) = Value;
#else
  SEGGER_WrU32BE(&aData[0], (U32)(Value >> 32));
  SEGGER_WrU32BE(&aData[4], (U32)(Value      ));
#endif
}
#endif

/*********************************************************************
*
*       SEGGER_GetPtrDistance()
*
*  Function description
*    Returns the distance between two pointers
*    which is the absolute value of the subtraction of two pointers.
*
*  Parameters
*    p0: First pointer
*    p1: Second pointer
*
*  Return value
*    Distance between two pointers, used e.g. as NumBytes in a buffer.
*
*  Notes
*    (1) Distance is always U32, although 64bit pointers might be
*        more far away.
*/
static INLINE U32 SEGGER_GetPtrDistance(const void* p0, const void* p1) {
#ifdef I64
  I64 Distance;
#else
  I32 Distance;
#endif

#ifdef I64
  Distance = (I64)((U8*)p0-(U8*)p1);  //lint !e9016 Pointer arithmetic.
#else
  Distance = (I32)((U8*)p0-(U8*)p1);  //lint !e9016 Pointer arithmetic.
#endif
  if (Distance < 0) {
    Distance *= (-1);
  }
  return (U32)Distance;
}
#endif

#endif

/*************************** End of file ****************************/
