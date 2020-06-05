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
* Filename : net_secure_emssl.h
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
#include  <SEGGER.h>
#include  <SSL.h>
#include  <SSL_Types.h>
#include  <CRYPTO.h>

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
*                                      GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void  SSL_X_Config   (void);
void  SSL_OS_Init    (void);
void  CRYPTO_OS_Init (void);
void  CRYPTO_X_Config(void);

#endif
