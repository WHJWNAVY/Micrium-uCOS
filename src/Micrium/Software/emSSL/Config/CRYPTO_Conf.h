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
File        : CRYPTO_Conf.h
Purpose     : SEGGER Cryptography library configuration
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef CRYPTO_CONF_H
#define CRYPTO_CONF_H

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#ifndef   CRYPTO_NL
#ifdef WIN32
  #define CRYPTO_NL          "\r\n"
#else
  #define CRYPTO_NL          "\n"
#endif
#endif

/*********************************************************************
*
*       CRYPTO_MPI_BITS_PER_LIMB
*
*  Description
*    Configure number of bits per limb for emSecure algorithm
*    calculations. Default: 32
*    Supported configurations are:
*    - 32, which requires the target compiler to support 64-bit
*      types natively (i.e. unsigned long long or unsigned __int64),
*    - 16, which should run on any ISO compiler whose native integer
*      types are 16 or 32 bit and support 32-bit unsigned long.
*    - 8, 8-bit limb sizes are supported and selecting this size may
*      well lead to better multiplication performance on 8-bit
*      architectures--you would need to run emBenchmark on your
*      target and compare encryption and decryption speeds for 8-bit
*      and 16-bit limb sizes to pick an optimal configuration.
*/
#ifndef   CRYPTO_MPI_BITS_PER_LIMB
  #define CRYPTO_MPI_BITS_PER_LIMB     32
#endif

#ifndef     CRYPTO_SIZEOF_INT
  #ifdef __SIZEOF_INT__
    #define CRYPTO_SIZEOF_INT __SIZEOF_INT__
  #else
    #define CRYPTO_SIZEOF_INT (CRYPTO_MPI_BITS_PER_LIMB / 8)
  #endif
#endif


/*********************************************************************
*
*       CRYPTO_CONFIG_xxx_OPTIMIZE
*
*  Description
*    Set zero to optimize the module functions for size rather
*    than for speed.  When optimized for speed, the functions are
*    gradually open-coded and/or table0driven, and are significantly
*    larger.
*/
#if defined(SEGGER_BUILD_MINSIZE) && defined(SEGGER_BUILD_MAXSPEED)
  #error Cannot compile with both SEGGER_BUILD_MINSIZE and SEGGER_BUILD_MAXSPEED defined
#endif

#if defined(SEGGER_BUILD_MINSIZE)
  #ifndef   CRYPTO_CONFIG_MD5_OPTIMIZE
    #define CRYPTO_CONFIG_MD5_OPTIMIZE        0
  #endif
  #ifndef   CRYPTO_CONFIG_MD5_HW_OPTIMIZE
    #define CRYPTO_CONFIG_MD5_HW_OPTIMIZE     0
  #endif
  #ifndef   CRYPTO_CONFIG_RIPEMD160_OPTIMIZE
    #define CRYPTO_CONFIG_RIPEMD160_OPTIMIZE  0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA1_OPTIMIZE
    #define CRYPTO_CONFIG_SHA1_OPTIMIZE       0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA1_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA1_HW_OPTIMIZE    0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA256_OPTIMIZE
    #define CRYPTO_CONFIG_SHA256_OPTIMIZE     0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA256_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA256_HW_OPTIMIZE  0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA512_OPTIMIZE
    #define CRYPTO_CONFIG_SHA512_OPTIMIZE     0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA512_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA512_HW_OPTIMIZE  0
  #endif
  #ifndef   CRYPTO_CONFIG_SHA3_OPTIMIZE
    #define CRYPTO_CONFIG_SHA3_OPTIMIZE       0
  #endif
  #ifndef   CRYPTO_CONFIG_DES_OPTIMIZE
    #define CRYPTO_CONFIG_DES_OPTIMIZE        0
  #endif
  #ifndef   CRYPTO_CONFIG_AES_OPTIMIZE
    #define CRYPTO_CONFIG_AES_OPTIMIZE        0
  #endif
  #ifndef   CRYPTO_CONFIG_AES_HW_OPTIMIZE
    #define CRYPTO_CONFIG_AES_HW_OPTIMIZE     0
  #endif
  #ifndef   CRYPTO_CONFIG_SEED_OPTIMIZE
    #define CRYPTO_CONFIG_SEED_OPTIMIZE       0
  #endif
  #ifndef   CRYPTO_CONFIG_ARIA_OPTIMIZE
    #define CRYPTO_CONFIG_ARIA_OPTIMIZE       0
  #endif
  #ifndef   CRYPTO_CONFIG_CAMELLIA_OPTIMIZE
    #define CRYPTO_CONFIG_CAMELLIA_OPTIMIZE   0
  #endif
  #ifndef   CRYPTO_CONFIG_CAST_OPTIMIZE
    #define CRYPTO_CONFIG_CAST_OPTIMIZE       0
  #endif
  #ifndef   CRYPTO_CONFIG_BLOWFISH_OPTIMIZE
    #define CRYPTO_CONFIG_BLOWFISH_OPTIMIZE   0
  #endif
  #ifndef   CRYPTO_CONFIG_TWOFISH_OPTIMIZE
    #define CRYPTO_CONFIG_TWOFISH_OPTIMIZE    0
  #endif
  #ifndef   CRYPTO_CONFIG_GCM_OPTIMIZE
    #define CRYPTO_CONFIG_GCM_OPTIMIZE        0
  #endif
  #ifndef   CRYPTO_CONFIG_OPTIMIZE_TABLES
    #define CRYPTO_CONFIG_OPTIMIZE_TABLES     0    /* Prefer not to use tables, reduce code size */
  #endif
  #ifndef   CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY
    #define CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY 0
  #endif

