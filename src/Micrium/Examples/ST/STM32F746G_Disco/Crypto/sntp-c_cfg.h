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
* Filename      : sntp-c_cfg.h
* Version       : V2.00.01
* Programmer(s) : JDH
*                 SR
*                 AL
*********************************************************************************************************
*/

#ifndef SNTPc_CFG_MODULE_PRESENT
#define SNTPc_CFG_MODULE_PRESENT

#include  <Source/sntp-c_type.h>


/*
*********************************************************************************************************
*                                  SNTPc ARGUMENT CHECK CONFIGURATION
*
* Note(s) : (1) Configure SNTPc_CFG_ARG_CHK_EXT_EN to enable/disable the SNTP client external argument
*               check feature :
*
*               (a) When ENABLED,  ALL arguments received from any port interface provided by the developer
*                   are checked/validated.
*
*               (b) When DISABLED, NO  arguments received from any port interface provided by the developer
*                   are checked/validated.
*********************************************************************************************************
*/
                                                                /* Configure external argument check feature ...        */
                                                                /* See Note 1.                                          */
#define  SNTPc_CFG_ARG_CHK_EXT_EN                   DEF_ENABLED
                                                                /* DEF_DISABLED     External argument check DISABLED    */
                                                                /* DEF_ENABLED      External argument check ENABLED     */

/*
*********************************************************************************************************
*                                SNTPc RUN-TIME STRUCTURE CONFIGURATION
*
* Note(s) : (1) This structure should be defined into a 'C' file.
*********************************************************************************************************
*/

extern  SNTPc_CFG  SNTPc_Cfg;


/*
*********************************************************************************************************
*                                                TRACING
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                   0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                                  1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                   2
#endif

#define  SNTPc_TRACE_LEVEL                      TRACE_LEVEL_INFO
#define  SNTPc_TRACE                            printf

#endif
