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

File        : SSL_Types.h
Purpose     : Internal TLS types.

*/

#ifndef SSL_TYPES_H
#define SSL_TYPES_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER.h"
#include "CRYPTO.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define SSL_MASTER_SECRET_LENGTH        48
#define SSL_RANDOM_LENGTH               32
#define SSL_VERIFY_DATA_LEN             12
#define SSL_MAX_SESSION_ID_LENGTH       32

#define SSL_MK_SIGNATURE_ALG(HASH, ENC) ((unsigned)(HASH)*0x100 + (unsigned)(ENC))
#define SSL_EX_HASH(ID)                 ((SSL_HASH_ALGORITHM_ID)      ((unsigned)(ID) >> 8))
#define SSL_EX_ENC(ID)                  ((CRYPTO_X509_PK_ALGORITHM_ID)((unsigned)(ID) & 0xFF))


/*********************************************************************
*
*       Local data types
*
**********************************************************************
*/

// Crypto suite hash algorithms assigned by IANA.
//   http://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-18
typedef enum {
  SSL_HASH_ALGORITHM_ID_NULL,
  SSL_HASH_ALGORITHM_ID_TLS1 = SSL_HASH_ALGORITHM_ID_NULL,   // Overload NULL MAC as TLS1.
  SSL_HASH_ALGORITHM_ID_MD5,
  SSL_HASH_ALGORITHM_ID_SHA,
  SSL_HASH_ALGORITHM_ID_SHA224,
  SSL_HASH_ALGORITHM_ID_SHA256,
  SSL_HASH_ALGORITHM_ID_SHA384,
  SSL_HASH_ALGORITHM_ID_SHA512,
  SSL_HASH_ALGORITHM_ID__MAX,
  SSL_HASH_ALGORITHM_ID_INVALID = SSL_HASH_ALGORITHM_ID__MAX
} SSL_HASH_ALGORITHM_ID;

// Ciphersuite ciphers
typedef enum {
  SSL_CIPHER_ID_NULL,
  SSL_CIPHER_ID_3DES_EDE_CBC,
  SSL_CIPHER_ID_AES_128_CBC,
  SSL_CIPHER_ID_AES_256_CBC,
  SSL_CIPHER_ID_AES_128_GCM,
  SSL_CIPHER_ID_AES_256_GCM,
  SSL_CIPHER_ID_AES_128_CCM,
  SSL_CIPHER_ID_AES_256_CCM,
  SSL_CIPHER_ID_AES_128_CCM_8,
  SSL_CIPHER_ID_AES_256_CCM_8,
  SSL_CIPHER_ID_RC4_128,
  SSL_CIPHER_ID_ARIA_128_CBC,
  SSL_CIPHER_ID_ARIA_256_CBC,
  SSL_CIPHER_ID_ARIA_128_GCM,
  SSL_CIPHER_ID_ARIA_256_GCM,
  SSL_CIPHER_ID_CAMELLIA_128_CBC,
  SSL_CIPHER_ID_CAMELLIA_256_CBC,
  SSL_CIPHER_ID_CAMELLIA_128_GCM,
  SSL_CIPHER_ID_CAMELLIA_256_GCM,
  SSL_CIPHER_ID_SEED_CBC,
  SSL_CIPHER_ID_CHACHA20_POLY1305,
  SSL_CIPHER_ID__MAX
} SSL_CIPHER_ID;

// Ciphersuite key exchanges
typedef enum {
  SSL_KEY_EXCHANGE_ID_NULL,
  SSL_KEY_EXCHANGE_ID_PSK,
  SSL_KEY_EXCHANGE_ID_RSA,
  SSL_KEY_EXCHANGE_ID_RSA_PSK,
  SSL_KEY_EXCHANGE_ID_DH_RSA,
  SSL_KEY_EXCHANGE_ID_DHE_RSA,
  SSL_KEY_EXCHANGE_ID_DHE_DSS,
  SSL_KEY_EXCHANGE_ID_DHE_PSK,
  SSL_KEY_EXCHANGE_ID_ECDH_RSA,       // Static ECDH signed with RSA    -- ECDH key is provided in server certificate, no ServerKeyExchange [https://tools.ietf.org/html/rfc4492#section-2.3]
  SSL_KEY_EXCHANGE_ID_ECDH_ECDSA,     // Static ECDH signed with ECDSA  -- ECDH key is provided in server certificate, no ServerKeyExchange [https://tools.ietf.org/html/rfc4492#section-2.1]
  SSL_KEY_EXCHANGE_ID_ECDHE_RSA,
  SSL_KEY_EXCHANGE_ID_ECDHE_ECDSA,
  SSL_KEY_EXCHANGE_ID_ECDHE_PSK
} SSL_KEY_EXCHANGE_ID;

