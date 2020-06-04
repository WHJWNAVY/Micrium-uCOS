/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                      Texas Instruments MSP430X
*                                               on the
*                                           MSP-EXP430F5438
*                                          Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : HS
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                                 DEFINES
*********************************************************************************************************
*/

#define  LCD_LINE_BUF       18u                                 /* The last character should always be a null character     */
#define  LCD_NUM_LINES       9u

#define  PB_LEFT            DEF_BIT_01
#define  PB_RIGHT           DEF_BIT_02
#define  PB_SELECT          DEF_BIT_03
#define  PB_UP              DEF_BIT_04
#define  PB_DOWN            DEF_BIT_05
#define  PB_S1              DEF_BIT_06
#define  PB_S2              DEF_BIT_07

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

static  OS_STK      AppTaskStartStk[APP_START_TASK_STK_SIZE];
static  OS_STK      AppTask1Stk[APP_TASK1_STK_SIZE];
static  OS_STK      AppTask2Stk[APP_TASK2_STK_SIZE];
static  OS_STK      AppTask3Stk[APP_TASK3_STK_SIZE];

static  CPU_CHAR    AppLCDTxt0[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt1[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt2[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt3[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt4[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt5[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt6[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt7[LCD_LINE_BUF];
static  CPU_CHAR    AppLCDTxt8[LCD_LINE_BUF];

static  CPU_CHAR    AppLCDTxt[(LCD_LINE_BUF * LCD_NUM_LINES)] =
                    "  0  Micrium      000 uCOS-II  00   0 ---------\
                     Time Ticks                        Context Switches                                   ";


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppDispInit   (void);
static  void  AppDispUpdate (void);
static  void  AppTaskCreate (void);
static  void  AppTaskStart  (void        *p_arg);
static  void  AppTask1      (void        *p_arg);
static  void  AppTask2      (void        *p_arg);
static  void  AppTask3      (void        *p_arg);
static  void  AppUpdatePB   (CPU_INT08U   pb_pressed);
static  void  AppFormatDec  (CPU_CHAR    *dst, CPU_INT32U  val, CPU_INT08U  len);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary CPU and C initialization.
*
* Arguments   : none
*********************************************************************************************************
*/

void  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                            /* Disable all int. until we are ready to accept them   */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_START_TASK_STK_SIZE - 1u],
                    APP_START_TASK_PRIO,
                    APP_START_TASK_PRIO,
                    (OS_STK *)&AppTaskStartStk[0u],
                    APP_START_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_START_TASK_PRIO, "Startup", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                               AppTaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Note(s)     : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif

    AppDispInit();                                              /* Initialize the Display layout                        */

    AppTaskCreate();                                            /* Create application tasks                             */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        AppDispUpdate();
        OSTimeDlyHMSM(0u, 0u, 0u, 100u);
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                               AppTaskCreate()
*
* Description: This function creates the application tasks.
*
* Arguments  : none.
*
* Note(s)    : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    CPU_INT08U  err;


    OSTaskCreateExt(AppTask1,                                   /* Create start task 1                                  */
                   (void *)0,
                   (OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE - 1u],
                    APP_START_TASK1_PRIO,
                    APP_START_TASK1_PRIO,
                   (OS_STK *)&AppTask1Stk[0u],
                    APP_TASK1_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_START_TASK1_PRIO, "Task 1", &err);
#endif


    OSTaskCreateExt(AppTask2,                                   /* Create start task 2                                  */
                   (void *)0,
                   (OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE - 1u],
                    APP_START_TASK2_PRIO,
                    APP_START_TASK2_PRIO,
                   (OS_STK *)&AppTask2Stk[0u],
                    APP_TASK2_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_START_TASK2_PRIO, "Task 2", &err);
#endif


    OSTaskCreateExt(AppTask3,                                   /* Create start task 3                                  */
                   (void *)0,
                   (OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE - 1u],
                    APP_START_TASK3_PRIO,
                    APP_START_TASK3_PRIO,
                   (OS_STK *)&AppTask3Stk[0u],
                    APP_TASK3_STK_SIZE,
                   (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_START_TASK3_PRIO, "Task 3", &err);
#endif
}


/*
*********************************************************************************************************
*                                                AppTask1()
*
* Description : This task periodically updates values to be output to the display
*
* Arguments   : p_arg   is the argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTask1 (void *p_arg)
{
    while (DEF_TRUE) {
        AppFormatDec(&AppLCDTxt5[0u],                           /* Update the value to be displayed                     */
                     OSTime,
                     LCD_LINE_BUF);

        OSTimeDlyHMSM(0u, 0u, 0u, 50u);
    }
}


/*
*********************************************************************************************************
*                                                AppTask2()
*
* Description : This task periodically updates values to be output to the display
*
* Arguments   : p_arg   is the argument passed to 'AppTask3()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTask2 (void *p_arg)
{
    while (DEF_TRUE) {
        AppFormatDec(&AppLCDTxt7[0u],                           /* Update the value to be displayed                     */
                     OSCtxSwCtr,
                     LCD_LINE_BUF);

        OSTimeDlyHMSM(0u, 0u, 0u, 50u);
    }
}


/*
*********************************************************************************************************
*                                                AppTask3()
*
* Description : This task periodically checks the status of the board's buttons
*
* Arguments   : p_arg   is the argument passed to 'AppTask3()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppTask3 (void *p_arg)
{
    while (DEF_TRUE) {
        AppUpdatePB(PB_Rd());                                   /* Update the PB layout to be displayed on the LCD      */
        OSTimeDlyHMSM(0u, 0u, 0u, 100u);
    }
}


