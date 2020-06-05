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
* Filename : bsp_usbh.h
* Version  : V3.41.05
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This USB host driver board-specific function header file is protected from multiple
*               pre-processor inclusion through use of the USB host configuration module present pre-
*               processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_USBH_PRESENT                                       /* See Note #1.                                         */
#define  BSP_USBH_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <HCD/DWC_OTG_HS/usbh_hcd_dwc_otg_hs.h>
#include  <Source/usbh_core.h>


/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                                   /* See Note #1.                                         */
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  USBH_HC_BSP_API  USBH_DrvBSP_STM32F746G_DISCO;


/*
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef __cplusplus
}                                                               /* End of 'extern'al C lang linkage.                    */
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