#define SSL_PRF_ID_TLS1    SSL_HASH_ALGORITHM_ID_TLS1
#define SSL_PRF_ID_MD5     SSL_HASH_ALGORITHM_ID_MD5
#define SSL_PRF_ID_SHA     SSL_HASH_ALGORITHM_ID_SHA
#define SSL_PRF_ID_SHA224  SSL_HASH_ALGORITHM_ID_SHA224
#define SSL_PRF_ID_SHA256  SSL_HASH_ALGORITHM_ID_SHA256
#define SSL_PRF_ID_SHA384  SSL_HASH_ALGORITHM_ID_SHA384
#define SSL_PRF_ID_SHA512  SSL_HASH_ALGORITHM_ID_SHA512
#define SSL_PRF_ID__MAX    SSL_HASH_ALGORITHM_ID__MAX

// [TLS1v2] s. 7.4.1.4.1. SignatureAndHashAlgorithm
typedef enum {
  /* RSA */
  SSL_SIGNATURE_MD5_WITH_RSA_ENCRYPTION    = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_MD5,    CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  SSL_SIGNATURE_SHA_WITH_RSA_ENCRYPTION    = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA,    CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  SSL_SIGNATURE_SHA224_WITH_RSA_ENCRYPTION = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA224, CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  SSL_SIGNATURE_SHA256_WITH_RSA_ENCRYPTION = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA256, CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  SSL_SIGNATURE_SHA384_WITH_RSA_ENCRYPTION = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA384, CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  SSL_SIGNATURE_SHA512_WITH_RSA_ENCRYPTION = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA512, CRYPTO_X509_PK_ALGORITHM_ID_RSA),
  /* DSA */
  SSL_SIGNATURE_SHA_WITH_DSA               = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA,    CRYPTO_X509_PK_ALGORITHM_ID_DSA),
  /* ECDSA */
  SSL_SIGNATURE_SHA_WITH_ECDSA             = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA,    CRYPTO_X509_PK_ALGORITHM_ID_ECDSA),
  SSL_SIGNATURE_SHA224_WITH_ECDSA          = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA224, CRYPTO_X509_PK_ALGORITHM_ID_ECDSA),
  SSL_SIGNATURE_SHA256_WITH_ECDSA          = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA256, CRYPTO_X509_PK_ALGORITHM_ID_ECDSA),
  SSL_SIGNATURE_SHA384_WITH_ECDSA          = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA384, CRYPTO_X509_PK_ALGORITHM_ID_ECDSA),
  SSL_SIGNATURE_SHA512_WITH_ECDSA          = SSL_MK_SIGNATURE_ALG(SSL_HASH_ALGORITHM_ID_SHA512, CRYPTO_X509_PK_ALGORITHM_ID_ECDSA),
  
  SSL_SIGNATURE_INVALID = 0xFFFF
} SSL_SIGNATURE_ALGORITHM_ID;

// TLS record-layer message type
typedef enum { 
  SSL_CONTENT_CHANGE_CIPHER_SPEC = 20,
  SSL_CONTENT_ALERT              = 21,
  SSL_CONTENT_HANDSHAKE          = 22,
  SSL_CONTENT_APPLICATION_DATA   = 23
} SSL_CONTENT_TYPE;

// [RFC4492] s. 5.1.1
typedef enum {
  SSL_NAMED_CURVE_sect163k1       =  1,
  SSL_NAMED_CURVE_sect163r1       =  2,
  SSL_NAMED_CURVE_sect163r2       =  3,
  SSL_NAMED_CURVE_sect193r1       =  4,
  SSL_NAMED_CURVE_sect193r2       =  5,
  SSL_NAMED_CURVE_sect233k1       =  6,
  SSL_NAMED_CURVE_sect233r1       =  7,
  SSL_NAMED_CURVE_sect239k1       =  8,
  SSL_NAMED_CURVE_sect283k1       =  9,
  SSL_NAMED_CURVE_sect283r1       = 10,
  SSL_NAMED_CURVE_sect409k1       = 11,
  SSL_NAMED_CURVE_sect409r1       = 12,
  SSL_NAMED_CURVE_sect571k1       = 13,
  SSL_NAMED_CURVE_sect571r1       = 14,
  SSL_NAMED_CURVE_secp160k1       = 15,
  SSL_NAMED_CURVE_secp160r1       = 16,
  SSL_NAMED_CURVE_secp160r2       = 17,
  SSL_NAMED_CURVE_secp192k1       = 18,
  SSL_NAMED_CURVE_secp192r1       = 19, 
  SSL_NAMED_CURVE_secp224k1       = 20,
  SSL_NAMED_CURVE_secp224r1       = 21,
  SSL_NAMED_CURVE_secp256k1       = 22, 
  SSL_NAMED_CURVE_secp256r1       = 23,
  SSL_NAMED_CURVE_secp384r1       = 24,
  SSL_NAMED_CURVE_secp521r1       = 25,
  SSL_NAMED_CURVE_brainpoolP256r1 = 26,
  SSL_NAMED_CURVE_brainpoolP384r1 = 27,
  SSL_NAMED_CURVE_brainpoolP512r1 = 28,
  SSL_NAMED_CURVE_curve25519      = 29,
  SSL_NAMED_CURVE_curve448        = 30,
  SSL_NAMED_CURVE_INVALID
} SSL_NAMED_CURVE;

