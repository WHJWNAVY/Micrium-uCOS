/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                         (c) Copyright 2008-2018; Silicon Laboratories Inc., 
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this 
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     NETWORK SECURITY PORT LAYER
*
*                                            SEGGER emSSL
*
* Filename : net_secure_emssl_cfg.h
* Version  : V3.04.06
*********************************************************************************************************
* Note(s)  : (1) Assumes the following versions (or more recent) of software modules are included in
*                the project build :
*
*                (a) SEGGER emSSL V2.54a
*                (b) uC/Clk V3.09
*
*                See also 'net.h  Note #1'.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    NET_SECURE_MODULE


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#define  NET_SECURE_EMSSL_MODULE
#ifdef   NET_SECURE_MODULE_EN


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define EMSSL_CFG_MIN_FOOTPRINT_EN             DEF_DISABLED     /* Minimal configuration.                              */
                                                                /* Install DHE cipher suite. Key agreement is slow ... */
#define EMSSL_CFG_DHE_RSA_CIPHER_SUITE         DEF_DISABLED     /* ... so it should be left disabled by default.       */
#define EMSSL_CFG_ECDHE_ECDSA_CIPHER_SUITE_EN  DEF_DISABLED     /* Install ECDHE-ECDSA cipher suites.                  */
#define EMSSL_CFG_ECDHE_RSA_CIPHER_SUITE_EN    DEF_DISABLED     /* Install ECDHE-RSA cipher suites.                    */
#define EMSSL_CFG_ECDH_ECDSA_CIPHER_SUITE_EN   DEF_DISABLED     /* Install ECDH-ECDSA cipher suites.                   */
#define EMSSL_CFG_ECDH_RSA_CIPHER_SUITE_EN     DEF_DISABLED     /* Install ECDH-RSA cipher suites.                     */
#define EMSSL_CFG_RSA_CIPHER_SUITE_EN          DEF_ENABLED      /* Install static RSA cipher suites.                   */
                                                                /* Install insecure NULL cipher suites for debugging...*/
#define EMSSL_CFG_NULL_CIPHER_SUITE_EN         DEF_DISABLED     /* ...purposes. No block encipherment.                 */

#endif