/*
*********************************************************************************************************
*                                               uC/SNTPc
*                                  Simple Network Time Protocol (client)
*
*                         (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/SNTPc is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                   SNTP CLIENT CONFIGURATION FILE
*
* Filename      : sntp-c_cfg.c
* Version       : V2.00.01
* Programmer(s) : AL
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  <Source/sntp-c.h>


/*
*********************************************************************************************************
*                                 SNTPc CLIENT CONFIGURATION STRUCTURE
*********************************************************************************************************
*/

SNTPc_CFG  SNTPc_Cfg = {

/*
*--------------------------------------------------------------------------------------------------------
*                                    SNTP SERVER CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/
                                                        /* NTP Server hostname or IP address.                           */
    "1.pool.ntp.org",
                                                        /* NTP Server port number.                                      */
     SNTPc_DFLT_IPPORT,
                                                        /* Select IP family of address when DNS resolution is used:     */
     NET_IP_ADDR_FAMILY_IPv4,
                                                        /* NET_IP_ADDR_FAMILY_NONE: No preference between IPv6 and IPv4.*/
                                                        /* NET_IP_ADDR_FAMILY_IPv4: Only IPv4 addr will be returned.    */
                                                        /* NET_IP_ADDR_FAMILY_IPv6: Only IPv6 addr will be returned.    */

/*
*--------------------------------------------------------------------------------------------------------
*                                     TIMEOUT CONFIGURATION
*--------------------------------------------------------------------------------------------------------
*/
     20000,                                            /* Maximum inactivity time (ms) on RX.                          */

};