// TLS alert message severity.
typedef enum {
  SSL_ALERT_LEVEL_WARNING = 1,
  SSL_ALERT_LEVEL_FATAL   = 2
} SSL_ALERT_LEVEL;

//
// Handshake message types (section 7.4) [HandshakeType]
//
// See IANA TLS Handshake Type Registry
//   http://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-7
//
typedef enum {
  SSL_HANDSHAKE_HELLO_REQUEST          =  0,         // [RFC5246]
  SSL_HANDSHAKE_CLIENT_HELLO           =  1,         // [RFC5246]
  SSL_HANDSHAKE_SERVER_HELLO           =  2,         // [RFC5246]
  SSL_HANDSHAKE_HELLO_VERIFY_REQUEST   =  3,         // [RFC6347]
  SSL_HANDSHAKE_NEW_SESSION_TICKET     =  4,         // [RFC4507]
  SSL_HANDSHAKE_CERTIFICATE            = 11,         // [RFC5246]
  SSL_HANDSHAKE_SERVER_KEY_EXCHANGE    = 12,         // [RFC5246]
  SSL_HANDSHAKE_CERTIFICATE_REQUEST    = 13,         // [RFC5246]
  SSL_HANDSHAKE_SERVER_HELLO_DONE      = 14,         // [RFC5246]
  SSL_HANDSHAKE_CERTIFICATE_VERIFY     = 15,         // [RFC5246]
  SSL_HANDSHAKE_CLIENT_KEY_EXCHANGE    = 16,         // [RFC5246]
  SSL_HANDSHAKE_FINISHED               = 20,         // [RFC5246]
  SSL_HANDSHAKE_CERTIFICATE_URL        = 21,         // [RFC6066]
  SSL_HANDSHAKE_CERTIFICATE_STATUS     = 22,         // [RFC6066]
  SSL_HANDSHAKE_SUPPLEMENTAL_DATA      = 23          // [RFC4680]
} SSL_HANDSHAKE_TYPE;

typedef enum {
  SSL_ALERT_CLOSE_NOTIFY            =   0,
  SSL_ALERT_UNEXPECTED_MESSAGE      =  10,
  SSL_ALERT_BAD_MESSAGE_MAC         =  20,
  SSL_ALERT_DECRYPTION_FAILED       =  21,
  SSL_ALERT_RECORD_OVERFLOW         =  22,
  SSL_ALERT_DECOMPRESSION_FAILURE   =  30,
  SSL_ALERT_HANDSHAKE_FAILURE       =  40,
  SSL_ALERT_BAD_CERTIFICATE         =  42,
  SSL_ALERT_UNSUPPORTED_CERTIFICATE =  43,
  SSL_ALERT_CERTIFICATE_REVOKED     =  44,
  SSL_ALERT_CERTIFICATE_EXPIRED     =  45,
  SSL_ALERT_CERTIFICATE_UNKNOWN     =  46,
  SSL_ALERT_ILLEGAL_PARAMETER       =  47,
  SSL_ALERT_UNKNOWN_CA              =  48,
  SSL_ALERT_ACCESS_DENIED           =  49,
  SSL_ALERT_DECODE_ERROR            =  50,
  SSL_ALERT_DECRYPT_ERROR           =  51,
  SSL_ALERT_EXPORT_RESTRICTION      =  60,
  SSL_ALERT_PROTOCOL_VERSION        =  70,
  SSL_ALERT_INSUFFICIENT_SECURITY   =  71,
  SSL_ALERT_INTERNAL_ERROR          =  80,
  SSL_ALERT_INAPPROPRIATE_FALLBACK  =  86, // [RFC 7507 https://tools.ietf.org/html/rfc7507#section-2] Section 2.
  SSL_ALERT_USER_CANCELED           =  90,
  SSL_ALERT_NO_RENEGOTIATION        = 100
} SSL_ALERT_DESCRIPTION;

