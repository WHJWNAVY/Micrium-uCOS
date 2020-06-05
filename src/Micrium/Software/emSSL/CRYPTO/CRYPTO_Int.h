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

File        : CRYPTO_Int.h
Purpose     : Private, internal structures and functions that support
              operation of the crypto toolkit.
Notes       : This file is subject to change, and is not guaranteed
              to be portable from release to release.

*/

#ifndef CRYPTO_INT_H
#define CRYPTO_INT_H

/*********************************************************************
*
*       Settings section
*
**********************************************************************
*/

/* For Visual C and -Wall, disable specific compiler warnings. */
#ifdef _MSC_VER
#pragma warning(disable:4295 4711)
#endif

/*********************************************************************
*
*       Lint configuration
*
**********************************************************************
*/

/*lint -emacro(835,__MPI_LITERAL_DATA)              */
/*lint -emacro(845,__MPI_LITERAL_DATA)              */
/*lint -emacro(835,__MPI_LITERAL_END)               */
/*lint -emacro(845,__MPI_LITERAL_END)               */
/*lint -esym(534,CRYPTO_MEMSHL_BE)                  */
/*lint -esym(534,CRYPTO_MEMSHL_LE)                  */
/*lint -esym(534,CRYPTO_MEMSHR_LE)                  */
/*lint -sem(CRYPTO_X_Panic,r_no) this never returns */

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "CRYPTO.h"

// This is required for the MMCAU workaround
#if defined (__IAR_SYSTEMS_ICC__)
  #include <intrinsics.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#if CRYPTO_API_CHECKS
  #define CRYPTO_ASSERT(X)               { if (!(X)) { CRYPTO_PANIC(); } }  // I know this is low-rent
#else
  #define CRYPTO_ASSERT(X)
#endif

#define CRYPTO_CHECK(X)           /*lint -e{717,801,9036} */ do { if ((Status = (X)) < 0) goto Finally; } while (0)
#define CRYPTO_GIVEN(C, X)        /*lint -e{717,801,9036} */ do { if (!(C)) { Status = X; goto Finally; } } while (0)
#define CRYPTO_RAISE(X)           /*lint -e{717,801}      */ do { Status = (X); goto Finally; } while (0)

// Functions that don't exist but must be able to be added to an API
#define  CRYPTO_GMAC_AES_Init                       NULL
#define  CRYPTO_GMAC_ARIA_Init                      NULL
#define  CRYPTO_GMAC_SEED_Init                      NULL
#define  CRYPTO_GMAC_CAMELLIA_Init                  NULL
#define  CRYPTO_GMAC_TWOFISH_Init                   NULL

// Functions that don't exist but must be able to be added to an API
#define  CRYPTO_MAC_GMAC_AES_Init                   NULL
#define  CRYPTO_MAC_GMAC_ARIA_Init                  NULL
#define  CRYPTO_MAC_GMAC_SEED_Init                  NULL
#define  CRYPTO_MAC_GMAC_CAMELLIA_Init              NULL
#define  CRYPTO_MAC_GMAC_TWOFISH_Init               NULL

// Functions that don't exist but must be able to be added to an API
#define  CRYPTO_MAC_POLY1305_AES_Init               NULL
#define  CRYPTO_MAC_POLY1305_ARIA_Init              NULL
#define  CRYPTO_MAC_POLY1305_SEED_Init              NULL
#define  CRYPTO_MAC_POLY1305_CAMELLIA_Init          NULL
#define  CRYPTO_MAC_POLY1305_TWOFISH_Init           NULL

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define CRYPTO_OS_INTERNAL_UNIT_RNG     0            // Unit number for internal RNG

#define CRYPTO_TDES_KEY_SIZE           21            // 168 bits, required by DRBG-TDEA

/*********************************************************************
*
*       Mersenne prime modulus reduction.
*/

