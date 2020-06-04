/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               BSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                  Texas Instruments MSP-EXP430F5438
*                                         Experimenter Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : HS
*********************************************************************************************************
*/

#include  <includes.h>
#include  "hal_MSP-EXP430F5438.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  BSP_DLY_CONST      (BSP_CPU_CLK_FREQ / 1000000u)
#define  BSP_DLY_MAX        (0xFFFFFFFF / BSP_DLY_CONST)

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               PROTOTYPES
*********************************************************************************************************
*/

static  void  TMR_TickInit  (void);
static  void  LCD_Init      (void);
static  void  PB_Init       (void);

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         Global Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                BSP_Init()
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    TMR_TickInit();                                             /* Initialize the OS tick timer                         */
    LCD_Init();                                                 /* Initialize the LCD                                   */
    PB_Init();                                                  /* Initialize the push buttons                          */
}

/*
*********************************************************************************************************
*                                             BSP_IntDisAll()
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    _DINT();                                                    /* Disable all interrupts                               */
}

/*
*********************************************************************************************************
*                                             BSP_IntEnAll()
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  BSP_IntEnAll (void)
{
    _EINT();                                                    /* Enable all interrupts                                */
}

/*
*********************************************************************************************************
*                                                BSP_Dly()
*
* Description   : This function performs a short delay.
*
* Argument      : us    the delay period, approximately in microseconds
*
* Returns       : None
*********************************************************************************************************
*/

void  BSP_Dly (CPU_INT32U  us)
{
    CPU_INT32U  loops;
    CPU_INT32U  dly;


    loops = 0u;

    if (us > BSP_DLY_MAX) {                                     /* If the requested delay period exceeds our max ...    */
        loops += us / BSP_DLY_MAX;                              /* ... break the delay period into shorter ones.        */
        dly    = us % BSP_DLY_MAX;
    }

    dly = us * BSP_DLY_CONST;                                   /* Multiply desired dly period with the delay constant. */

    while (dly--);                                              /* Perform delay                                        */

    while (loops--) {                                           /* If required perform additional delays                */
        dly = BSP_DLY_MAX;
        while (dly--);
    }
}

/*
*********************************************************************************************************
*                                             TMR_TickInit()
*
* Description   : This function initializes the uC/OS-II's tick source.
*
* Argument    	: None
*
* Returns       : None
*********************************************************************************************************
*/


void  TMR_TickInit (void)
{

    SetVCore(PMMCOREV_3);                                       /* Set board to operate at 25 Mhz                       */

    SELECT_FLLREF(SELREF__REFOCLK);                             /* Set FLLCLK = REFO                                    */

    SELECT_ACLK(SELA__REFOCLK);                                 /* Set ACLK = REFO                                      */
#if (OS_VERSION < 30000u)
    Init_FLL_Settle(25000u, OS_TICKS_PER_SEC);
#else
    Init_FLL_Settle(25000u, OS_CFG_TICK_RATE_HZ);
#endif
    WDTCTL  = WDT_MDLY_32;                                      /* This configuration produces interrupts every 32  ... */
                                                                /* ... ms for SMCLK = 1 MHz. For a SMCLK = 25 MHz,  ... */
                                                                /* ... interrupt period is 1.3 ms.                      */

    SFRIE1 |= 1;                                                /* Enable WDT interrupts.                               */
}

/*
*********************************************************************************************************
*                                              PB_Init()
*
* Description   : This function initializes the push buttons.
*
* Argument      : None
*
* Returns       : None
*********************************************************************************************************
*/

void  PB_Init (void)
{
    halButtonsInit(BUTTON_ALL);                                 /* Configure the port where the PBs are attached        */
}

/*
*********************************************************************************************************
*                                               PB_Rd()
*
* Description   : This function returns the push buttons pressed.
*
* Argument      : None
*
* Returns       : None
*********************************************************************************************************
*/

CPU_INT08U  PB_Rd (void)
{
    return (halButtonsPressed());                               /* Read the port to determine the PBs pushed            */
}

/*
*********************************************************************************************************
*                                             LCD_Init()
*
* Description   : This function initializes the LCD for use.
*
* Argument      : None
*
* Returns       : None
*********************************************************************************************************
*/

void  LCD_Init (void)
{
    halLcdInit();                                               /* Initialize the LCD                                   */
    LCD_Clear();                                                /* Clear the screen                                     */
    halLcdBackLightInit();                                      /* Initialize the back light                            */
    halLcdSetBackLight(16u);                                    /* Set the back light to max                            */
    halLcdSetContrast(100u);                                    /* Set the contrast level                               */
}

/*
*********************************************************************************************************
*                                            LCD_Print()
*
* Description   : This function prints a string onto the LCD.
*
* Argument      : txt   a pointer to the string of text to be printed, last character must be null
*
* Returns       : None
*********************************************************************************************************
*/

void  LCD_Print (CPU_CHAR  *txt)
{
    halLcdPrint((char *)txt, OVERWRITE_TEXT);                   /* Prints a null delimited string                       */
}

/*
*********************************************************************************************************
*                                           LCD_SetLine()
*
* Description   : This function sets the cursor to a specified line.
*
* Argument      : line      the line to set the LCD to (0 - 8)
*
* Returns       : None
*********************************************************************************************************
*/

void  LCD_SetLine (CPU_INT08U  line)
{
    if (line < 9u) {
        halLcdPrintLine(0u, line, OVERWRITE_TEXT);              /* Sets the cursor to a specified location on the LCD   */
    }
}

/*
*********************************************************************************************************
*                                        LCD_PrintLineCol()
*
* Description   : This function prints at the given line and column
*
* Argument      : txt       the string to be printed
*                 x         line number
*                 y         column number
*
* Returns       : None
*********************************************************************************************************
*/

void  LCD_PrintLineCol (CPU_CHAR txt[], CPU_INT08U x, CPU_INT08U y)
{
    halLcdPrintLineCol(txt, x, y, OVERWRITE_TEXT);
}


/*
*********************************************************************************************************
*                                            LCD_Clear()
*
* Description   : This function clears the LCD.
*
* Argument      : None
*
* Returns       : None
*********************************************************************************************************
*/

void  LCD_Clear (void)
{
    halLcdClearScreen();                                        /* Clear the LCD                                        */
}

/*
*********************************************************************************************************
*                                          BSP_FormatDec()
*
* Description   : This function formats the decimal string to be sent.
*
* Argument      : s         Pointer to the location where the formatted string will be stored.
*                 value     The number to be sent.
*                 digits    The number of digits within the decimal string.
*
* Returns       : None
*********************************************************************************************************
*/

void  BSP_FormatDec (CPU_INT08U  *s, CPU_INT32U  value, CPU_INT08U  digits)
{
    CPU_INT08U  i;
    CPU_INT32U  mult;


    mult = 1u;

    for (i = 0u; i < (digits - 1u); i++) {
        mult *= 10u;                                            /* Configure the number of digits desired               */
    }

    while (mult > 0u) {                                         /* Perform conversion of string to decimal              */
       *s++    = value / mult + '0';
        value %= mult;
        mult  /= 10u;
    }
}

/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (1);
}
#endif

