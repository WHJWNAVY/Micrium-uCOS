/*******************************************************************************
 *
 *  hal_board.c
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include "msp430.h"
#include "hal_MSP-EXP430F5438.h"

/**********************************************************************//**
 * @brief  Initializes ACLK, MCLK, SMCLK outputs on P11.0, P11.1,
 *         and P11.2, respectively.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halBoardOutputSystemClock(void) //outputs clock to testpoints
{
    CLK_PORT_DIR |= 0x07;
    CLK_PORT_SEL |= 0x07;
}

/**********************************************************************//**
 * @brief  Stops the output of ACLK, MCLK, SMCLK on P11.0, P11.1, and P11.2.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halBoardStopOutputSystemClock(void)
{
    CLK_PORT_OUT &= ~0x07;
    CLK_PORT_DIR |= 0x07;
    CLK_PORT_SEL &= ~0x07;
}

/**********************************************************************//**
 * @brief  Initializes all GPIO configurations.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void halBoardInit(void)
{
    //Tie unused ports
    PAOUT  = 0;
    PADIR  = 0xFFFF;
    PASEL  = 0;
    PBOUT  = 0;
    PBDIR  = 0xFFFF;
    PBSEL  = 0;
    PCOUT  = 0;
    PCDIR  = 0xFFFF;
    PCSEL  = 0;
    PDOUT  = 0;
    PDDIR  = 0xFFFF;
    PDSEL  = 0x0003;
    PEOUT  = 0;
    PEDIR  = 0xFEFF;                        // P10.0 to USB RST pin,
                                            // ...if enabled with J5
    PESEL  = 0;
    P11OUT = 0;
    P11DIR = 0xFF;
    PJOUT  = 0;
    PJDIR  = 0xFF;
    P11SEL = 0;

    AUDIO_PORT_OUT = AUDIO_OUT_PWR_PIN;
    USB_PORT_DIR &= ~USB_PIN_RXD;           // USB RX Pin, Input with
                                            // ...pulled down Resistor
    USB_PORT_OUT &= ~USB_PIN_RXD;
    USB_PORT_REN |= USB_PIN_RXD;
}