#if defined(U64) && defined(I64) && (CRYPTO_MPI_BITS_PER_LIMB == 32)

#define CRYPTO_PRIME_REDUCE_MERSENNE_DECL_VARS \
  I64 Sum;

#define CRYPTO_PRIME_REDUCE_MERSENNE_BEGIN0(I) \
  Sum = pSelf->aLimbs[I];

#define CRYPTO_PRIME_REDUCE_MERSENNE_BEGIN1(I) \
  Sum >>= 32; /*Assumes arithmetic shift*/ \
  Sum += pSelf->aLimbs[I]

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADD(I) \
  Sum += pSelf->aLimbs[I]

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADD2(I) \
  Sum += pSelf->aLimbs[I]; \
  Sum += pSelf->aLimbs[I]

#define CRYPTO_PRIME_REDUCE_MERSENNE_SUB(I) \
  Sum -= pSelf->aLimbs[I];

#define CRYPTO_PRIME_REDUCE_MERSENNE_END(I) \
  pSelf->aLimbs[I] = (U32)Sum;

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADJUST(I) \
  Sum >>= 32; /*Assumes arithmetic shift*/ \
  if (Sum < 0) { \
    CRYPTO_MPI M; \
    CRYPTO_MPI_FAST_End(pSelf, (I)*32); \
    CRYPTO_MPI_Init(&M, pMem); \
    CRYPTO_CHECK(CRYPTO_MPI_FAST_Begin(&M, ((I)+1)*32)); \
    CRYPTO_MPI_FAST_SetU32(&M, (I), (U32)(-Sum)); \
    CRYPTO_MPI_FAST_End(&M, ((I)+1)*32); \
    CRYPTO_CHECK(CRYPTO_MPI_Sub(pSelf, &M)); \
    CRYPTO_MPI_Kill(&M); \
  } else { \
    pSelf->aLimbs[I] = (U32)Sum; \
    CRYPTO_MPI_FAST_End(pSelf, ((I)+1)*32); \
  }

#else

#define CRYPTO_PRIME_REDUCE_MERSENNE_DECL_VARS \
  U32 Sum; \
  U32 X; \
  I32 C;

#define CRYPTO_PRIME_REDUCE_MERSENNE_BEGIN0(I) \
  Sum = CRYPTO_MPI_FAST_GetU32(pSelf, I); C = 0;

#define CRYPTO_PRIME_REDUCE_MERSENNE_BEGIN1(I) \
  Sum = CRYPTO_MPI_FAST_GetU32(pSelf, I); \
  if (C < 0) { \
    C = -C; \
    X = (U32)C; C = Sum < (U32)C ? -1 : 0; Sum -= X; \
  } else { \
    Sum += C; C = Sum < (U32)C ? 1 : 0; \
  }

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADD(I) \
  Sum += X = CRYPTO_MPI_FAST_GetU32(pSelf, I); C += Sum < X ? 1 : 0;

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADD2(I) \
  CRYPTO_PRIME_REDUCE_MERSENNE_ADD(I); CRYPTO_PRIME_REDUCE_MERSENNE_ADD(I)

#define CRYPTO_PRIME_REDUCE_MERSENNE_SUB(I) \
  X = CRYPTO_MPI_FAST_GetU32(pSelf, I); C -= Sum < X ? 1 : 0; Sum -= X;

#define CRYPTO_PRIME_REDUCE_MERSENNE_END(I) \
  CRYPTO_MPI_FAST_SetU32(pSelf, I, Sum);

