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

File        : SSL_Int.h
Purpose     : Internal configuration setup.

*/

#ifndef SSL_INT_H
#define SSL_INT_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SSL.h"
#include "SSL_ConfDefaults.h"
#include "CRYPTO_ConfDefaults.h"
#include "SEGGER_UTIL.h"
#include "SEGGER_MEM.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Lint configuration
*
**********************************************************************
*/

/*lint -esym(534,memcpy)  */
/*lint -esym(534,memset)  */
/*lint -esym(534,memmove) */
/*lint -esym(534,strcpy)  */
/*lint -esym(534,strcat)  */

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define SSL_PRF_MD5_HANDSHAKE_SIZE     CRYPTO_SHA1_DIGEST_BYTE_COUNT
#define SSL_PRF_SHA_HANDSHAKE_SIZE     CRYPTO_SHA1_DIGEST_BYTE_COUNT
#define SSL_PRF_SHA256_HANDSHAKE_SIZE  CRYPTO_SHA256_DIGEST_BYTE_COUNT
#define SSL_PRF_SHA384_HANDSHAKE_SIZE  CRYPTO_SHA384_DIGEST_BYTE_COUNT

#define SSL_ALLOC(LVALUE, SIZE)      /*lint -e{717} */     do { if (((LVALUE) = SSL_MEM_Alloc(SIZE)) == 0 && (SIZE) > 0)     { (void)SSL_LOG_ERR(SSL_ERROR_OUT_OF_MEMORY); SSL_RAISE(SSL_ERROR_OUT_OF_MEMORY); } } while (0)
#define SSL_ZEROALLOC(LVALUE, SIZE)  /*lint -e{717} */     do { if (((LVALUE) = SSL_MEM_ZeroAlloc(SIZE)) == 0 && (SIZE) > 0) { (void)SSL_LOG_ERR(SSL_ERROR_OUT_OF_MEMORY); SSL_RAISE(SSL_ERROR_OUT_OF_MEMORY); } } while (0)
#define SSL_CHECK_ALLOC(X)           /*lint -e{717} */     do { if ((X) == 0)                                                { (void)SSL_LOG_ERR(SSL_ERROR_OUT_OF_MEMORY); SSL_RAISE(SSL_ERROR_OUT_OF_MEMORY); } } while (0)
#define SSL_CHECK(X)                 /*lint -e{717,801} */ do { if ((Status = (X)) < 0)  { goto Finally; } } while (0)
#define SSL_RAISE(X)                 /*lint -e{717,801} */ do { Status = (X); goto Finally; } while (0)

#define SSL_PROFILE_GET_EVENT_ID(ID)  ((ID) + SSL_PROFILE_IdOffset)

/*********************************************************************
*
*       SSL_PROFILE_CALL_VOID()
*/
#if SSL_SUPPORT_PROFILE
  #define SSL_PROFILE_CALL(EventId, Session)                                              \
    if (SSL_PROFILE_pAPI) {                                                               \
      SSL_PROFILE_pAPI->pfRecordU32(SSL_PROFILE_GET_EVENT_ID(EventId), (U32)(Session));   \
    }
#else
  #define SSL_PROFILE_CALL(EventId, Session)
#endif

/*********************************************************************
*
*       SSL_PROFILE_CALL_U32()
*/
#if SSL_SUPPORT_PROFILE
  #define SSL_PROFILE_CALL_U32(EventId, Session, Para0)                                                  \
    if (SSL_PROFILE_pAPI) {                                                                              \
      SSL_PROFILE_pAPI->pfRecordU32x2(SSL_PROFILE_GET_EVENT_ID(EventId), (U32)(Session), (U32)(Para0));  \
    }
#else
  #define SSL_PROFILE_CALL_U32(EventId, Session, Para0)
#endif