//
// SSL extensions.  See [RFC6066 https://tools.ietf.org/html/rfc6066#section-1.1] Section 1.1.
typedef enum {
  SSL_EXTENSION_ID_SERVER_NAME                  =  0,     // [RFC6066]
  SSL_EXTENSION_ID_MAX_FRAGMENT_LENGTH          =  1,     // [RFC6066]
  SSL_EXTENSION_ID_CLIENT_CERTIFICATE_URL       =  2,     // [RFC6066]
  SSL_EXTENSION_ID_TRUSTED_CA_KEYS              =  3,     // [RFC6066]
  SSL_EXTENSION_ID_TRUNCATED_HMAC               =  4,     // [RFC6066]
  SSL_EXTENSION_ID_STATUS_REQUEST               =  5,     // [RFC6066]
  SSL_EXTENSION_ID_USER_MAPPING                 =  6,     // [RFC4681]
  SSL_EXTENSION_ID_CLIENT_AUTHZ                 =  7,     // [RFC5878]
  SSL_EXTENSION_ID_SERVER_AUTHZ                 =  8,     // [RFC5878]
  SSL_EXTENSION_ID_CERT_TYPE                    =  9,     // [RFC6091]
  SSL_EXTENSION_ID_SUPPORED_GROUPS              = 10,     // [RFC4492][RFC7919]
  SSL_EXTENSION_ID_EC_POINT_FORMATS             = 11,     // [RFC4492]
  SSL_EXTENSION_ID_SRP                          = 12,     // [RFC5054]
  SSL_EXTENSION_ID_SIGNATURE_ALGORITHMS         = 13,     // [RFC5246]
  SSL_EXTENSION_ID_USE_SRP                      = 14,     // [RFC5764]
  SSL_EXTENSION_ID_HEARTBEAT                    = 15,     // [RFC6520]
  SSL_EXTENSION_ID_ALPN                         = 16,     // [RFC7301]  application_layer_protocol_negotiation
  SSL_EXTENSION_ID_STATUS_REQUEST_V2            = 17,     // [RFC7301]
  SSL_EXTENSION_ID_SIGNED_CERTIFICATE_TIMESTAMP = 18,     // [RFC6962]
  SSL_EXTENSION_ID_CLIENT_CERTIFICATE_TYPE      = 19,     // [RFC7250]
  SSL_EXTENSION_ID_SERVER_CERTIFICATE_TYPE      = 19,     // [RFC7250]
  SSL_EXTENSION_ID_PADDING                      = 21,     // [RFC7685]
  SSL_EXTENSION_ID_ENCRYPT_THEN_MAC             = 22,     // [RFC7366]
  SSL_EXTENSION_ID_EXTENDED_MASTER_SECRET       = 23,     // [RFC7627]
  SSL_EXTENSION_ID_CACHED_INFO                  = 25,     // [RFC7924]
  SSL_EXTENSION_ID_SESSION_TICKET               = 35,     // [RFC4507]
  SSL_EXTENSION_ID_SECURE_RENEGOTIATION         = 0xFF01  // [RFC5746]  renegotiaion_info
} SSL_EXTENSION_ID;

typedef enum {
  SSL_NAME_TYPE_HOST_NAME = 0
} SSL_NAME_TYPE;

// [RFC4492] s. 5.4. ECCurveType
typedef enum {
  SSL_ECC_CURVE_TYPE_EXPLICIT_PRIME  = 1,
  SSL_ECC_CURVE_TYPE_EXPLICIT_CHAR_2 = 2,
  SSL_ECC_CURVE_TYPE_NAMED           = 3
} SSL_EC_CURVE_TYPE;

// [RFC4492] s. 5.1.2.  ECPointFormat
typedef enum {
  SSL_ECC_POINT_FORMAT_UNCOMPRESSED              = 0,
  SSL_ECC_POINT_FORMAT_ANSIX962_COMPRESSED_PRIME = 1,
  SSL_ECC_POINT_FORMAT_ANSIX962_COMPRESSED_CHAR2 = 2
} SSL_EC_POINT_FORMAT;

// [X9.62]
typedef enum {
  SSL_ECC_POINT_TYPE_COMPRESSED   = 2,
  SSL_ECC_POINT_TYPE_UNCOMPRESSED = 4
} SSL_EC_POINT_TYPE;

// [RFC5246 https://tools.ietf.org/html/rfc5246#section-7.4.4] section 7.4.4.
typedef enum {
  SSL_CERTIFICATE_TYPE_RSA_SIGNED                = 1,
  SSL_CERTIFICATE_TYPE_DSS_SIGNED                = 2,
  SSL_CERTIFICATE_TYPE_RSA_FIXED_DH              = 3,
  SSL_CERTIFICATE_TYPE_DSS_FIXED_DH              = 4,
  SSL_CERTIFICATE_TYPE_RSA_EPHEMERAL_DH_RESERVED = 5,
  SSL_CERTIFICATE_TYPE_DSS_EPHEMERAL_DH_RESERVED = 6,
  SSL_CERTIFICATE_TYPE_FORTEZZA_DMS_RESERVED     = 20
} SSL_CERTIFICATE_TYPE;

