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
*                                            EXAMPLE CODE
*
*                                         STM32F746G-DISCO
*                                         Evaluation Board
*
* Filename : main.c
* Version  : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <stm32f7xx_hal.h>
#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>
#include  <app_cfg.h>
#include  <bsp_int.h>
#include  <bsp_led.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>

#if (APP_CFG_TCPIP_EN == DEF_ENABLED)
#include  <app_tcpip.h>

#if (APP_CFG_HTTP_S_EN == DEF_ENABLED)
#include  <app_http-s.h>
#endif

#if (APP_CFG_HTTP_C_EN == DEF_ENABLED)
#include  <http-c_app.h>
#endif
#endif

#if (APP_CFG_FS_EN == DEF_ENABLED)
#include  <fs_app.h>
#endif

#include  <clk.h>


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* --------------- APPLICATION GLOBALS ---------------- */
static  OS_TCB          AppTaskStartTCB;
static  CPU_STK         AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE]; 
        CLK_TS_SEC      App_SystemResetTime;
#if (APP_CFG_HTTP_C_EN == DEF_ENABLED) && \
    (NET_SECURE_CFG_EN == DEF_ENABLED)  
static  HTTPc_CONN_OBJ  AppSecureServerConn;
extern  HTTPc_REQ_OBJ   HTTPcApp_Req;
#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void         AppTaskStart       (void  *p_arg);
static  void         AppTaskCreate      (void);
static  void         AppObjCreate       (void);
        CPU_BOOLEAN  AppHTTPc_ReqSendGet(void);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 1) HAL library initialization:
*                      a) Configures the Flash prefetch, intruction and data caches.
*                      b) Configures the Systick to generate an interrupt. However, the function ,
*                         HAL_InitTick(), that initializes the Systick has been overwritten since Micrium's
*                         RTOS has its own Systick initialization and it is recommended to initialize the
*                         Systick after multitasking has started.
*
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR   os_err;
    CPU_ERR  cpu_err;


    HAL_Init();                                                 /* Initialize STM32Cube HAL Library                     */
    BSP_ClkInit();                                              /* Initialize the main clock                            */
    BSP_IntInit();                                              /* Initialize RAM interrupt vector table.               */
    BSP_OS_TickInit();                                          /* Initialize kernel tick timer                         */

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    Math_Init();                                                /* Initialize Mathematical Module                       */

    CPU_IntDis();                                               /* Disable all Interrupts.                              */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    CPU_NameSet("STM32F746", &cpu_err);

    OSInit(&os_err);                                            /* Init uC/OS-III.                                      */
    if (os_err != OS_ERR_NONE) {
        while (1);
    }

    App_OS_SetAllHooks();

    OSTaskCreate(&AppTaskStartTCB,                              /* Create the start task                                */
                  "App Task Start",
                  AppTaskStart,
                  0u,
                  APP_CFG_TASK_START_PRIO,
                 &AppTaskStartStk[0u],
                  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                  APP_CFG_TASK_START_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
    if (os_err != OS_ERR_NONE) {
        while (1);
    }

    OSStart(&os_err);                                           /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
#if (APP_CFG_TCPIP_EN == DEF_ENABLED)
    CPU_BOOLEAN    started;
#endif
    CPU_BOOLEAN    success;
    CLK_ERR        clk_err;
    CLK_DATE_TIME  date_time;
    CPU_CHAR       date_str[CLK_STR_FMT_DAY_MONTH_DD_YYYY_MAX_LEN];
    OS_ERR         err;


   (void)p_arg;

    OS_TRACE_INIT();                                            /* Initialize the OS Trace recorder                     */

    BSP_OS_TickEnable();                                        /* Enable the tick timer and interrupt                  */

    BSP_LED_Init();                                             /* Initialize LEDs                                      */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    Clk_Init(&clk_err);

#if (APP_CFG_FS_EN == DEF_ENABLED)
    started = App_FS_Init();
    if (started != DEF_YES) {
        APP_TRACE_INFO(("Initializing uC/FS ... FAILED!! \r\n"));
    }
#endif

#if (APP_CFG_TCPIP_EN == DEF_ENABLED)
    started = AppTCPIP_Init();                                  /* Initialize uC/TCPIP Module.                          */

    if (started == DEF_YES) {
#if (APP_CFG_HTTP_S_EN == DEF_ENABLED)
        started = AppHTTPs_Init();                              /* Initialize HTTPs modules.                            */
        if (started != DEF_YES) {
            APP_TRACE_INFO(("App_HTTPs_Init() returned an error, uC/HTTPs is not started"));
        }
#endif
#if (APP_CFG_HTTP_C_EN == DEF_ENABLED)
        started = HTTPcApp_Init();
        if (started != DEF_YES) {
            APP_TRACE_INFO(("HTTPc_Init() returned an error, uC/HTTPc is not started"));
        }
#endif
    } else {
        APP_TRACE_INFO(("Initializing uC/TCP-IP ... FAILED!! \r\n"));
    }
#endif

    APP_TRACE_DBG(("Creating Application kernel objects\n\r"));
    AppObjCreate();                                             /* Create Applicaiton kernel objects                    */

    APP_TRACE_DBG(("Creating Application Tasks\n\r"));
    AppTaskCreate();                                            /* Create Application tasks                             */

    App_SystemResetTime = Clk_GetTS();                          /* Record reset time & output current date from uC-Clk. */
    success             = Clk_GetDateTime(&date_time);

    if (success) {
        success = Clk_DateTimeToStr(&date_time,
                                     CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY,
                                     date_str,
                                     CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY_LEN);
        if (success) {
            APP_TRACE_INFO(("\r\nCurrent Date and Time: %s. \r\n\r\n", date_str));
        }
    }
    
    success = DEF_FALSE;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        BSP_LED_Toggle();        
#if (APP_CFG_HTTP_C_EN == DEF_ENABLED) && \
    (NET_SECURE_CFG_EN == DEF_ENABLED)                          /* Attempt to connect to the remote server.             */
     
        if (started) {
            while (!HTTPcApp_ServerConn(&AppSecureServerConn)) {
                APP_TRACE_DBG(("\r\nFailed Connection Attempt.\r\n"));
                OSTimeDlyHMSM( 0u, 0u, 0u, 500u,
                               OS_OPT_TIME_HMSM_STRICT,
                              &err);
            }

            Mem_Clr(HTTPcApp_Buf, sizeof(HTTPcApp_Buf));
            success = HTTPcApp_ServerReq(&AppSecureServerConn); /* Send request to server.                              */
        }
#endif
        OSTimeDlyHMSM( 0u, 0u, 5u, 0u,
                       OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}


/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{

}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppObjCreate (void)
{

}