/*********************************************************************
*
*       SSL_PROFILE_CALL_U32x2()
*/
#if SSL_SUPPORT_PROFILE
  #define SSL_PROFILE_CALL_U32x2(EventId, Session, Para0, Para1)                                                         \
    if (SSL_PROFILE_pAPI) {                                                                                              \
      SSL_PROFILE_pAPI->pfRecordU32x3(SSL_PROFILE_GET_EVENT_ID((EventId)), (U32)(Session), (U32)(Para0), (U32)(Para1));  \
    }
#else
  #define SSL_PROFILE_CALL_U32x2(Id, Session, Para0, Para1)
#endif

/*********************************************************************
*
*       SSL_PROFILE_END_CALL()
*/
#if SSL_SUPPORT_PROFILE
  #define SSL_PROFILE_END_CALL(EventId)                                      \
    if (SSL_PROFILE_pAPI) {                                                  \
      SSL_PROFILE_pAPI->pfRecordEndCall(SSL_PROFILE_GET_EVENT_ID(EventId));  \
    }
#else
  #define SSL_PROFILE_END_CALL(EventId)
#endif

/*********************************************************************
*
*       SSL_PROFILE_END_CALL_U32()
*/
#if SSL_SUPPORT_PROFILE
  #define SSL_PROFILE_END_CALL_U32(EventId, Para)                                          \
    if (SSL_PROFILE_pAPI) {                                                                \
      SSL_PROFILE_pAPI->pfRecordEndCallU32(SSL_PROFILE_GET_EVENT_ID(EventId), (U32)Para);  \
    }
#else
  #define SSL_PROFILE_END_CALL_U32(EventId, Para)
#endif

/*********************************************************************
*
*       Profile event identifiers
*/
enum {
  //
  // Events for IP API functions (IDs 0-249).
  //
  SSL_EVTID_NETSEND,
  SSL_EVTID_NETRECV,
  SSL_EVTID_ENCRYPT,
  SSL_EVTID_DECRYPT,
  SSL_EVTID_AGREED_CIPHER_SUITE,
  SSL_EVTID_SEND_CLIENT_HELLO,
  SSL_EVTID_RECV_CLIENT_HELLO,
  SSL_EVTID_SEND_SERVER_HELLO,
  SSL_EVTID_RECV_SERVER_HELLO,
  SSL_EVTID_SEND_SERVER_KEY_EXCHANGE,
  SSL_EVTID_RECV_SERVER_KEY_EXCHANGE,
  SSL_EVTID_SEND_CERTIFICATE_REQUEST,
  SSL_EVTID_RECV_CERTIFICATE_REQUEST,
  SSL_EVTID_SEND_SERVER_HELLO_DONE,
  SSL_EVTID_RECV_SERVER_HELLO_DONE,
  SSL_EVTID_SEND_CERTIFICATE,
  SSL_EVTID_RECV_CERTIFICATE,
  SSL_EVTID_SEND_CLIENT_KEY_EXCHANGE,
  SSL_EVTID_RECV_CLIENT_KEY_EXCHANGE,
  SSL_EVTID_SEND_CERTIFICATE_VERIFY,
  SSL_EVTID_RECV_CERTIFICATE_VERIFY,
  SSL_EVTID_SEND_NEW_SESSION_TICKET,
  SSL_EVTID_RECV_NEW_SESSION_TICKET,
  SSL_EVTID_SEND_CHANGE_CIPHER_SPEC,
  SSL_EVTID_RECV_CHANGE_CIPHER_SPEC,
  SSL_EVTID_SEND_FINISHED,
  SSL_EVTID_RECV_FINISHED,
  SSL_EVTID_LAST
};

/*********************************************************************
*
*       SSL profile API
*/
typedef struct {
  void (*pfRecordEndCall)   (unsigned int EventId);
  void (*pfRecordEndCallU32)(unsigned int EventId, U32 Para0);
  void (*pfRecordVoid)      (unsigned int EventId);
  void (*pfRecordU32)       (unsigned int EventId, U32 Para0);
  void (*pfRecordU32x2)     (unsigned int EventId, U32 Para0, U32 Para1);
  void (*pfRecordU32x3)     (unsigned int EventId, U32 Para0, U32 Para1, U32 Para2);
} SSL_PROFILE_API;

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