typedef struct {
  CRYPTO_MPI d;      // random integer < n; this is the private key
  CRYPTO_EC_POINT Q; // Q = d * G; this is the public key
} SSL_EC_KEY;

typedef enum {
  SSL_CONNECTION_END_CLIENT = 0,
  SSL_CONNECTION_END_SERVER = SSL_VERIFY_DATA_LEN
} SSL_CONNECTION_END;

typedef struct {
  SSL_CIPHER_ID ID;
  unsigned      ContextSize;
  void (*pfInitEncrypt)   (void *pContext, const U8 *pKey);
  void (*pfInitDecrypt)   (void *pContext, const U8 *pKey);
  void (*pfBulkEncrypt)   (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, U8 *pIV);
  void (*pfBulkDecrypt)   (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, U8 *pIV);
  void (*pfAEADEncrypt)   (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, const U8 *pAAD, unsigned AADLen, const U8 *pIV);
  int  (*pfAEADDecrypt)   (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, const U8 *pAAD, unsigned AADLen, const U8 *pIV);
  void (*pfChaChaEncrypt) (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, const U8 *pAAD, unsigned AADLen, const U8 *pIV);
  int  (*pfChaChaDecrypt) (void *pContext, U8 *pOutput, const U8 *pInput, unsigned InputLen, const U8 *pAAD, unsigned AADLen, const U8 *pIV);
} SSL_CIPHER_API;

typedef union {
  CRYPTO_MD5_CONTEXT         HASH_MD5;
  CRYPTO_SHA1_CONTEXT        HASH_SHA1;
  CRYPTO_SHA224_CONTEXT      HASH_SHA224;
  CRYPTO_SHA256_CONTEXT      HASH_SHA256;
  CRYPTO_SHA384_CONTEXT      HASH_SHA384;
  CRYPTO_SHA512_CONTEXT      HASH_SHA512;
  CRYPTO_MD5SHA1_CONTEXT     HASH_MD5SHA1;
} SSL_HASH_CONTEXT;

typedef union {
  CRYPTO_HMAC_MD5_CONTEXT    HMAC_MD5;
  CRYPTO_HMAC_SHA1_CONTEXT   HMAC_SHA1;
  CRYPTO_HMAC_SHA224_CONTEXT HMAC_SHA224;
  CRYPTO_HMAC_SHA256_CONTEXT HMAC_SHA256;
  CRYPTO_HMAC_SHA384_CONTEXT HMAC_SHA384;
  CRYPTO_HMAC_SHA512_CONTEXT HMAC_SHA512;
} SSL_MAC_CONTEXT;

typedef struct {
  U8       ID;              /*SSL_HASH_ALGORITHM_ID*/
  U8       DigestLen;
  U16      HashContextSize;

  // For RSA signatures
  const U8 * pRSASigHeader;
  unsigned   RSASigHeaderLen;

  // Hashing methods
  void (*pfHashInit)  (void *pSelf);
  void (*pfHashAdd)   (void *pSelf, const U8 *pInput,  unsigned InputLen);
  void (*pfHashGet)   (void *pSelf,       U8 *pDigest, unsigned DigestLen);
  void (*pfHashFinal) (void *pSelf,       U8 *pDigest, unsigned DigestLen);

  // HMAC methods
  void (*pfHMACInit)  (void *pSelf, const U8 *pKey,  unsigned KeyLen);
  void (*pfHMACAdd)   (void *pSelf, const U8 *pData, unsigned DataLen);
  void (*pfHMACFinal) (void *pSelf,       U8 *pDigest);

  // PRF
  unsigned PRFSize;  // Size in bytes of PRF's digest for handshake
  void (*pfPRF)(const U8 *pSecret, unsigned SecretLen,
                const U8 *pLabel,  unsigned LabelLen, 
                const U8 *pSeed,   unsigned SeedLen,
                      U8 *pOutput, unsigned OutputLen);
} SSL_MAC_API;

typedef struct {
  unsigned ID;
} SSL_PRF_API;

typedef struct SSL_SESSION_tag SSL_SESSION;

typedef struct {
  U8       * pKeyExchMessage;
  int        KeyExchMessageLen;
  U8       * pPremasterSecret;
  unsigned   PremasterSecretLen;
} SSL_KEX_PARAS;

