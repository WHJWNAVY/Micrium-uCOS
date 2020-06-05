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

File    : CRYPTO_OID_List.h
Purpose : Header file for ASN OIDs

*/

#ifndef CRYPTO_OID_LIST_H
#define CRYPTO_OID_LIST_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER.h"

extern const U8 CRYPTO_OID_brainpoolP512t1[7];  // 1.3.36.3.3.2.8.14
extern const U8 CRYPTO_OID_brainpoolP512r1[7];  // 1.3.36.3.3.2.8.13
extern const U8 CRYPTO_OID_brainpoolP384t1[7];  // 1.3.36.3.3.2.8.12
extern const U8 CRYPTO_OID_brainpoolP384r1[7];  // 1.3.36.3.3.2.8.11
extern const U8 CRYPTO_OID_brainpoolP320t1[7];  // 1.3.36.3.3.2.8.10
extern const U8 CRYPTO_OID_brainpoolP320r1[7];  // 1.3.36.3.3.2.8.9
extern const U8 CRYPTO_OID_brainpoolP256t1[7];  // 1.3.36.3.3.2.8.8
extern const U8 CRYPTO_OID_brainpoolP256r1[7];  // 1.3.36.3.3.2.8.7
extern const U8 CRYPTO_OID_brainpoolP224t1[7];  // 1.3.36.3.3.2.8.6
extern const U8 CRYPTO_OID_brainpoolP224r1[7];  // 1.3.36.3.3.2.8.5
extern const U8 CRYPTO_OID_brainpoolP192t1[7];  // 1.3.36.3.3.2.8.4
extern const U8 CRYPTO_OID_brainpoolP192r1[7];  // 1.3.36.3.3.2.8.3
extern const U8 CRYPTO_OID_brainpoolP160t1[7];  // 1.3.36.3.3.2.8.2
extern const U8 CRYPTO_OID_brainpoolP160r1[7];  // 1.3.36.3.3.2.8.1
extern const U8 CRYPTO_OID_secp521r1[5];  // 1.3.132.0.35
extern const U8 CRYPTO_OID_secp384r1[5];  // 1.3.132.0.34
extern const U8 CRYPTO_OID_secp256r1[8];  // 1.2.840.10045.3.1.7
extern const U8 CRYPTO_OID_secp256k1[5];  // 1.3.132.0.10
extern const U8 CRYPTO_OID_secp224r1[5];  // 1.3.132.0.33
extern const U8 CRYPTO_OID_secp224k1[5];  // 1.3.132.0.32
extern const U8 CRYPTO_OID_secp192r1[8];  // 1.2.840.10045.3.1.1
extern const U8 CRYPTO_OID_secp192k1[5];  // 1.3.132.0.31
extern const U8 CRYPTO_OID_id_aes256_CCM[9];  // 2.16.840.1.101.3.4.1.47
extern const U8 CRYPTO_OID_id_aes192_CCM[9];  // 2.16.840.1.101.3.4.1.27
extern const U8 CRYPTO_OID_id_aes128_CCM[9];  // 2.16.840.1.101.3.4.1.7
extern const U8 CRYPTO_OID_id_aes256_GCM[9];  // 2.16.840.1.101.3.4.1.46
extern const U8 CRYPTO_OID_id_aes192_GCM[9];  // 2.16.840.1.101.3.4.1.26
extern const U8 CRYPTO_OID_id_aes128_GCM[9];  // 2.16.840.1.101.3.4.1.6
extern const U8 CRYPTO_OID_aes[8];  // 2.16.840.1.101.3.4.1
extern const U8 CRYPTO_OID_id_rsassa_pkcs1_v1_5_with_sha3_512[9];  // 2.16.840.1.101.3.4.3.16
extern const U8 CRYPTO_OID_id_rsassa_pkcs1_v1_5_with_sha3_384[9];  // 2.16.840.1.101.3.4.3.15
extern const U8 CRYPTO_OID_id_rsassa_pkcs1_v1_5_with_sha3_256[9];  // 2.16.840.1.101.3.4.3.14
extern const U8 CRYPTO_OID_id_rsassa_pkcs1_v1_5_with_sha3_224[9];  // 2.16.840.1.101.3.4.3.13
extern const U8 CRYPTO_OID_id_ecdsa_with_sha3_12[9];  // 2.16.840.1.101.3.4.3.12
extern const U8 CRYPTO_OID_id_ecdsa_with_sha3_84[9];  // 2.16.840.1.101.3.4.3.11
extern const U8 CRYPTO_OID_id_ecdsa_with_sha3_56[9];  // 2.16.840.1.101.3.4.3.10
extern const U8 CRYPTO_OID_id_ecdsa_with_sha3_24[9];  // 2.16.840.1.101.3.4.3.9
extern const U8 CRYPTO_OID_ecdsa_with_SHA512[8];  // 1.2.840.10045.4.3.4
extern const U8 CRYPTO_OID_ecdsa_with_SHA384[8];  // 1.2.840.10045.4.3.3
extern const U8 CRYPTO_OID_ecdsa_with_SHA256[8];  // 1.2.840.10045.4.3.2
extern const U8 CRYPTO_OID_ecdsa_with_SHA224[8];  // 1.2.840.10045.4.3.1
extern const U8 CRYPTO_OID_id_dsa_with_sha3_512[9];  // 2.16.840.1.101.3.4.3.8
extern const U8 CRYPTO_OID_id_dsa_with_sha3_384[9];  // 2.16.840.1.101.3.4.3.7
extern const U8 CRYPTO_OID_id_dsa_with_sha3_256[9];  // 2.16.840.1.101.3.4.3.6
extern const U8 CRYPTO_OID_id_dsa_with_sha3_224[9];  // 2.16.840.1.101.3.4.3.5
extern const U8 CRYPTO_OID_id_dsa_with_sha512[9];  // 2.16.840.1.101.3.4.3.4
extern const U8 CRYPTO_OID_id_dsa_with_sha384[9];  // 2.16.840.1.101.3.4.3.3
extern const U8 CRYPTO_OID_id_dsa_with_sha256[9];  // 2.16.840.1.101.3.4.3.2
extern const U8 CRYPTO_OID_id_dsa_with_sha224[9];  // 2.16.840.1.101.3.4.3.1
extern const U8 CRYPTO_OID_prime256v1[8];  // 1.2.840.10045.3.1.7
extern const U8 CRYPTO_OID_prime239v3[8];  // 1.2.840.10045.3.1.6
extern const U8 CRYPTO_OID_prime239v2[8];  // 1.2.840.10045.3.1.5
extern const U8 CRYPTO_OID_prime239v1[8];  // 1.2.840.10045.3.1.4
extern const U8 CRYPTO_OID_prime192v3[8];  // 1.2.840.10045.3.1.3
extern const U8 CRYPTO_OID_prime192v2[8];  // 1.2.840.10045.3.1.2
extern const U8 CRYPTO_OID_prime192v1[8];  // 1.2.840.10045.3.1.1
extern const U8 CRYPTO_OID_primeCurve[7];  // 1.2.840.10045.3.1
extern const U8 CRYPTO_OID_c2tnb431r1[8];  // 1.2.840.10045.3.0.20
extern const U8 CRYPTO_OID_c2pnb368w1[8];  // 1.2.840.10045.3.0.19
extern const U8 CRYPTO_OID_c2tnb359v1[8];  // 1.2.840.10045.3.0.18
extern const U8 CRYPTO_OID_c2pnb304w1[8];  // 1.2.840.10045.3.0.17
extern const U8 CRYPTO_OID_c2pnb272w1[8];  // 1.2.840.10045.3.0.16
extern const U8 CRYPTO_OID_c2onb239v5[8];  // 1.2.840.10045.3.0.15
extern const U8 CRYPTO_OID_c2onb239v4[8];  // 1.2.840.10045.3.0.14
extern const U8 CRYPTO_OID_c2tnb239v3[8];  // 1.2.840.10045.3.0.13
extern const U8 CRYPTO_OID_c2tnb239v2[8];  // 1.2.840.10045.3.0.12
extern const U8 CRYPTO_OID_c2tnb239v1[8];  // 1.2.840.10045.3.0.11
extern const U8 CRYPTO_OID_c2pnb208w1[8];  // 1.2.840.10045.3.0.10
extern const U8 CRYPTO_OID_c2onb191v5[8];  // 1.2.840.10045.3.0.9
extern const U8 CRYPTO_OID_c2onb191v4[8];  // 1.2.840.10045.3.0.8
extern const U8 CRYPTO_OID_c2tnb191v3[8];  // 1.2.840.10045.3.0.7
extern const U8 CRYPTO_OID_c2tnb191v2[8];  // 1.2.840.10045.3.0.6
extern const U8 CRYPTO_OID_c2tnb191v1[8];  // 1.2.840.10045.3.0.5
extern const U8 CRYPTO_OID_c2pnb176w1[8];  // 1.2.840.10045.3.0.4
extern const U8 CRYPTO_OID_c2pnb163v3[8];  // 1.2.840.10045.3.0.3
extern const U8 CRYPTO_OID_c2pnb163v2[8];  // 1.2.840.10045.3.0.2
extern const U8 CRYPTO_OID_c2pnb163v1[8];  // 1.2.840.10045.3.0.1
extern const U8 CRYPTO_OID_c_TwoCurve[7];  // 1.2.840.10045.3.0
extern const U8 CRYPTO_OID_ellipticCurve[6];  // 1.2.840.10045.3
extern const U8 CRYPTO_OID_id_ecPublicKey[7];  // 1.2.840.10045.2.1
extern const U8 CRYPTO_OID_id_publicKeyType[6];  // 1.2.840.10045.2
extern const U8 CRYPTO_OID_ppBasis[9];  // 1.2.840.10045.1.2.3.3
extern const U8 CRYPTO_OID_tpBasis[9];  // 1.2.840.10045.1.2.3.2
extern const U8 CRYPTO_OID_gnBasis[9];  // 1.2.840.10045.1.2.3.1
extern const U8 CRYPTO_OID_id_characteristic_two_basis[8];  // 1.2.840.10045.1.2.3
extern const U8 CRYPTO_OID_characteristic_two_field[7];  // 1.2.840.10045.1.2
extern const U8 CRYPTO_OID_prime_field[7];  // 1.2.840.10045.1.1
extern const U8 CRYPTO_OID_id_fieldType[6];  // 1.2.840.10045.1
extern const U8 CRYPTO_OID_ecdsa_with_SHA1[7];  // 1.2.840.10045.4.1
extern const U8 CRYPTO_OID_id_ecSigType[6];  // 1.2.840.10045.4
extern const U8 CRYPTO_OID_id_keyExchangeAlgorithm[9];  // 2.16.840.1.101.2.1.1.22
extern const U8 CRYPTO_OID_dhpublicnumber[7];  // 1.2.840.10046.2.1
extern const U8 CRYPTO_OID_id_dsa_with_sha1[7];  // 1.2.840.10040.4.3
extern const U8 CRYPTO_OID_id_dsa[7];  // 1.2.840.10040.4.1
extern const U8 CRYPTO_OID_id_hmacWithSHA3_512[9];  // 2.16.840.1.101.3.4.2.16
extern const U8 CRYPTO_OID_id_hmacWithSHA3_384[9];  // 2.16.840.1.101.3.4.2.15
extern const U8 CRYPTO_OID_id_hmacWithSHA3_256[9];  // 2.16.840.1.101.3.4.2.14
extern const U8 CRYPTO_OID_id_hmacWithSHA3_224[9];  // 2.16.840.1.101.3.4.2.13
extern const U8 CRYPTO_OID_id_shake256[9];  // 2.16.840.1.101.3.4.2.12
extern const U8 CRYPTO_OID_id_shake128[9];  // 2.16.840.1.101.3.4.2.11
extern const U8 CRYPTO_OID_id_sha3_512[9];  // 2.16.840.1.101.3.4.2.10
extern const U8 CRYPTO_OID_id_sha3_384[9];  // 2.16.840.1.101.3.4.2.9
extern const U8 CRYPTO_OID_id_sha3_256[9];  // 2.16.840.1.101.3.4.2.8
extern const U8 CRYPTO_OID_id_sha3_224[9];  // 2.16.840.1.101.3.4.2.7
extern const U8 CRYPTO_OID_id_sha512_256[9];  // 2.16.840.1.101.3.4.2.6
extern const U8 CRYPTO_OID_id_sha512_224[9];  // 2.16.840.1.101.3.4.2.5
extern const U8 CRYPTO_OID_id_sha512[9];  // 2.16.840.1.101.3.4.2.3
extern const U8 CRYPTO_OID_id_sha384[9];  // 2.16.840.1.101.3.4.2.2
extern const U8 CRYPTO_OID_id_sha256[9];  // 2.16.840.1.101.3.4.2.1
extern const U8 CRYPTO_OID_id_sha224[9];  // 2.16.840.1.101.3.4.2.4
extern const U8 CRYPTO_OID_id_sha1[5];  // 1.3.14.3.2.26
extern const U8 CRYPTO_OID_id_hmacWithSHA1[8];  // 1.2.840.113549.2.7
extern const U8 CRYPTO_OID_id_md5[8];  // 1.2.840.113549.2.5
extern const U8 CRYPTO_OID_id_md2[8];  // 1.2.840.113549.2.2
extern const U8 CRYPTO_OID_id_pSpecified[9];  // 1.2.840.113549.1.1.9
extern const U8 CRYPTO_OID_id_mgf1[9];  // 1.2.840.113549.1.1.8
extern const U8 CRYPTO_OID_id_RSASSA_PSS[9];  // 1.2.840.113549.1.1.10
extern const U8 CRYPTO_OID_id_RSAES_OAEP[9];  // 1.2.840.113549.1.1.7
extern const U8 CRYPTO_OID_rsaEncryption[9];  // 1.2.840.113549.1.1.1
extern const U8 CRYPTO_OID_sha512WithRSAEncryption[9];  // 1.2.840.113549.1.1.13
extern const U8 CRYPTO_OID_sha384WithRSAEncryption[9];  // 1.2.840.113549.1.1.12
extern const U8 CRYPTO_OID_sha256WithRSAEncryption[9];  // 1.2.840.113549.1.1.11
extern const U8 CRYPTO_OID_sha224WithRSAEncryption[9];  // 1.2.840.113549.1.1.14
extern const U8 CRYPTO_OID_sha1WithRSAEncryption[9];  // 1.2.840.113549.1.1.5
extern const U8 CRYPTO_OID_md5WithRSAEncryption[9];  // 1.2.840.113549.1.1.4
extern const U8 CRYPTO_OID_md2WithRSAEncryption[9];  // 1.2.840.113549.1.1.2
extern const U8 CRYPTO_OID_rc5_CBC_PAD[8];  // 1.2.840.113549.3.9
extern const U8 CRYPTO_OID_des_EDE3_CBC[8];  // 1.2.840.113549.3.7
extern const U8 CRYPTO_OID_rc2CBC[8];  // 1.2.840.113549.3.2
extern const U8 CRYPTO_OID_desCBC[5];  // 1.3.14.3.2.7
extern const U8 CRYPTO_OID_id_PBMAC1[9];  // 1.2.840.113549.1.5.14
extern const U8 CRYPTO_OID_id_PBES2[9];  // 1.2.840.113549.1.5.13
extern const U8 CRYPTO_OID_id_PBKDF2[9];  // 1.2.840.113549.1.5.12
extern const U8 CRYPTO_OID_pbeWithSHA1AndRC2_CBC[9];  // 1.2.840.113549.1.5.11
extern const U8 CRYPTO_OID_pbeWithSHA1AndDES_CBC[9];  // 1.2.840.113549.1.5.10
extern const U8 CRYPTO_OID_pbeWithMD5AndRC2_CBC[9];  // 1.2.840.113549.1.5.6
extern const U8 CRYPTO_OID_pbeWithMD5AndDES_CBC[9];  // 1.2.840.113549.1.5.3
extern const U8 CRYPTO_OID_pbeWithMD2AndRC2_CBC[9];  // 1.2.840.113549.1.5.4
extern const U8 CRYPTO_OID_pbeWithMD2AndDES_CBC[9];  // 1.2.840.113549.1.5.1
extern const U8 CRYPTO_OID_id_at_organizationalUnitName[3];  // 2.5.4.11
extern const U8 CRYPTO_OID_id_at_organizationName[3];  // 2.5.4.10
extern const U8 CRYPTO_OID_id_at_stateOrProvinceName[3];  // 2.5.4.8
extern const U8 CRYPTO_OID_id_at_localityName[3];  // 2.5.4.7
extern const U8 CRYPTO_OID_id_at_countryName[3];  // 2.5.4.6
extern const U8 CRYPTO_OID_id_at_serialNumber[3];  // 2.5.4.5
extern const U8 CRYPTO_OID_id_at_commonName[3];  // 2.5.4.3
#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
