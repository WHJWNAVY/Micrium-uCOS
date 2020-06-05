/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2014 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL * Embedded Transport Layer Security                    *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL version: V2.54a                                        *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller GmbH
Licensed to:              Silicon Laboratories Inc., 400 W. Cesar Chavez, Austin, TX 78701, USA
Licensed SEGGER software: emSSL
License number:           SSL-00158
License model:            OEM LICENSE DISTRIBUTION AGREEMENT, signed June 3rd, 2018 and June 5th, 2018
----------------------------------------------------------------------
File        : SEGGER_MEM_Conf.h
Purpose     : Configuration file for SEGGER memory allocators.
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_MEM_CONF_H
#define SEGGER_MEM_CONF_H

//
// Define SEGGER_MEM_DEBUG: Debug level for SSL product
//                  0: No checks                      (Smallest and fastest code)
//                  1: Warnings & Panic checks
//                  2: Warnings, logs, & panic checks (Bigger code)
//
#ifndef   DEBUG
  #define DEBUG            0
#endif

#if DEBUG
  #ifndef   SEGGER_MEM_DEBUG
    #define SEGGER_MEM_DEBUG      2      // Default for debug builds
  #endif
  #ifndef   SEGGER_MEM_STATS
    #define SEGGER_MEM_STATS      1      // Default for debug builds, include statistics
  #endif
#else
  #ifndef   SEGGER_MEM_DEBUG
    #define SEGGER_MEM_DEBUG      0      // Default for release builds
  #endif
  #ifndef   SEGGER_MEM_STATS
    #define SEGGER_MEM_STATS      0      // Default for release builds, don't include statistics
  #endif
#endif

#endif

/****** End Of File *************************************************/
