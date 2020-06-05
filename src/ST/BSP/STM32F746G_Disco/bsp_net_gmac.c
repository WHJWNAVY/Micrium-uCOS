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
*                            NETWORK BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*                                         STM32F746G-DISCO
*
* Filename : bsp_net_gmac.c
* Version  : V3.04.05
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <IF/net_if_ether.h>

#include  "bsp_clk.h"
#include  "bsp_int.h"

#include  <os.h>
#include  <stm32f7xx_hal.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                  NETWORK DEVICE INTERFACE NUMBERS
*********************************************************************************************************
*/

static  NET_IF_NBR  STM32F746G_DISCO_GMAC_0_IF_Nbr = NET_IF_NBR_NONE;


/*
*********************************************************************************************************
*                               NETWORK DEVICE BSP FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef  NET_IF_ETHER_MODULE_EN

static  void        NetDev_CfgClk          (NET_IF   *p_if,
                                            NET_ERR  *p_err);

static  void        NetDev_CfgIntCtrl      (NET_IF   *p_if,
                                            NET_ERR  *p_err);

static  void        NetDev_CfgGPIO_RMII    (NET_IF   *p_if,
                                            NET_ERR  *p_err);

static  CPU_INT32U  NetDev_ClkFreqGet      (NET_IF   *p_if,
                                            NET_ERR  *p_err);

static  void        NetDev_GMAC_ISR_Handler(void);


/*
*********************************************************************************************************
*                                    NETWORK DEVICE BSP INTERFACE
*
* Note(s) : (1) Device board-support package (BSP) interface structures are used by the device driver to
*               call specific devices' BSP functions via function pointer instead of by name.  This
*               enables the network protocol suite to compile & operate with multiple instances of
*               multiple devices & drivers.
*
*           (2) In most cases, the BSP interface structure provided below SHOULD suffice for most devices'
*               BSP functions exactly as is with the exception that BSP interface structures' names MUST
*               be unique & SHOULD clearly identify the development board, device name, & possibly the
*               specific device number (if the development board supports multiple instances of any given
*               device).
*********************************************************************************************************
*/

