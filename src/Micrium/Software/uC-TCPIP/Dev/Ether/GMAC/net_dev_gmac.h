/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                         (c) Copyright 2004-2018; Silicon Laboratories Inc.,
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
*                                        NETWORK DEVICE DRIVER
*
*                                            GMAC 10/100
*
* Filename : net_dev_gmac.h
* Version  : V3.04.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  NET_DEV_GMAC_MODULE_PRESENT
#define  NET_DEV_GMAC_MODULE_PRESENT

#include  <IF/net_if_ether.h>

#ifdef  NET_IF_ETHER_MODULE_EN


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
* Note(s) : (1) The following MCUs are support by NetDev_API_GMAC API:
*
*                          STMicroelectronics  STM32F107xx  series
*                          STMicroelectronics  STM32F2xxx   series
*                          STMicroelectronics  STM32F4xxx   series
*                          STMicroelectronics  STM32F74xxx  series
*                          STMicroelectronics  STM32F75xxx  series
*                          FUJITSU             MB9BFD10T    series
*                          FUJITSU             MB9BF610T    series
*                          FUJITSU             MB9BF210T    series
*
*           (2) The following MCUs are support by NetDev_API_LPCXX_ENET API:
*
*                          NXP                 LPC185x      series
*                          NXP                 LPC183x      series
*                          NXP                 LPC435x      series
*                          NXP                 LPC433x      series
*
*           (3) The following MCUs are support by NetDev_API_XMC4000_ENET API:
*
*                          INFINEON            XMC4500      series
*                          INFINEON            XMC4400      series
*
*           (4) The following MCUs are support by NetDev_API_TM4C12X_ENET API:
*
*                          TEXAS INSTRUMENTS   TM4C12x      series
*
*********************************************************************************************************
*/

extern  const  NET_DEV_API_ETHER  NetDev_API_GMAC;
extern  const  NET_DEV_API_ETHER  NetDev_API_LPCXX_ENET;
extern  const  NET_DEV_API_ETHER  NetDev_API_XMC4000_ENET;
extern  const  NET_DEV_API_ETHER  NetDev_API_TM4C12X_ENET;
extern  const  NET_DEV_API_ETHER  NetDev_API_HPS_EMAC;
extern  const  NET_DEV_API_ETHER  NetDev_API_STM32Fx_EMAC;


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif /* NET_IF_ETHER_MODULE_EN      */
#endif /* NET_DEV_GMAC_MODULE_PRESENT */
