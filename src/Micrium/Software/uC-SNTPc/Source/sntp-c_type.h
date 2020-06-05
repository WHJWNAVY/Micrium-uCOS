/*
*********************************************************************************************************
*                                              uC/SNTPc
*                                Simple Network Time Protocol (client)
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
*                                             SNTP CLIENT
*
* Filename     : sntp-c_type.h
* Version      : V2.00.01
* Programmer(s): AL
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SNTPc present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  SNTPc_TYPE_PRESENT                                     /* See Note #1.                                         */
#define  SNTPc_TYPE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <Source/net_type.h>


/*
*********************************************************************************************************
*                                    SNTPc CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef struct sntp_server_cfg {

    CPU_CHAR             *ServerHostnamePtr;
    NET_PORT_NBR          ServerPortNbr;
    NET_IP_ADDR_FAMILY    ServerAddrFamily;
    CPU_INT32U            ReqRxTimeout_ms;

}SNTPc_CFG;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/
#endif                                                          /* End of lib mem module include.                       */

