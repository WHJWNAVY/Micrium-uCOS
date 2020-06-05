/*
*********************************************************************************************************
*                                             uC/USB-Host
*                                     The Embedded USB Host Stack
*
*                         (c) Copyright 2008-2018; Silicon Laboratories Inc.,
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
*                        USB HOST DRIVER BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*                                          STM32F746G-DISCO
*
* Filename : bsp_usbh.c
* Version  : V3.41.05
*********************************************************************************************************
*/

/*
**************************************************************************************************************
*                                            INCLUDE FILES
**************************************************************************************************************
*/

#include  <os.h>
#include  <cpu.h>
#include  <lib_def.h>
#include  <usbh_cfg.h>
#include  <stm32f7xx_hal.h>

#include  "bsp_usbh.h"
#include  "bsp_int.h"


/*
**************************************************************************************************************
*                                            LOCAL DEFINES
**************************************************************************************************************
*/


/*
**************************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
**************************************************************************************************************
*/

static  USBH_HC_DRV   *USBH_HC_OTG_HS_DrvPtr;

static  CPU_FNCT_PTR   BSP_USBH_OTG_HS_ISR_Ptr;


/*
**************************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
**************************************************************************************************************
*/

static  void  BSP_USBH_HS_Init          (USBH_HC_DRV   *p_drv,
                                         USBH_ERR      *p_err);

static  void  BSP_USBH_HS_ISR_Register  (CPU_FNCT_PTR   isr_fnct,
                                         USBH_ERR      *p_err);

static  void  BSP_USBH_HS_ISR_Unregister(USBH_ERR      *p_err);

static  void  BSP_USBH_OTG_HS_IntHandler(void);


/*
*********************************************************************************************************
*                                    USB HOST DRIVER BSP INTERFACE
*********************************************************************************************************
*/

USBH_HC_BSP_API  USBH_DrvBSP_STM32F746G_DISCO = {
    BSP_USBH_HS_Init,
    BSP_USBH_HS_ISR_Register,
    BSP_USBH_HS_ISR_Unregister
};


/*
**************************************************************************************************************
**************************************************************************************************************
*                                           LOCAL FUNCTION
**************************************************************************************************************
**************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BSP_USBH_HS_Init()
*
* Description : This function performs board specific initialization of USB host controller.
*
* Argument(s) : p_drv    Pointer to host controller driver structure.
*
*               p_err    Pointer to variable that will receive the return error code from this function
*
*                            USBH_ERR_NONE    BSP init successfull.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_USBH_HS_Init (USBH_HC_DRV  *p_drv,
                                USBH_ERR     *p_err)
{
    GPIO_InitTypeDef  GPIO_Init;


    USBH_HC_OTG_HS_DrvPtr = p_drv;

                                                                /* --------- CONFIGURE USB OTG-HS ULPI GPIO'S --------- */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
                                                                /* ------------------- ULPI_CK (PA5) ------------------ */
    GPIO_Init.Pin       = GPIO_PIN_5;
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Speed     = GPIO_SPEED_HIGH;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);
                                                                /* ------------------ ULPI_D0 (PA3) ------------------- */
    GPIO_Init.Pin       = GPIO_PIN_3;
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Speed     = GPIO_SPEED_HIGH;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);
                                                                /* ------------------- ULPI_D[1:7] -------------------- */
    GPIO_Init.Pin       = GPIO_PIN_0  |                         /* ULPI_D1 (PB0)                                        */
                          GPIO_PIN_1  |                         /* ULPI_D2 (PB1)                                        */
                          GPIO_PIN_5  |                         /* ULPI_D7 (PB5)                                        */
                          GPIO_PIN_10 |                         /* ULPI_D3 (PB10)                                       */
                          GPIO_PIN_11 |                         /* ULPI_D4 (PB11)                                       */
                          GPIO_PIN_12 |                         /* ULPI_D5 (PB12)                                       */
                          GPIO_PIN_13;                          /* ULPI_D6 (PB13)                                       */
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);
                                                                /* ------------------ ULPI_STP (PC0) ------------------ */
    GPIO_Init.Pin       = GPIO_PIN_0;
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_Init);
                                                                /* ------------------ ULPI_NXT (PH4) ------------------ */
    GPIO_Init.Pin       = GPIO_PIN_4;
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_Init);
                                                                /* ------------------ ULPI_DIR (PC2) ------------------ */
    GPIO_Init.Pin       = GPIO_PIN_2;
    GPIO_Init.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init.Pull      = GPIO_NOPULL;
    GPIO_Init.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_Init);

    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();                          /* Enable USB OTG-HS clocks.                            */

    *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                     BSP_USBH_HS_ISR_Register()
*
* Description : Registers Interrupt Service Routine.
*
* Argument(s) : isr_fnct    Host controller ISR address.
*
*               p_err       Pointer to variable that will receive the return error code from this function
*
*                               USBH_ERR_NONE    ISR registered successfully.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_USBH_HS_ISR_Register (CPU_FNCT_PTR   isr_fnct,
                                        USBH_ERR      *p_err)
{
    BSP_USBH_OTG_HS_ISR_Ptr = isr_fnct;

    BSP_IntVectSet(INT_ID_OTG_HS,                               /* Set Kernel Aware Interrupt                           */
                   CPU_CFG_KA_IPL_BOUNDARY,                     /* Make sure we are within the Kernel Aware boundary.   */
                   CPU_INT_KA,
                   BSP_USBH_OTG_HS_IntHandler);

    BSP_IntEnable(INT_ID_OTG_HS);

    *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                    BSP_USBH_HS_ISR_Unregister()
*
* Description : Unregisters Interrupt Service Routine.
*
* Argument(s) : p_err    Pointer to variable that will receive the return error code from this function
*
*                            USBH_ERR_NONE    ISR unregistered successfully.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_USBH_HS_ISR_Unregister (USBH_ERR  *p_err)
{
    BSP_IntDisable(INT_ID_OTG_HS);

    BSP_USBH_OTG_HS_ISR_Ptr = (CPU_FNCT_PTR)0;

   *p_err = USBH_ERR_NONE;
}


/*
*********************************************************************************************************
*                                    BSP_USBH_OTG_HS_IntHandler()
*
* Description : USB host interrupt handler.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_USBH_OTG_HS_IntHandler (void)
{
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    OSIntEnter();                                               /* Tell OS that we are starting an ISR           */
    CPU_CRITICAL_EXIT();

    if (BSP_USBH_OTG_HS_ISR_Ptr != (CPU_FNCT_PTR)0) {
        BSP_USBH_OTG_HS_ISR_Ptr((void *)USBH_HC_OTG_HS_DrvPtr);
    }

    OSIntExit();                                                /* Tell OS that we are leaving the ISR           */
}


/*
**************************************************************************************************************
*                                                  END
**************************************************************************************************************
*/