#elif defined(SEGGER_BUILD_MAXSPEED)
  #ifndef   CRYPTO_CONFIG_MD5_OPTIMIZE
    #define CRYPTO_CONFIG_MD5_OPTIMIZE        1
  #endif
  #ifndef   CRYPTO_CONFIG_MD5_HW_OPTIMIZE
    #define CRYPTO_CONFIG_MD5_HW_OPTIMIZE     1
  #endif
  #ifndef   CRYPTO_CONFIG_RIPEMD160_OPTIMIZE
    #define CRYPTO_CONFIG_RIPEMD160_OPTIMIZE  1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA1_OPTIMIZE
    #define CRYPTO_CONFIG_SHA1_OPTIMIZE       1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA1_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA1_HW_OPTIMIZE    1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA256_OPTIMIZE
    #define CRYPTO_CONFIG_SHA256_OPTIMIZE     1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA256_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA256_HW_OPTIMIZE  1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA512_OPTIMIZE
    #define CRYPTO_CONFIG_SHA512_OPTIMIZE     2
  #endif
  #ifndef   CRYPTO_CONFIG_SHA512_HW_OPTIMIZE
    #define CRYPTO_CONFIG_SHA512_HW_OPTIMIZE  1
  #endif
  #ifndef   CRYPTO_CONFIG_SHA3_OPTIMIZE
    #define CRYPTO_CONFIG_SHA3_OPTIMIZE       1
  #endif
  #ifndef   CRYPTO_CONFIG_DES_OPTIMIZE
    #define CRYPTO_CONFIG_DES_OPTIMIZE        5
  #endif
  #ifndef   CRYPTO_CONFIG_AES_OPTIMIZE
    #define CRYPTO_CONFIG_AES_OPTIMIZE        7  /* Tables in RAM, GF multiply LUTs */
  #endif
  #ifndef   CRYPTO_CONFIG_AES_HW_OPTIMIZE
    #define CRYPTO_CONFIG_AES_HW_OPTIMIZE     1
  #endif
  #ifndef   CRYPTO_CONFIG_SEED_OPTIMIZE
    #define CRYPTO_CONFIG_SEED_OPTIMIZE       3  /* Tables in RAM, optimized SBox */
  #endif
  #ifndef   CRYPTO_CONFIG_ARIA_OPTIMIZE
    #define CRYPTO_CONFIG_ARIA_OPTIMIZE       1  /* Tables in RAM */
  #endif
  #ifndef   CRYPTO_CONFIG_CAMELLIA_OPTIMIZE
    #define CRYPTO_CONFIG_CAMELLIA_OPTIMIZE   3  /* Tables in RAM, optimized SBox */
  #endif
  #ifndef   CRYPTO_CONFIG_CAST_OPTIMIZE
    #define CRYPTO_CONFIG_CAST_OPTIMIZE       1  /* Tables in RAM */
  #endif
  #ifndef   CRYPTO_CONFIG_BLOWFISH_OPTIMIZE
    #define CRYPTO_CONFIG_BLOWFISH_OPTIMIZE   1  /* Tables in RAM */
  #endif
  #ifndef   CRYPTO_CONFIG_TWOFISH_OPTIMIZE
    #define CRYPTO_CONFIG_TWOFISH_OPTIMIZE   15  /* Tables in RAM, MDS LUT, RS LUT, full keying */
  #endif
  #ifndef   CRYPTO_CONFIG_GCM_OPTIMIZE
    #ifdef _WIN32
      #define CRYPTO_CONFIG_GCM_OPTIMIZE      1
    #else
      #define CRYPTO_CONFIG_GCM_OPTIMIZE      0
    #endif
  #endif

  #ifndef   CRYPTO_CONFIG_OPTIMIZE_TABLES
    #define CRYPTO_CONFIG_OPTIMIZE_TABLES     1  /* Prefer to use tables, maximize code speed at expense of code size */
  #endif
  #ifndef   CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY
    #define CRYPTO_CONFIG_ECDSA_TWIN_MULTIPLY 1
  #endif
#endif

/*********************************************************************
*
*       CRYPTO_NOP
*
*  Description
*    Insert a NOP into the code.  This is required for some
*    crypto coprocessor workarounds.
*/
#if defined(__GNUC__) || defined(__clang__) || defined (__CC_ARM)
  #define CRYPTO_NOP() __asm volatile("nop")
#elif defined (__IAR_SYSTEMS_ICC__)
  #include <intrinsics.h>
  #define CRYPTO_NOP() __no_operation()
#elif defined(WIN32)
  #define CRYPTO_NOP()
#elif defined(__CCRX__)
  #define CRYPTO_NOP() asm("NOP")
#else
  #error CRYPTO: Unknown compiler, dont know how to define CRYPTO_NOP()
#endif

/*********************************************************************
*
*       CRYPTO_API_CHECKS
*
*  Description
*    Configure whether the crypto implementation checks API calls.
*    In general, this ensures that MPIs that are written to
*    must indeed be writable and not in ROM.
*
*    Setting CRYPTO_API_CHECKS nonzero, typically to 1, will enable
*    parameter checks at the API level and we strongly suggest that
*    you set this to one when developing.  Setting CRYPTO_API_CHECKS
*    to zero disables all API parameter checks and assumes that
*    clients will only pass valid parameters to exposed API
*    functions.  If you violate this condition, the behavior of
*    the crypto library is undefined.
*/
#ifndef CRYPTO_API_CHECKS
  #ifdef DEBUG
    #define CRYPTO_API_CHECKS    DEBUG
  #else
    #define CRYPTO_API_CHECKS    0
  #endif
#endif

#endif

/*************************** End of file ****************************/