typedef struct {
  int (*pfClientSendKEX)   (SSL_SESSION *pSelf, SSL_KEX_PARAS *pParas);    // Construct client key exchange data in server mode
  int (*pfServerGenKeys)   (SSL_SESSION *pSelf, CRYPTO_BUFFER *pPDU);      // Generate ephemeral keys in server mode
  int (*pfServerProcessKEX)(SSL_SESSION *pSelf, CRYPTO_TLV *pTLV);         // Process incoming ClientKeyExchange
} SSL_KEX_API;

typedef struct {
  U16                     ID;             // TLS-assigned curve ID
  const CRYPTO_EC_CURVE * pCurve;         // Curve parameters for prime curves
  int (*pfClientSendKEX)   (SSL_SESSION *pSelf, SSL_KEX_PARAS *pParas);    // Construct client key exchange data in server mode
  int (*pfServerGenKeys)   (SSL_SESSION *pSelf, CRYPTO_BUFFER *pPDU);      // Generate ephemeral keys in server mode
  int (*pfServerProcessKEX)(SSL_SESSION *pSelf, CRYPTO_TLV *pTLV);         // Process incoming ClientKeyExchange
} SSL_CURVE;

extern const SSL_KEX_API SSL_KEX_INVALID;
extern const SSL_KEX_API SSL_KEX_RSA;
extern const SSL_KEX_API SSL_KEX_DH_RSA;
extern const SSL_KEX_API SSL_KEX_DHE_RSA;
extern const SSL_KEX_API SSL_KEX_ECDH_RSA;
extern const SSL_KEX_API SSL_KEX_ECDH_ECDSA;
extern const SSL_KEX_API SSL_KEX_ECDHE_RSA;
extern const SSL_KEX_API SSL_KEX_ECDHE_ECDSA;

// Current KEXs that are unimplemented
#define SSL_KEX_PSK        SSL_KEX_INVALID
#define SSL_KEX_DHE_PSK    SSL_KEX_INVALID
#define SSL_KEX_RSA_PSK    SSL_KEX_INVALID
#define SSL_KEX_ECDHE_PSK  SSL_KEX_INVALID

typedef struct SSL_SUITE {
  U16                     ID;
  SSL_KEY_EXCHANGE_ID     KeyExchangeID;     /*:8*/
  SSL_CIPHER_ID           CipherID;          /*:8*/
  SSL_HASH_ALGORITHM_ID   MACID;             /*:8*/
  U8                      BlockSize;         // Bulk cipher block size
  U8                      KeySize;           // Bulk cipher key size
  U8                      IVSize;            // = ImplicitIVSize + ExplicitIVSize
  U8                      ImplicitIVSize;    // Size of cipher implicit IV in bytes
  U8                      ExplicitIVSize;    // Size of cipher explicit IV in bytes
  U8                      MACSize;           // Computed MAC or authentication tag size in bytes
  U8                      MACKeySize;        // MAC key size in bytes
  const SSL_KEX_API     * pServerKEXAPI;     // Pointer to API methods for client key exchange when running in server mode
  U16                     MinVersion;        // Minimum TLS version this suite is valid for
  U16                     MaxVersion;        // Maximum TLS version this suite is valid for
} SSL_SUITE;

typedef struct {
  U8              * pMACKey;
  U8              * pKey;
  U8              * pIV;
  void            * pContext;
  const SSL_SUITE * pCipherSuite;
  U32               SequenceNumber;
} SSL_PROTECTION_PARAS;

typedef struct {
  U16                 Mask;
  const SSL_MAC_API * PRF_MAC;
} SSL_PROTOCOL_API;

/*
For reference: Message flow for a full handshake

      Client                                               Server  Server State

      ClientHello                  -------->                       SERVER_WAIT_CLIENT_HELLO
                                                      ServerHello  SERVER_SEND_SERVER_HELLO
                                                     Certificate*  SERVER_SEND_CERTIFICATE
                                               ServerKeyExchange*  SERVER_SEND_SERVER_KEY_EXCHANGE
                                              CertificateRequest*  SERVER_SEND_CERTIFICATE_REQUEST
                                   <--------      ServerHelloDone  SERVER_SEND_HELLO_DONE
      Certificate*                                                 SERVER_WAIT_CERTIFICATE
      ClientKeyExchange                                            SERVER_WAIT_CLIENT_KEY_EXCHANGE
      CertificateVerify*                                           SERVER_WAIT_CERTIFICATE_VERIFY
      [ChangeCipherSpec]                                           SERVER_WAIT_CHANGE_CIPHER_SPEC
      Finished                     -------->                       SERVER_WAIT_FINISHED
                                               [ChangeCipherSpec]  SERVER_SEND_CHANGE_CIPHER_SPEC
                                   <--------             Finished  SERVER_SEND_FINISHED
      Application Data             <------->     Application Data  CONNECTED


* Indicates optional or situation-dependent messages that are not
  always sent.

* To help avoid pipeline stalls, ChangeCipherSpec is an independent
  TLS protocol content type, and is not actually a TLS handshake
  message.
*/