/*
*********************************************************************************************************
*                                               AppDispInit()
*
* Description : This initializes the display text.
*
* Arguments   : p_arg   is the argument passed to 'AppTask3()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppDispInit (void)
{
    CPU_CHAR    *tmp;
    CPU_INT08U   i;


    i = 0u;

    while (i < ((LCD_LINE_BUF - 1u) * LCD_NUM_LINES)) {         /* Break the initial display string into smaller ones   */
        switch (i / (LCD_LINE_BUF - 1u)) {
            case 0u:
                 tmp = &AppLCDTxt0[0u];
                 break;


            case 1u:
                 tmp = &AppLCDTxt1[0u];
                 break;


            case 2u:
                 tmp = &AppLCDTxt2[0u];
                 break;


            case 3u:
                 tmp = &AppLCDTxt3[0u];
                 break;


            case 4u:
                 tmp = &AppLCDTxt4[0u];
                 break;


            case 5u:
                 tmp = &AppLCDTxt5[0u];
                 break;


            case 6u:
                 tmp = &AppLCDTxt6[0u];
                 break;


            case 7u:
                 tmp = &AppLCDTxt7[0u];
                 break;


            case 8u:
                 tmp = &AppLCDTxt8[0u];
                 break;


            default:
                 tmp = 0u;                                      /* Error, set exit conditions                           */
                 i   = ((LCD_LINE_BUF - 1u) * LCD_NUM_LINES);
                 break;
        }

        do {
            tmp[i % 17u] = AppLCDTxt[i];
            i++;
        } while (((i % 17u) != 0u));

        tmp[17u] = 0u;
    }
}


/*
*********************************************************************************************************
*                                               AppDispUpdate()
*
* Description : This updates the display.
*
* Arguments   : p_arg   is the argument passed to 'AppTask3()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppDispUpdate (void)
{
    LCD_PrintLineCol (&AppLCDTxt0[0u], 0u, 0u);
    LCD_PrintLineCol (&AppLCDTxt1[0u], 1u, 0u);
    LCD_PrintLineCol (&AppLCDTxt2[0u], 2u, 0u);
    LCD_PrintLineCol (&AppLCDTxt3[0u], 3u, 0u);
    LCD_PrintLineCol (&AppLCDTxt4[0u], 4u, 0u);
    LCD_PrintLineCol (&AppLCDTxt5[0u], 5u, 0u);
    LCD_PrintLineCol (&AppLCDTxt6[0u], 6u, 0u);
    LCD_PrintLineCol (&AppLCDTxt7[0u], 7u, 0u);
    LCD_PrintLineCol (&AppLCDTxt8[0u], 8u, 0u);
}

/*
*********************************************************************************************************
*                                               AppFormatDec()
*
* Description : This formats a right aligned decimal string.
*
* Arguments   : dst     pointer to the memory location to store the formatted string
*               val     the value to format to string
*               len     the length of the string
*
* Returns     : none
*********************************************************************************************************
*/

void  AppFormatDec (CPU_CHAR  *dst, CPU_INT32U  val, CPU_INT08U  len)
{
    CPU_INT08U  i;
    CPU_INT08U  j;
    CPU_INT08U  k;
    CPU_INT08U  tmp[LCD_LINE_BUF];


    BSP_FormatDec(&tmp[0u], val, 10u);                          /* Get a string representation of the value             */

    tmp[10u] = 0u;                                              /* Null terminator                                      */

    i = 0u;

                                                                /* Find the number of leading 0 characters              */
    while ((tmp[i] == '0') && (i < (len - 2u)) && (tmp[i] != 0u)) {
        i++;
    };

    if (i == (len - 2u)) {                                      /* If the entire string is 0...                         */
        dst[len - 2u] = '0';                                    /* Set the last character as 0                          */
    } else {
        j = i;

        while (tmp[j++] != 0u);                                 /* Iterate to the end of the string (null terminated)   */

        k = j - i - 1u;                                         /*  Determine the number of actual digits               */
        j = k;

        while (k--) {
            dst[len - (k + 2u)] = tmp[i++];                     /* Copy the digits with a right alignment               */
        }

        i -= j;                                                 /* Determine the number of non-digits                   */

        while (i < (len - (j + 1u))) {
            dst[i++] = ' ';                                     /* "Erase" the unimportant characters                   */
        }
    }

    dst[len - 1u] = 0u;                                         /* Null terminate the string                            */
}


/*
*********************************************************************************************************
*                                               AppUpdatePB()
*
* Description : This updates the push buttons pressed and the strings to display.
*
* Arguments   : p_arg   is the argument passed to 'AppTask3()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

void  AppUpdatePB (CPU_INT08U  pb_pressed)
{
    if (pb_pressed & PB_LEFT) {
        AppLCDTxt1[1u]  = '0';
    } else {
        AppLCDTxt1[1u]  = 'o';
    }

    if (pb_pressed & PB_RIGHT) {
        AppLCDTxt1[3u]  = '0';
    } else {
        AppLCDTxt1[3u]  = 'o';
    }

    if (pb_pressed & PB_SELECT) {
        AppLCDTxt1[2u]  = '0';
    } else {
        AppLCDTxt1[2u]  = 'o';
    }

    if (pb_pressed & PB_UP) {
        AppLCDTxt0[2u]  = '0';
    } else {
        AppLCDTxt0[2u]  = 'o';
    }

    if (pb_pressed & PB_DOWN) {
        AppLCDTxt2[2u]  = '0';
    } else {
        AppLCDTxt2[2u]  = 'o';
    }

    if (pb_pressed & PB_S1) {
        AppLCDTxt1[14u] = '0';
    } else {
        AppLCDTxt1[14u] = 'o';
    }

    if (pb_pressed & PB_S2) {
        AppLCDTxt1[15u] = '0';
    } else {
        AppLCDTxt1[15u] = 'o';
    }
}