const  NET_DEV_BSP_ETHER  NetDev_BSP_STM32F746G_DISCO_0 = {     /* Board-/device-specific BSP fnct ptrs : */
    &NetDev_CfgClk,                                             /*   Cfg clk(s)                           */
    &NetDev_CfgIntCtrl,                                         /*   Cfg int ctrl(s)                      */
    &NetDev_CfgGPIO_RMII,                                       /*   Cfg GPIO                             */
    &NetDev_ClkFreqGet                                          /*   Get clk freq                         */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                              NETWORK ETHERNET DEVICE DRIVER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           NetDev_CfgClk()
*
* Description : Configure clocks for the specified interface/device
*
* Argument(s) : p_if     Pointer to network interface to configure
*               ----     Argument validated in NetIF_Add()
*
*               p_err    Pointer to variable that will receive the return error code from this function:
*
*                           NET_DEV_ERR_NONE     Device clock(s)     successfully configured
*                           NET_DEV_ERR_FAULT    Device clock(s) NOT successfully configured
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_CfgClk (NET_IF   *p_if,
                             NET_ERR  *p_err)
{
    NET_PHY_CFG_ETHER  *p_phy_cfg;


    p_phy_cfg = p_if->Ext_Cfg;                                  /* Obtain pointer to Phy cfg.                           */
    if (p_phy_cfg == (NET_PHY_CFG_ETHER *)0u) {
       *p_err = NET_DEV_ERR_FAULT;
        return;
    }

    __HAL_RCC_ETHMAC_CLK_ENABLE();
    __HAL_RCC_ETHMACTX_CLK_ENABLE();
    __HAL_RCC_ETHMACRX_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    SYSCFG->PMC &= ~(SYSCFG_PMC_MII_RMII_SEL);
    SYSCFG->PMC |= ETH_MEDIA_INTERFACE_RMII;                    /* Select Ethernet Media Interface (RMII).              */

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                         NetDev_CfgIntCtrl()
*
* Description : Configure interrupts &/or interrupt controller for the specified interface/device
*
* Argument(s) : p_if     Pointer to network interface to configure
*               ----     Argument validated in NetIF_Add()
*
*               p_err    Pointer to variable that will receive the return error code from this function:
*
*                           NET_DEV_ERR_NONE     Device interrupt(s)     successfully configured
*                           NET_DEV_ERR_FAULT    Device interrupt(s) NOT successfully configured
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static  void  NetDev_CfgIntCtrl (NET_IF   *p_if,
                                 NET_ERR  *p_err)
{
    STM32F746G_DISCO_GMAC_0_IF_Nbr = p_if->Nbr;                 /* Cfg this device's BSP instance with specific IF nbr. */

    BSP_IntVectSet(INT_ID_ETH,
                   CPU_CFG_KA_IPL_BOUNDARY,                     /* Make sure we are within the Kernel Aware boundary.   */
                   CPU_INT_KA,
                   NetDev_GMAC_ISR_Handler);

    BSP_IntEnable(INT_ID_ETH);                                  /* Enable Ethernet Interrupt request.                   */

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                       NetDev_CfgGPIO_RMII()
*
* Description : Configure general-purpose I/O (GPIO) for the specified interface/device
*
* Argument(s) : p_if     Pointer to network interface to configure
*               ----     Argument validated in NetIF_Add()
*
*               p_err    Pointer to variable that will receive the return error code from this function:
*
*                           NET_DEV_ERR_NONE     Device GPIO     successfully configured
*                           NET_DEV_ERR_FAULT    Device GPIO NOT successfully configured
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static  void  NetDev_CfgGPIO_RMII (NET_IF   *p_if,
                                   NET_ERR  *p_err)
{
    NET_PHY_CFG_ETHER  *p_phy_cfg;
    GPIO_InitTypeDef    gpio_init;


    p_phy_cfg = p_if->Ext_Cfg;                                  /* Obtain pointer to Phy cfg.                           */
    if (p_phy_cfg == (NET_PHY_CFG_ETHER *)0u) {
       *p_err = NET_DEV_ERR_INVALID_CFG;
        return;
    }

                                                                /* ------------------ PORT A PINS --------------------- */
                                                                /* Configure as alternative function.                   */
    gpio_init.Pin       = GPIO_PIN_1 |                          /* ETH_RMII_REF_CLK (PA1)                               */
                          GPIO_PIN_2 |                          /* ETH_RMII_MDIO    (PA2)                               */
                          GPIO_PIN_7;                           /* ETH_RMII_CRS_DV  (PA7)                               */
    gpio_init.Speed     = GPIO_SPEED_HIGH;
    gpio_init.Mode      = GPIO_MODE_AF_PP;
    gpio_init.Pull      = GPIO_NOPULL;
    gpio_init.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &gpio_init);

                                                                /* ------------------ PORT G PINS --------------------- */
                                                                /* Configure as alternative function.                   */
    gpio_init.Pin =  GPIO_PIN_2  |                              /* RMII_MII_RXER  (PG2)                                 */
                     GPIO_PIN_11 |                              /* RMII_MII_TX_EN (PG11)                                */
                     GPIO_PIN_13 |                              /* ETH_RMII_TXD0  (PG13)                                */
                     GPIO_PIN_14;                               /* ETH_RMII_TXD1  (PG14)                                */
    HAL_GPIO_Init(GPIOG, &gpio_init);

                                                                /* ------------------ PORT C PINS --------------------- */
                                                                /* Configure as alternative function.                   */
    gpio_init.Pin = GPIO_PIN_1 |                                /* ETH_RMII_MDC (PC1)                                   */
                    GPIO_PIN_4 |                                /* ETH_RMII_RXD0 (PC4)                                  */
                    GPIO_PIN_5;                                 /* ETH_RMII_RXD0 (PC5)                                  */
    HAL_GPIO_Init(GPIOC, &gpio_init);

   *p_err = NET_DEV_ERR_NONE;
}


/*
*********************************************************************************************************
*                                         NetDev_ClkFreqGet()
*
* Description : Get device clock frequency
*
* Argument(s) : p_if     Pointer to network interface to get clock frequency
*               ----     Argument validated in NetIF_Add()
*
*               p_err    Pointer to variable that will receive the return error code from this function:
*
*                            NET_DEV_ERR_NONE     Device clock frequency successfully returned
*                            NET_DEV_ERR_FAULT    Device clock frequency unsuccessfully returned
*
* Return(s)   : Device clock frequency (in Hz)
*
* Note(s)     : none
*********************************************************************************************************
*/

static  CPU_INT32U  NetDev_ClkFreqGet (NET_IF   *p_if,
                                       NET_ERR  *p_err)
{
    CPU_INT32U  clk_freq;


   (void)p_if;                                                  /* Prevent 'variable unused' compiler warning.          */

    *p_err   = NET_DEV_ERR_NONE;
    clk_freq = BSP_ClkFreqGet(CLK_ID_HCLK);

    if (clk_freq == 0u) {
        *p_err = NET_DEV_ERR_FAULT;
    }

    return (clk_freq);
}


/*
*********************************************************************************************************
*                                        NetDev_GMAC_ISR_Handler()
*
* Description : BSP-level ISR handler(s) for device interrupts
*
* Argument(s) : none
*
* Return(s)   : none
*
*
* Note(s)     : none
*********************************************************************************************************
*/

static void  NetDev_GMAC_ISR_Handler (void)
{
    NET_ERR  err;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    OSIntEnter();                                               /* Tell OS that we are starting an ISR                  */
    CPU_CRITICAL_EXIT();

    NetIF_ISR_Handler(STM32F746G_DISCO_GMAC_0_IF_Nbr, NET_DEV_ISR_TYPE_UNKNOWN, &err);

    OSIntExit();                                                /* Tell OS that we are leaving the ISR                  */

   (void)err;
}


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/

#endif                                                          /* NET_IF_ETHER_MODULE_EN                               */
