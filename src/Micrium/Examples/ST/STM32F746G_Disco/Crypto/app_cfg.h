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
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
* Filename : app_cfg.h
*********************************************************************************************************
*/

#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_TCPIP_EN                       DEF_ENABLED
#define  APP_CFG_DHCP_C_EN                      DEF_ENABLED
#define  APP_CFG_HTTP_S_EN                      DEF_DISABLED
#define  APP_CFG_HTTP_C_EN                      DEF_ENABLED
#define  APP_CFG_FS_EN                          DEF_DISABLED


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                  3u
#define  NET_CFG_TASK_TX_DEALLOC_PRIO             4u
#define  NET_CFG_TASK_RX_PRIO                    25u
#define  NET_CFG_TASK_TMR_PRIO                   18u
#define  APP_CFG_TASK_HTTP_TEST_PRIO              6u
#define  HTTPs_OS_CFG_INSTANCE_TASK_PRIO          7u
#define  DHCPc_OS_CFG_TASK_PRIO                   8u
#define  DHCPc_OS_CFG_TMR_TASK_PRIO               9u
#define  CLK_OS_CFG_TASK_PRIO                     10


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE            1700u
#define  APP_CFG_TASK_HTTP_STK_SIZE             2048u


#define  NET_CFG_TASK_RX_STK_SIZE               2048u
#define  NET_CFG_TASK_TX_DEALLOC_STK_SIZE       2048u
#define  NET_CFG_TASK_TMR_STK_SIZE              2048u

#define  DHCPc_OS_CFG_TASK_STK_SIZE             2048u
#define  DHCPc_OS_CFG_TMR_TASK_STK_SIZE         1024u
#define  CLK_OS_CFG_TASK_STK_SIZE               1024u
#define  HTTPs_OS_CFG_INSTANCE_TASK_STK_SIZE    3096u


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                        0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                       1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                        2u
#endif

#define  APP_TRACE_LEVEL                        TRACE_LEVEL_INFO
#define  APP_TRACE                              printf

#define  APP_TRACE_INFO(x)               ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)

#define  SSL_DEBUG                               2

#endif