typedef enum {
  //
  // States at client end.
  //
  SSL_CLIENT_SEND_CLIENT_HELLO,
  SSL_CLIENT_WAIT_SERVER_HELLO,
  SSL_CLIENT_WAIT_CERTIFICATE,
  SSL_CLIENT_WAIT_SERVER_KEY_EXCHANGE,
  SSL_CLIENT_WAIT_CERTIFICATE_REQUEST,
  SSL_CLIENT_WAIT_SERVER_HELLO_DONE,
  SSL_CLIENT_WAIT_SESSION_TICKET,
  SSL_CLIENT_SEND_CERTIFICATE,
  SSL_CLIENT_SEND_CLIENT_KEY_EXCHANGE,
  SSL_CLIENT_SEND_CERTIFICATE_VERIFY,
  SSL_CLIENT_SEND_CHANGE_CIPHER_SPEC,
  SSL_CLIENT_SEND_FINISHED,
  SSL_CLIENT_WAIT_CHANGE_CIPHER_SPEC,
  SSL_CLIENT_WAIT_FINISHED,
  //
  // States at server end.
  //
  SSL_SERVER_WAIT_CLIENT_HELLO,
  SSL_SERVER_SEND_SERVER_HELLO,
  SSL_SERVER_SEND_CERTIFICATE,
  SSL_SERVER_SEND_CERTIFICATE_REQUEST,
  SSL_SERVER_SEND_SERVER_HELLO_DONE,
  SSL_SERVER_SEND_SERVER_KEY_EXCHANGE,
  SSL_SERVER_SEND_CHANGE_CIPHER_SPEC,
  SSL_SERVER_WAIT_CERTIFICATE,
  SSL_SERVER_WAIT_CLIENT_KEY_EXCHANGE,
  SSL_SERVER_WAIT_CHANGE_CIPHER_SPEC,
  SSL_SERVER_SEND_FINISHED,
  SSL_SERVER_WAIT_FINISHED,
  //
  SSL_CONNECTED,
  SSL_CLOSED,
  SSL_ERRORED
} SSL_HANDSHAKE_STATE;

typedef enum {
  SSL_SESSION_RESUME_TYPE_NULL,
  SSL_SESSION_RESUME_TYPE_ID,
  SSL_SESSION_RESUME_TYPE_TICKET
} SSL_SESSION_RESUME_TYPE;

typedef struct {
  SSL_SESSION_RESUME_TYPE Type;
  U8                      aMasterSecret[SSL_MASTER_SECRET_LENGTH];
  union {
    struct {
      unsigned SessionIDLen;
      U8       aSessionID[SSL_MAX_SESSION_ID_LENGTH];
    } ID;
    struct {
      unsigned TicketLen;
      U8       aTicket[SSL_MAX_SESSION_TICKET_LEN ? SSL_MAX_SESSION_TICKET_LEN : 1];
    } Ticket;
  } u;
} SSL_SESSION_RESUME_PARAS;

typedef int (SSL_HANDSHAKE_MESSAGE_FUNC)(SSL_SESSION *pSelf, const U8 *pMessage, unsigned MessageLen);

