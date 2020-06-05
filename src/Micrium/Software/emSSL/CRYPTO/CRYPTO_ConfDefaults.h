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

File        : CRYPTO_ConfDefaults.h
Purpose     : Configure CRYPTO module routines.

*/

#ifndef CRYPTO_CONFDEFAULTS_H
#define CRYPTO_CONFDEFAULTS_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "CRYPTO_Conf.h"
#include "SEGGER.h"
#include "SEGGER_UTIL.h"
#include "SEGGER_MEM.h"
#include <string.h>  // to get memxxx functions

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Multiprecision integers (MPI)
*/

#ifndef CRYPTO_MPI_BITS_PER_LIMB
  #define CRYPTO_MPI_BITS_PER_LIMB            32
#endif

#ifndef     CRYPTO_SIZEOF_INT
  #ifdef __SIZEOF_INT__
    #define CRYPTO_SIZEOF_INT __SIZEOF_INT__
  #elif defined WIN32
    #define CRYPTO_SIZEOF_INT 4
  #else
    #define CRYPTO_SIZEOF_INT (CRYPTO_MPI_BITS_PER_LIMB / 8)
  #endif
#endif

/*********************************************************************
*
*       Cipher optimizations
*/

#ifndef   CRYPTO_CONFIG_CAMELLIA_OPTIMIZE
  #define CRYPTO_CONFIG_CAMELLIA_OPTIMIZE  0
#endif

#ifndef   CRYPTO_CONFIG_DES_OPTIMIZE
  #define CRYPTO_CONFIG_DES_OPTIMIZE       0
#endif

#ifndef   CRYPTO_CONFIG_AES_OPTIMIZE
  #define CRYPTO_CONFIG_AES_OPTIMIZE       2
#endif
#ifndef   CRYPTO_CONFIG_AES_HW_OPTIMIZE
  #define CRYPTO_CONFIG_AES_HW_OPTIMIZE    0
#endif

#ifndef   CRYPTO_CONFIG_ARIA_OPTIMIZE
  #define CRYPTO_CONFIG_ARIA_OPTIMIZE      0
#endif

#ifndef   CRYPTO_CONFIG_SEED_OPTIMIZE
  #define CRYPTO_CONFIG_SEED_OPTIMIZE      0
#endif

#ifndef   CRYPTO_CONFIG_CAST_OPTIMIZE
  #define CRYPTO_CONFIG_CAST_OPTIMIZE      0
#endif

#ifndef   CRYPTO_CONFIG_BLOWFISH_OPTIMIZE
  #define CRYPTO_CONFIG_BLOWFISH_OPTIMIZE  0
#endif

#ifndef   CRYPTO_CONFIG_TWOFISH_OPTIMIZE
  #define CRYPTO_CONFIG_TWOFISH_OPTIMIZE   1
#endif

#ifndef   CRYPTO_CONFIG_SHA1_OPTIMIZE
  #define CRYPTO_CONFIG_SHA1_OPTIMIZE      0
#endif
#ifndef   CRYPTO_CONFIG_SHA1_HW_OPTIMIZE
  #define CRYPTO_CONFIG_SHA1_HW_OPTIMIZE   0
#endif

#ifndef   CRYPTO_CONFIG_SHA256_OPTIMIZE
  #define CRYPTO_CONFIG_SHA256_OPTIMIZE    0
#endif
#ifndef   CRYPTO_CONFIG_SHA256_HW_OPTIMIZE
  #define CRYPTO_CONFIG_SHA256_HW_OPTIMIZE 0
#endif

#ifndef   CRYPTO_CONFIG_SHA512_OPTIMIZE
  #define CRYPTO_CONFIG_SHA512_OPTIMIZE    0
#endif

#ifndef   CRYPTO_CONFIG_SHA3_OPTIMIZE
  #define CRYPTO_CONFIG_SHA3_OPTIMIZE      0
#endif

#ifndef   CRYPTO_CONFIG_MD5_OPTIMIZE
  #define CRYPTO_CONFIG_MD5_OPTIMIZE       0
#endif
#ifndef   CRYPTO_CONFIG_MD5_HW_OPTIMIZE
  #define CRYPTO_CONFIG_MD5_HW_OPTIMIZE    0
#endif

#ifndef   CRYPTO_CONFIG_RIPEMD160_OPTIMIZE   
  #define CRYPTO_CONFIG_RIPEMD160_OPTIMIZE 0
#endif

#ifndef   CRYPTO_CONFIG_GCM_OPTIMIZE
  #define CRYPTO_CONFIG_GCM_OPTIMIZE  0
#endif

#ifndef   CRYPTO_CONFIG_OPTIMIZE_TABLES
  #define CRYPTO_CONFIG_OPTIMIZE_TABLES    1    /* Prefer to use tables */
#endif

// Avoid problems in Microsoft headers when linting.
#ifdef _lint
  #define CRYPTO_CONFIG_AES_AESNI          0
#endif

#ifndef   CRYPTO_CONFIG_AES_AESNI
  #if defined(_WIN32) && defined(_MSC_VER) && _MSC_VER >= 1900
    #define CRYPTO_CONFIG_AES_AESNI        1
  #else
    #define CRYPTO_CONFIG_AES_AESNI        0
  #endif
#endif

#if CRYPTO_CONFIG_AES_AESNI > 0 && defined(_WIN32)
  #include <wmmintrin.h>
#endif

/*********************************************************************
*
*       CSPRNG configuration
*/

#ifndef   CRYPTO_CONFIG_FORTUNA_POOL_MAX
  #define CRYPTO_CONFIG_FORTUNA_POOL_MAX            32   // Number of pools for entropy acquisition [4, 32].
#endif