extern SEGGER_MEM_CONTEXT     * SSL_MEM__pDefaultContext;
extern const SSL_CIPHER_API     SSL_CIPHER_NULL_API;
extern const SSL_MAC_API        SSL_MAC_TLS1_API;
extern const SSL_PROFILE_API  * SSL_PROFILE_pAPI;
extern unsigned                 SSL_PROFILE_IdOffset;
extern SEGGER_MEM_SIMPLE_HEAP   SSL_MEM_SimpleHeap;
extern SEGGER_MEM_BUDDY_HEAP    SSL_MEM_BuddyHeap;

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Internal cipher suite functions.
*/
const char           * SSL_SUITE_GetKeyExchangeName    (SSL_KEY_EXCHANGE_ID ID);
const char           * SSL_SUITE_GetCipherName         (SSL_CIPHER_ID ID);
const char           * SSL_SUITE_GetMACAlgorithmName   (SSL_HASH_ALGORITHM_ID ID);
const char           * SSL_SUITE_GetPKAlgorithmName    (CRYPTO_X509_PK_ALGORITHM_ID ID);
CRYPTO_X509_PK_ALGORITHM_ID SSL_SUITE_QueryPKAlgorithm (SSL_KEY_EXCHANGE_ID ID);
void                   SSL_SUITE_Exit                  (void);
const SSL_CIPHER_API * SSL_CIPHER_FindByIndex          (unsigned Index);
const SSL_MAC_API    * SSL_MAC_FindByIndex             (unsigned Index);

/*********************************************************************
*
*       TLV decoding functions
*/
SSL_HASH_ALGORITHM_ID       SSL_OID_DecodeDigestAlgorithm    (const CRYPTO_TLV *pOID);
SSL_SIGNATURE_ALGORITHM_ID  SSL_OID_DecodeSignatureAlgorithm (const CRYPTO_TLV *pOID);


/*********************************************************************
*
*       TLS memory wrappers.
*/
void              * SSL_MEM_Alloc     (unsigned ByteCnt);
void              * SSL_MEM_ZeroAlloc (unsigned ByteCnt);
void                SSL_MEM_Free      (void *pMemory);
void              * SSL_MEM_Realloc   (void *pMemory, unsigned ByteCnt);

/*********************************************************************
*
*       TLS memory architecture.
*/
void                SSL_MEM_ConfigBinaryBuddy(void);
void                SSL_MEM_ConfigBestFit    (void);
void                SSL_MEM_ConfigUser       (SEGGER_MEM_CONTEXT *pMem);

/*********************************************************************
*
*       Elliptic curves.
*/
void                SSL_CURVE_Exit        (void);
const SSL_CURVE   * SSL_CURVE_FindByID    (unsigned ID);
const SSL_CURVE   * SSL_CURVE_FindByOID   (const U8 *pOID, unsigned OIDByteCnt);
const SSL_CURVE   * SSL_CURVE_FindByIndex (unsigned Index);

/*********************************************************************
*
*       Pseudo-random functions.
*/
void                SSL_PRF_TLS1       (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_MD5        (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_MD5_XOR    (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA1       (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA1_XOR   (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA224     (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA224_XOR (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA256     (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA256_XOR (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA384     (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA384_XOR (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA512     (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);
void                SSL_PRF_SHA512_XOR (const U8 *pSecret, unsigned SecretLen, const U8 *pLabel, unsigned LabelLen, const U8 *pSeed, unsigned SeedLen, U8 *pOutput, unsigned OutputLen);

/*********************************************************************
*
*       Profiling functions.
*/
U32                 SSL_PROFILE_GetAPIDesc(const char **psDesc);
void                SSL_PROFILE_SetAPI    (const SSL_PROFILE_API *pAPI, U32 IdOffset);

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