struct SSL_SESSION_tag {
  //
  // State variables, better encoded as a client/server state.
  //
  SSL_HANDSHAKE_STATE State;
  //
  // Unread data held at record layer.
  //
  U8       * pUnreadBuffer;
  unsigned   UnreadLen;
  unsigned   UnreadStart;
  //
  int        Socket;
  //
  // Defined connection end, client or server.
  //
  SSL_CONNECTION_END    ConnectionEnd;
  //
  // TLS version we and the peer will be using.
  //
  U16 ProtocolVersion;
  U16 RecordLayerVersion;
  //
  // Minimum and maximum protocol versions that we are prepared
  // to agree on.  These are restricted to the supported TLS
  // version that emSSL is configured for.
  //
  U16 OwnMinVersion;
  U16 OwnMaxVersion;
  //
  // Per-session cipher suites.
  //
  const U16 *pSuites;
  unsigned   SuiteCnt;
  //
  // Server name for SNI.
  //
  const char *sServerName;
  //
  // PRF selected for this connection.
  //
  const        SSL_MAC_API             * pPRF;
  const struct SSL_TRANSPORT_API_tag   * pIO;
  const struct SSL_CERITIFCATE_API_tag * pCertificate;
  //
  // Public key material.
  //
  const SSL_CURVE          * pServerECDHECurve;    // Server's ECDH curve
  CRYPTO_MPI                 ServerXKey;           // Server's ephemeral ECDH public key for X25519 and X448
  CRYPTO_EC_POINT            ServerECDHEKey;       // Server's ephemeral ECDH public key for ECDHE suites using prime curves
  CRYPTO_DH_KA_CONTEXT       DH;                   // Enpoint DH key agreement
  CRYPTO_MPI                 ServerPublic;         // Server's public value (Y)
  //
  const SSL_CURVE          * pClientCurve;         // Server's selected curve from client list
  CRYPTO_MPI                 ClientXKey;           // Server's ephemeral ECDH public key for X25519 and X448
  CRYPTO_ECDH_PRIVATE_KEY    ClientECDHPrivateKey; // Server's ephemeral ECDH private key for ECDHE suites
  CRYPTO_ECDH_PUBLIC_KEY     ClientECDHPublicKey;  // Server's ephemeral ECDH public key for ECDHE suites
  //
  // Handshake flags and methods.
  //
  unsigned   Flags;                     // Per-session flags
  U16        aOfferedSigAlgs[12];       // Client: signature algorithms offered by server and in our list.
  //  
  int        SupportSecureRenegotiation;
  SSL_HANDSHAKE_MESSAGE_FUNC *pfProcessHandshake;
  //
  // Current session ID for stateful resumption (client) or identifier (server).
  //
  unsigned   SessionIDLen;
  U8         aSessionID[SSL_MAX_SESSION_ID_LENGTH];
  //
  // Protocol data.
  //
  U8         aMasterSecret[SSL_MASTER_SECRET_LENGTH];
  U8         aClientRandom[SSL_RANDOM_LENGTH];
  U8         aServerRandom[SSL_RANDOM_LENGTH];
  //
  // Client verify data followed by server verify data.
  //
  U8         aVerifyData[2*SSL_VERIFY_DATA_LEN];
  //
  // Holding area for incoming resume.
  //
  SSL_SESSION_RESUME_PARAS SessionTicketOffered;
  SSL_SESSION_RESUME_PARAS SessionTicketRequested;
  //
  // Server and client certificates at start of chain.
  //
  CRYPTO_X509_CERTIFICATE_DATA ServerCert;
  CRYPTO_X509_CERTIFICATE_DATA ClientCert;
  //
  // SSL protection, i.e. cipher suite, sequence number, IVs, and so on.
  //
  SSL_PROTECTION_PARAS PendingTxParams;
  SSL_PROTECTION_PARAS PendingRxParams;
  SSL_PROTECTION_PARAS ActiveTxParams;
  SSL_PROTECTION_PARAS ActiveRxParams;
  //
  // If emSSL is configured to support more than one version of TLS,
  // then unfortunately we need to keep all these digests around as
  // we don't know which one will be required before we start to
  // hash data at the record layer.  Same for client certificate verification
  // which requires that we sign or verify handshake messages with the
  // appropriate algorithm defined by the certificate.
  //
  void *aHandshakeDigest[SSL_HASH_ALGORITHM_ID__MAX];
};

typedef struct {
  CRYPTO_X509_PK_ALGORITHM_ID ID;
  //
  int (*pfVerifyHandshake)  (      SSL_SESSION                 * pSelf,
                             const U8                          * pMessage,
                                   unsigned                      MessageLen,
                             const U8                          * pSignature,
                                   unsigned                      SignatureLen,
                                   SSL_HASH_ALGORITHM_ID         HashAlgorithm,
                                   CRYPTO_X509_PUBLIC_KEY_INFO * pPublicKey);
  //
  int (*pfVerifyCertificate)(      SSL_SESSION                 * pSelf,
                             const U8                          * pMessage,
                                   unsigned                      MessageLen,
                             const U8                          * pSignature,
                                   unsigned                      SignatureLen,
                                   SSL_HASH_ALGORITHM_ID         HashAlgorithm,
                                   CRYPTO_X509_PUBLIC_KEY_INFO * pPublicKey);
} SSL_SIGNATURE_VERIFY_API;

typedef struct {
  CRYPTO_X509_PK_ALGORITHM_ID ID;
  //
  int (*pfPutSignature)(SSL_SESSION *pSelf, CRYPTO_BUFFER *pPDU, U8 *pDigest, const SSL_MAC_API *pHashMethods);
} SSL_SIGNATURE_SIGN_API;

typedef struct {
  int (*pfSendCertificateRequest)   (SSL_SESSION *pSelf);
  int (*pfProcessCertificate)       (SSL_SESSION *pSelf, CRYPTO_TLV *pPDU);
  int (*pfProcessCertificateVerify) (SSL_SESSION *pSelf, CRYPTO_TLV *pPDU);
} SSL_SERVER_MUTUAL_AUTH_API;

typedef struct {
  int (*pfSendCertificateVerify)    (SSL_SESSION *pSelf);
} SSL_CLIENT_MUTUAL_AUTH_API;

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