#define CRYPTO_PRIME_REDUCE_MERSENNE_ADJUST(I) \
  if (C < 0) { \
    CRYPTO_MPI M; \
    CRYPTO_MPI_FAST_End(pSelf, (I)*32); \
    CRYPTO_MPI_Init(&M, pMem); \
    CRYPTO_CHECK(CRYPTO_MPI_FAST_Begin(&M, ((I)+1)*32)); \
    CRYPTO_MPI_FAST_SetU32(&M, (I), (U32)(-C)); \
    CRYPTO_MPI_FAST_End(&M, ((I)+1)*32); \
    CRYPTO_CHECK(CRYPTO_MPI_Sub(pSelf, &M)); \
    CRYPTO_MPI_Kill(&M); \
  } else { \
    CRYPTO_MPI_FAST_SetU32(pSelf, (I), (U32)C); \
    CRYPTO_MPI_FAST_End(pSelf, ((I)+1)*32); \
  }
#endif


/*********************************************************************
*
*       Local data types
*
**********************************************************************
*/

/*********************************************************************
*
*       GCM and CCM blocks.
*/
typedef U8 CRYPTO_GCM_BLOCK[CRYPTO_GCM_BLOCK_SIZE];
typedef U8 CRYPTO_CCM_BLOCK[CRYPTO_CCM_BLOCK_SIZE];