#ifndef   CRYPTO_CONFIG_FORTUNA_SOURCE_MAX
  #define CRYPTO_CONFIG_FORTUNA_SOURCE_MAX           2  // Maximum number of entropy sources [1, 255].
#endif

#ifndef   CRYPTO_CONFIG_FORTUNA_RESEED_MAX
  #define CRYPTO_CONFIG_FORTUNA_RESEED_MAX        (1024u*1024)  // Maximum number of generated pseudorandom data bytes between reseed events.
#endif

#ifndef   CRYPTO_CONFIG_FORTUNA_GET_MAX
  #define CRYPTO_CONFIG_FORTUNA_GET_MAX           (1024u*1024)  // Maximum number of bytes that s single "get" request can provide
#endif

/*********************************************************************
*
*       ECDSA verification configuration
*/
#ifndef   CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY
  #define CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY       1            // Use twin multiply-accumulate (Shamir's Trick) in ECDSA signature vertification
#endif

/*********************************************************************
*
*       Global CRYPTO configuration
*/

#ifndef   CRYPTO_NL
  #define CRYPTO_NL          "\r\n"
#endif

#ifndef   CRYPTO_API_CHECKS
  #define CRYPTO_API_CHECKS    1
#endif

#ifndef   CRYPTO_USE_PARA                     // Some compiler complain about unused parameters.
  #define CRYPTO_USE_PARA(Para)   (void)Para  // This works for most compilers.
#endif

#ifndef   CRYPTO_MEMZAP
  #define CRYPTO_MEMZAP     CRYPTO_memzap
#endif

#ifndef   CRYPTO_MEMDIF
  #define CRYPTO_MEMDIF     CRYPTO_memdif
#endif

#ifndef   CRYPTO_MEMCPY
  #define CRYPTO_MEMCPY     memcpy
#endif

#ifndef   CRYPTO_MEMSET
  #define CRYPTO_MEMSET     memset
#endif

#ifndef   CRYPTO_MEMMOVE
  #define CRYPTO_MEMMOVE    memmove
#endif

#ifndef   CRYPTO_MEMCMP
  #define CRYPTO_MEMCMP     memcmp
#endif

#ifndef   CRYPTO_STRLEN
#define   CRYPTO_STRLEN     strlen
#endif

#ifndef   CRYPTO_STRCHR
#define   CRYPTO_STRCHR     strchr
#endif

#ifndef   CRYPTO_STRSTR
#define   CRYPTO_STRSTR     strstr
#endif

#ifndef   CRYPTO_STRNCMP
#define   CRYPTO_STRNCMP    strncmp
#endif

#ifndef   CRYPTO_MEMXOR
  #define CRYPTO_MEMXOR     SEGGER_memxor
#endif

#ifndef   CRYPTO_RDU16BE
  #define CRYPTO_RDU16BE    SEGGER_RdU16BE
#endif

#ifndef   CRYPTO_WRU16BE
  #define CRYPTO_WRU16BE    SEGGER_WrU16BE
#endif

#ifndef   CRYPTO_RDU24BE
  #define CRYPTO_RDU24BE    SEGGER_RdU24BE
#endif

#ifndef   CRYPTO_WRU24BE
  #define CRYPTO_WRU24BE    SEGGER_WrU24BE
#endif

#ifndef   CRYPTO_RDU32BE
  #define CRYPTO_RDU32BE    SEGGER_RdU32BE
#endif

#ifndef   CRYPTO_WRU32BE
  #define CRYPTO_WRU32BE    SEGGER_WrU32BE
#endif

#ifndef   CRYPTO_RDU64BE
  #define CRYPTO_RDU64BE    SEGGER_RdU64BE
#endif

#ifndef   CRYPTO_WRU64BE
  #define CRYPTO_WRU64BE    SEGGER_WrU64BE
#endif

#ifndef   CRYPTO_RDU16LE
  #define CRYPTO_RDU16LE    SEGGER_RdU16LE
#endif

#ifndef   CRYPTO_WRU16LE
  #define CRYPTO_WRU16LE    SEGGER_WrU16LE
#endif

#ifndef   CRYPTO_RDU24LE
  #define CRYPTO_RDU24LE    SEGGER_RdU24LE
#endif

#ifndef   CRYPTO_WRU24LE
  #define CRYPTO_WRU24LE    SEGGER_WrU24LE
#endif

#ifndef   CRYPTO_RDU32LE
  #define CRYPTO_RDU32LE    SEGGER_RdU32LE
#endif

#ifndef   CRYPTO_WRU32LE
  #define CRYPTO_WRU32LE    SEGGER_WrU32LE
#endif

#ifndef   CRYPTO_RDU64LE
  #define CRYPTO_RDU64LE    SEGGER_RdU64LE
#endif

#ifndef   CRYPTO_WRU64LE
  #define CRYPTO_WRU64LE    SEGGER_WrU64LE
#endif

#ifndef   CRYPTO_ROL_U32
  #define CRYPTO_ROL_U32(X, N)   ( ((U32)(X) << (N)) | ((U32)(X) >> (32-(N))) )
#endif

#ifndef   CRYPTO_ROR_U32
  #define CRYPTO_ROR_U32(X, N)   ( ((U32)(X) >> (N)) | ((U32)(X) << (32-(N))) )
#endif

#ifndef   CRYPTO_ROL_U64
  #define CRYPTO_ROL_U64(X, N)   ( ((U64)(X) << (N)) | ((U64)(X) >> (64-(N))) )
#endif

#ifndef   CRYPTO_MEM_CONTEXT
  #define CRYPTO_MEM_CONTEXT     SEGGER_MEM_CONTEXT
#endif

#ifndef   CRYPTO_PANIC
  #define CRYPTO_PANIC()         CRYPTO_X_Panic()
#endif


#endif

/*************************** End of file ****************************/
