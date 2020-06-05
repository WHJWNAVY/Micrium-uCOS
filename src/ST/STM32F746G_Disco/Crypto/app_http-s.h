/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       HTTP INSTANCE APPLICATION
*
* Filename : app_http-s.h
* Version  : V3.00.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  APP_HTTP_S_H
#define  APP_HTTP_S_H


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <cpu.h>
#include  <lib_def.h>
#include  <app_cfg.h>
#include  <Source/os.h>
#include  "http-s_instance_cfg.h"


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   APP_HTTP_S_MODULE
#define  APP_HTTP_S_MODULE_EXT
#else
#define  APP_HTTP_S_MODULE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#ifndef  APP_CFG_HTTP_S_EN
#define  APP_CFG_HTTP_S_EN                       DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

APP_HTTP_S_MODULE_EXT  CPU_CHAR  App_Dflt_GW      [16u];
APP_HTTP_S_MODULE_EXT  CPU_CHAR  App_SubMsk       [16u];
APP_HTTP_S_MODULE_EXT  CPU_CHAR  App_Dflt_IP_Addr [16u];


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (APP_CFG_HTTP_S_EN == DEF_ENABLED)
  CPU_BOOLEAN  AppHTTPs_Init(void);
#endif

#endif