/*********************************************************************
*
*       ModExp API signatures.
*/
typedef int (*CRYPTO_MODEXP_INIT_FUNC)(CRYPTO_MPI *pSelf,       CRYPTO_MPI *pAccum,            CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
typedef int (*CRYPTO_MODEXP_EXIT_FUNC)(CRYPTO_MPI *pSelf,                                      CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
typedef int (*CRYPTO_MODEXP_MUL_FUNC) (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pMultiplier, const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
typedef int (*CRYPTO_MODEXP_SQR_FUNC) (CRYPTO_MPI *pSelf,                                const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);

typedef struct {
  U8      * pSeed;
  unsigned  SeedLen;
  unsigned  GenCounter;
} SHAWE_TAYLOR_RANDOM_PRIME_STATE;

typedef struct {
  CRYPTO_MODEXP_INIT_FUNC pfInit;
  CRYPTO_MODEXP_EXIT_FUNC pfExit;
  CRYPTO_MODEXP_MUL_FUNC  pfMul;
  CRYPTO_MODEXP_SQR_FUNC  pfSqr;
} CRYPTO_MPI_MODEXP_API;

typedef U8 CRYPTO_XTS_BLOCK[16];

/*********************************************************************
*
*       Public const data
*
**********************************************************************
*/

extern const U8  CRYPTO__aZeroBits              [128];
extern const U8  CRYPTO__U8_0x00                [1];
extern const U8  CRYPTO__U8_0x01                [1];
extern const U8  CRYPTO__U8_0x02                [1];
extern const U8  CRYPTO__U8_0x03                [1];
extern const U8  CRYPTO__U8_0x80                [1];
extern const U8  CRYPTO__U8_aOnesFromLeft       [9];
extern const U8  CRYPTO__aDigestInfo_SHA1       [15];
extern const U8  CRYPTO__aDigestInfo_SHA224     [19];
extern const U8  CRYPTO__aDigestInfo_SHA256     [19];
extern const U8  CRYPTO__aDigestInfo_SHA384     [19];
extern const U8  CRYPTO__aDigestInfo_SHA512     [19];
extern const U8  CRYPTO__aDigestInfo_SHA512_224 [19];
extern const U8  CRYPTO__aDigestInfo_SHA512_256 [19];
extern const U8  CRYPTO__aDigestInfo_SHA3_224   [19];
extern const U8  CRYPTO__aDigestInfo_SHA3_256   [19];
extern const U8  CRYPTO__aDigestInfo_SHA3_384   [19];
extern const U8  CRYPTO__aDigestInfo_SHA3_512   [19];
extern const U8  CRYPTO_DRBG_CTR_aKey           [32];
extern const U8  CRYPTO_KW_aICV1                [8];
extern const U8  CRYPTO_KW_aICV2                [4];
extern const U32 CRYPTO_SHA224_aIV              [CRYPTO_SHA256_DIGEST_UNIT_COUNT];
extern const U32 CRYPTO_SHA256_aIV              [CRYPTO_SHA256_DIGEST_UNIT_COUNT];

extern const CRYPTO_MPI CRYPTO_MPI_One;
extern const CRYPTO_MPI CRYPTO_MPI_Two;
extern const CRYPTO_MPI CRYPTO_MPI_Three;
extern const CRYPTO_MPI CRYPTO_MPI_Four;
extern const CRYPTO_MPI CRYPTO_MPI_Five;
extern const CRYPTO_MPI CRYPTO_MPI_Seven;

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

extern const CRYPTO_RNG_API        * CRYPTO_RNG_pSecureAPI;           // Typically a software DRBG
extern const CRYPTO_RNG_API        * CRYPTO_RNG_pEntropyAPI;          // Typically a hardware entropy source
extern const CRYPTO_MPI_MODEXP_API   CRYPTO_MPI_MODEXP_EFM32_CRYPTO;  // Modular exponentiation using EFM32 CRYPTO coprocessor
//
extern unsigned                      CRYPTO_MPI_LimbAllocUnit;

extern CRYPTO_MPI_MODEXP_FUNC CRYPTO_MPI_ModExp_Public;
extern CRYPTO_MPI_MODEXP_FUNC CRYPTO_MPI_ModExp_Private;


/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

U32          CRYPTO_GetLowBitMask(unsigned Count);

void         CRYPTO_HASH_BUFFER_Init                    (CRYPTO_HASH_BUFFER *pSelf, U8 *pBuffer);
const U8 *   CRYPTO_HASH_BUFFER_Wr                      (CRYPTO_HASH_BUFFER *pSelf, const U8 **pInput, unsigned *pInputLen, unsigned ChunkSize);
const U8 *   CRYPTO_HASH_BUFFER_WrDelay                 (CRYPTO_HASH_BUFFER *pSelf, const U8 **pInput, unsigned *pInputLen, unsigned ChunkSize);
unsigned     CRYPTO_MEMSHL_LE                           (U8 *pData, unsigned DataLen, unsigned Bits);
unsigned     CRYPTO_MEMSHR_LE                           (U8 *pData, unsigned DataLen, unsigned Bits);
unsigned     CRYPTO_MEMSHL_BE                           (U8 *pData, unsigned DataLen, unsigned Bits);

/*********************************************************************
*
*       Abstract algorithms for modular exponentiation.
*/
int      CRYPTO_MPI_ModExp_Fast                         (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, const CRYPTO_MPI_MODEXP_API *pMethods, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Ladder                       (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, const CRYPTO_MPI_MODEXP_API *pMethods, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_RM                           (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, unsigned K, CRYPTO_MPI *pX, const CRYPTO_MPI_MODEXP_API *pMethods, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_FW                           (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, unsigned K, CRYPTO_MPI *pX, const CRYPTO_MPI_MODEXP_API *pMethods, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Internal MPI representation modification.  Required for
*       hardware-assisted MPI functions outside of the MPI kernel.
*/
int      CRYPTO_MPI_Stretch                             (CRYPTO_MPI *pSelf, unsigned LimbCnt);
void     CRYPTO_MPI_Trim                                (CRYPTO_MPI *pSelf);
int      CRYPTO_MPI_Mul_Comba_Partial_HW_EFM32_CRYPTO   (CRYPTO_MPI *pSelf, CRYPTO_MPI *pMultiplier, unsigned Cutoff, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Binary_HW_EFM32_CRYPTO       (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_2b_RM_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_3b_RM_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_4b_RM_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_5b_RM_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_6b_RM_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_2b_FW_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_3b_FW_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_4b_FW_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_5b_FW_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_ModExp_Montgomery_6b_FW_EFM32_CRYPTO(CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Internal point multiplication functions
*/
int      CRYPTO_EC_Mul_FW                               (CRYPTO_EC_POINT *pSelf, const CRYPTO_EC_CURVE *pCurve, const CRYPTO_MPI *pMultiplier, unsigned K, CRYPTO_EC_POINT *pX, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Mul_RM                               (CRYPTO_EC_POINT *pSelf, const CRYPTO_EC_CURVE *pCurve, const CRYPTO_MPI *pMultiplier, unsigned K, CRYPTO_EC_POINT *pX, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Mul_wNAF                             (CRYPTO_EC_POINT *pSelf, const CRYPTO_EC_CURVE *pCurve, const CRYPTO_MPI *pMultiplier, unsigned K, CRYPTO_EC_POINT *pX, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Internal encryption functions, required for KATs with known seed
*/
int      CRYPTO_RSAES_OAEP_KDF1_SHA1_EncryptEx          (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA224_EncryptEx        (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA256_EncryptEx        (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA384_EncryptEx        (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA512_EncryptEx        (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA512_224_EncryptEx    (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA512_256_EncryptEx    (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA3_224_EncryptEx      (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA3_256_EncryptEx      (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA3_384_EncryptEx      (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSAES_OAEP_KDF1_SHA3_512_EncryptEx      (const CRYPTO_RSA_PUBLIC_KEY *pSelf, U8 *pOutput, unsigned OutputLen, const U8 *pInput, unsigned InputLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Internal signing functions
*/
int      CRYPTO_RSA_EMSA_PSS_SHA1_Encode                (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA224_Encode              (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA256_Encode              (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA384_Encode              (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_Encode              (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_224_Encode          (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_256_Encode          (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_224_Encode            (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_256_Encode            (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_384_Encode            (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_512_Encode            (const U8 *pMessageHash, const U8 *pSalt, unsigned SaltLen, U8 *pEncodedMessage, unsigned EncodedMessageLen, unsigned EncodedMessageBitCnt, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA1_Verify                (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA224_Verify              (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA256_Verify              (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA384_Verify              (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_Verify              (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_224_Verify          (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_256_Verify          (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_224_Verify            (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_256_Verify            (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_384_Verify            (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_512_Verify            (const U8 *pMessage, unsigned MessageLen, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT * pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA1_VerifyDigest          (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA224_VerifyDigest        (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA256_VerifyDigest        (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA384_VerifyDigest        (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_VerifyDigest        (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_224_VerifyDigest    (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA512_256_VerifyDigest    (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_224_VerifyDigest      (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_256_VerifyDigest      (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_384_VerifyDigest      (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_RSA_EMSA_PSS_SHA3_512_VerifyDigest      (const U8 *pMessageHash, const U8 *pEncodedMessage, unsigned EncodedMessageLen, U8 *pSalt, unsigned SaltLen, unsigned EncodedMessageBitCnt, U8 *pWorking, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Internal FIPS186 prime generation support
*/
int      CRYPTO_FIPS186_SHA1_GenRandom                  (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA224_GenRandom                (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA256_GenRandom                (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA384_GenRandom                (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_GenRandom                (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_224_GenRandom            (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_256_GenRandom            (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_224_GenRandom              (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_256_GenRandom              (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_384_GenRandom              (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_512_GenRandom              (CRYPTO_MPI *pOutput, unsigned N,        U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
//
int      CRYPTO_FIPS186_SHA1_GenSTRandomPrime           (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA224_GenSTRandomPrime         (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA256_GenSTRandomPrime         (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA384_GenSTRandomPrime         (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_GenSTRandomPrime         (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_224_GenSTRandomPrime     (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA512_256_GenSTRandomPrime     (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_224_GenSTRandomPrime       (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_256_GenSTRandomPrime       (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_384_GenSTRandomPrime       (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_FIPS186_SHA3_512_GenSTRandomPrime       (CRYPTO_MPI *pPrime,  unsigned PrimeLen, U8 *pSeed, unsigned SeedLen, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Deterministic DSA support.
*/
int      CRYPTO_DSA_RFC6979_SHA1_GenK                   (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA224_GenK                 (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA256_GenK                 (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA384_GenK                 (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA512_GenK                 (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA512_224_GenK             (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA512_256_GenK             (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA3_224_GenK               (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA3_256_GenK               (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA3_384_GenK               (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_DSA_RFC6979_SHA3_512_GenK               (const CRYPTO_DSA_DOMAIN_PARAMS *pPara, const CRYPTO_DSA_PRIVATE_KEY *pKey, const U8 *pMessage, unsigned MessageLen, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Deterministic ECDSA support.
*/
int      CRYPTO_ECDSA_RFC6979_SHA1_GenK                 (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA224_GenK               (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA256_GenK               (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA384_GenK               (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA512_GenK               (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA512_224_GenK           (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA512_256_GenK           (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA3_224_GenK             (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA3_256_GenK             (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA3_384_GenK             (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_ECDSA_RFC6979_SHA3_512_GenK             (const CRYPTO_EC_CURVE *pCurve, const CRYPTO_ECDSA_PRIVATE_KEY *pKey, const U8 *pDigest, CRYPTO_MPI *pK, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Functions generated by the template but not exposed, prototypes required to eliminate warning.
*/
void     CRYPTO_CIPHER_SEED_192_InitEncrypt             (void *pSelf, const U8 *pKey);
void     CRYPTO_CIPHER_SEED_192_InitDecrypt             (void *pSelf, const U8 *pKey);
void     CRYPTO_CIPHER_SEED_256_InitEncrypt             (void *pSelf, const U8 *pKey);
void     CRYPTO_CIPHER_SEED_256_InitDecrypt             (void *pSelf, const U8 *pKey);

/*********************************************************************
*
*       Fast access to MPIs.
*/
int      CRYPTO_MPI_FAST_Begin                          (CRYPTO_MPI *pSelf, unsigned BitCnt);
void     CRYPTO_MPI_FAST_End                            (CRYPTO_MPI *pSelf, unsigned BitCnt);
U32      CRYPTO_MPI_FAST_GetU32                         (const CRYPTO_MPI *pSelf, unsigned Index);
U64      CRYPTO_MPI_FAST_GetU64                         (const CRYPTO_MPI *pSelf, unsigned Index);
void     CRYPTO_MPI_FAST_SetU32                         (CRYPTO_MPI *pSelf, unsigned Index, U32 Value);
void     CRYPTO_MPI_FAST_SetU64                         (CRYPTO_MPI *pSelf, unsigned Index, U64 Value);

/*********************************************************************
*
*       Low-level methods conforming to MODEXP API.
*/
int      CRYPTO_MPI_MODEXP_Basic_Init                   (CRYPTO_MPI *pSelf,       CRYPTO_MPI *pAccum,            CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Basic_Exit                   (CRYPTO_MPI *pSelf,                                      CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Basic_Mul                    (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pMultiplier, const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Basic_Square                 (CRYPTO_MPI *pSelf,                                const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Montgomery_Init              (CRYPTO_MPI *pSelf,       CRYPTO_MPI *pAccum,            CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Montgomery_Exit              (CRYPTO_MPI *pSelf,                                      CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Montgomery_Mul               (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pMultiplier, const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Montgomery_Square            (CRYPTO_MPI *pSelf,                                const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Barrett_Init                 (CRYPTO_MPI *pSelf,       CRYPTO_MPI *pAccum,            CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Barrett_Exit                 (CRYPTO_MPI *pSelf,                                      CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Barrett_Mul                  (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pMultiplier, const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_MODEXP_Barrett_Square               (CRYPTO_MPI *pSelf,                                const CRYPTO_MPI *pAux, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_MPI_Reduce                              (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pModulus, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Key and parameter I/O.
*/
void     CRYPTO_IO_SEGGER_WrHeaderComment               (CRYPTO_BUFFER *pBuffer, const char *sTitle);
void     CRYPTO_IO_SEGGER_WrFooterComment               (CRYPTO_BUFFER *pBuffer);
void     CRYPTO_IO_SEGGER_WrPara                        (CRYPTO_BUFFER *pBuffer, const char *sName, const CRYPTO_MPI *pValue);
int      CRYPTO_IO_SEGGER_RdMPI                         (CRYPTO_MPI *pValue, CRYPTO_TLV *pTLV);

/*********************************************************************
*
*       Curve25519 field arithmetic.
*/
int      CRYPTO_EC_Curve25519_Reduce                    (CRYPTO_MPI *pSelf, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModAdd                    (CRYPTO_MPI *pX, const CRYPTO_MPI *pY);
int      CRYPTO_EC_Curve25519_ModAddEx                  (CRYPTO_MPI *pSum, const CRYPTO_MPI *pAddend0, const CRYPTO_MPI *pAddend1);
int      CRYPTO_EC_Curve25519_ModSubEx                  (CRYPTO_MPI *pSum, const CRYPTO_MPI *pAddend0, const CRYPTO_MPI *pAddend1);
int      CRYPTO_EC_Curve25519_ModMul                    (CRYPTO_MPI *pX, const CRYPTO_MPI *pY, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModMulEx                  (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand0, const CRYPTO_MPI *pMultiplicand1, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModMulU32Ex               (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand0, U32 Multiplicand1, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModSquare                 (CRYPTO_MPI *pX, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModSquareEx               (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModExp                    (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModExpEx                  (CRYPTO_MPI *pResult, const CRYPTO_MPI *pBase, const CRYPTO_MPI *pExponent, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve25519_ModInv                    (CRYPTO_MPI *pSelf, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Curve448 field arithmetic.
*/
int      CRYPTO_EC_Curve448_Reduce                      (CRYPTO_MPI *pSelf, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModAdd                      (CRYPTO_MPI *pX, const CRYPTO_MPI *pY);
int      CRYPTO_EC_Curve448_ModAddEx                    (CRYPTO_MPI *pSum, const CRYPTO_MPI *pAddend0, const CRYPTO_MPI *pAddend1);
int      CRYPTO_EC_Curve448_ModSub                      (CRYPTO_MPI *pX, const CRYPTO_MPI *pY);
int      CRYPTO_EC_Curve448_ModSubEx                    (CRYPTO_MPI *pSum, const CRYPTO_MPI *pAddend0, const CRYPTO_MPI *pAddend1);
int      CRYPTO_EC_Curve448_ModMul                      (CRYPTO_MPI *pX, const CRYPTO_MPI *pY, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModMulEx                    (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand0, const CRYPTO_MPI *pMultiplicand1, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModMulU32Ex                 (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand0, U32 Multiplicand1, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModSquare                   (CRYPTO_MPI *pX, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModSquareEx                 (CRYPTO_MPI *pProduct, const CRYPTO_MPI *pMultiplicand, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModExp                      (CRYPTO_MPI *pSelf, const CRYPTO_MPI *pExponent, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModExpEx                    (CRYPTO_MPI *pResult, const CRYPTO_MPI *pBase, const CRYPTO_MPI *pExponent, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_EC_Curve448_ModInv                      (CRYPTO_MPI *pSelf, CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Ed25519 shared support.
*/
int      CRYPTO_Ed25519_MakeAffine                      (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_MakeProjective                  (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_Projective_Mul                  (CRYPTO_EC_POINT *pSelf, const CRYPTO_MPI *pK,   CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_Projective_Add                  (CRYPTO_EC_POINT *pP, const CRYPTO_EC_POINT *pQ, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_Projective_Double               (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_EncodePoint                     (CRYPTO_EC_POINT *pPoint, CRYPTO_MPI *pY,        CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_DecodePoint                     (const CRYPTO_MPI *pY, CRYPTO_EC_POINT *pPoint,  CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed25519_ExpandPrivateKey                (CRYPTO_EdDSA_PRIVATE_KEY *pPrivateKey,          CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Ed448 shared support.
*/
int      CRYPTO_Ed448_MakeAffine                        (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_MakeProjective                    (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_Projective_Mul                    (CRYPTO_EC_POINT *pSelf, const CRYPTO_MPI *pK,   CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_Projective_Add                    (CRYPTO_EC_POINT *pP, const CRYPTO_EC_POINT *pQ, CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_Projective_Double                 (CRYPTO_EC_POINT *pP,                            CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_EncodePoint                       (CRYPTO_EC_POINT *pPoint, CRYPTO_MPI *pY,        CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_DecodePoint                       (const CRYPTO_MPI *pY, CRYPTO_EC_POINT *pPoint,  CRYPTO_MEM_CONTEXT *pMem);
int      CRYPTO_Ed448_ExpandPrivateKey                  (CRYPTO_EdDSA_PRIVATE_KEY *pPrivateKey,          CRYPTO_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       CMS.
*/
int      CRYPTO_CMS_RdINTEGER_Small                     (CRYPTO_TLV *pSelf, unsigned Tag, long *pValue);
int      CRYPTO_TLV_MatchTag                            (CRYPTO_TLV *pSelf, unsigned ExpectedTag);

/*********************************************************************
*
*       OID decoding.
*/
int      CRYPTO_OID_DecodeAlgorithm                     (CRYPTO_X509_PK_ALGORITHM_ID *pAlgorithm, const CRYPTO_TLV *pOID);
int      CRYPTO_X509_ParseBitstring                     (CRYPTO_TLV *pSelf, CRYPTO_TLV *pBitstring, unsigned MaxUnusedBitCnt);

/*********************************************************************
*
*       Internal API selection functions
*/
const CRYPTO_HASH_API   *CRYPTO_HASH_SelectAPI          (const CRYPTO_HASH_API *pPrefer, const CRYPTO_HASH_API *pFallback);
const CRYPTO_CIPHER_API *CRYPTO_CIPHER_SelectAPI        (const CRYPTO_CIPHER_API *pPrefer, const CRYPTO_CIPHER_API *pFallback, unsigned KeySize);

/*********************************************************************
*
*       Curve25519 and Ed25519 constants.
*
*/
extern const CRYPTO_MPI      CRYPTO_EC_Curve25519_Q;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed25519_D;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed25519_L;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed25519_I;
extern const CRYPTO_EC_POINT CRYPTO_EdDSA_Ed25519_B;
extern const U8              CRYPTO_EdDSA_Ed25519_aSigEd25519[32];

/*********************************************************************
*
*       Curve448 and Ed448 constants.
*
*/
extern const CRYPTO_MPI      CRYPTO_EC_Curve448_Q;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed448_D;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed448_L;
extern const CRYPTO_MPI      CRYPTO_EdDSA_Ed448_I;
extern const CRYPTO_EC_POINT CRYPTO_EdDSA_Ed448_B;
extern const U8              CRYPTO_EdDSA_Ed448_aSigEd448[8];

/*********************************************************************
*
*       Internal hardware accelerator addresses
*/
extern void *CRYPTO_HW_STM32_CRYP;
extern void *CRYPTO_HW_STM32_RNG;
extern void *CRYPTO_HW_STM32_HASH;
extern void *CRYPTO_HW_STM32_AES;
extern void *CRYPTO_HW_EFM32_CRYPTO;
extern void *CRYPTO_HW_NXP_CAU;
extern void *CRYPTO_HW_NXP_RNGA;

/*********************************************************************
*
*       EFM32 CRYPTO power-on/off
*/
void CRYPTO_EFM32_X_PowerOn  (void);
void CRYPTO_EFM32_X_PowerOff (void);

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
