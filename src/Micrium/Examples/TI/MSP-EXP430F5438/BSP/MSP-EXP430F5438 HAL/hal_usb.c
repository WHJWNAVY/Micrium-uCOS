/*******************************************************************************
 *
 *  hal_usb.c
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

char halUsbReceiveBuffer[255];
unsigned char bufferSize = 0;

/**********************************************************************//**
* @brief  Initializes the serial communications peripheral and GPIO ports
*         to communicate with the TUSB3410.
*
* @param  none
*
* @return none
**************************************************************************/

void halUsbInit(void)
{
    unsigned char i;

    for (i = 0; i < 255; i++)
        halUsbReceiveBuffer[i] = '\0';

    bufferSize = 0;
    USB_PORT_SEL |= USB_PIN_RXD + USB_PIN_TXD;
    USB_PORT_DIR |= USB_PIN_TXD;
    USB_PORT_DIR &= ~USB_PIN_RXD;

    UCA1CTL1 |= UCSWRST;                    //Reset State
    UCA1CTL0 = UCMODE_0;

    UCA1CTL0 &= ~UC7BIT;                    // 8bit char
    UCA1CTL1 |= UCSSEL_2;
    UCA1BR0 = 16;                           // 8Mhz/57600=138
    UCA1BR1 = 1;
    UCA1MCTL = 0xE;
    UCA1CTL1 &= ~UCSWRST;
    UCA1IE |= UCRXIE;

    __bis_SR_register(GIE);                 // Enable Interrupts
}

/**********************************************************************//**
* @brief  Disables the serial communications peripheral and clears the GPIO
*         settings used to communicate with the TUSB3410.
*
* @param  none
*
* @return none
**************************************************************************/

void halUsbShutDown(void)
{
    UCA1IE &= ~UCRXIE;
    UCA1CTL1 = UCSWRST;                     //Reset State
    USB_PORT_SEL &= ~(USB_PIN_RXD + USB_PIN_TXD);
    USB_PORT_DIR |= USB_PIN_TXD;
    USB_PORT_DIR |= USB_PIN_RXD;
    USB_PORT_OUT &= ~(USB_PIN_TXD + USB_PIN_RXD);
}

/**********************************************************************//**
* @brief  Sends a character over UART to the TUSB3410.
*
* @param  character The character to be sent.
*
* @return none
**************************************************************************/

void halUsbSendChar(char character)
{
    while (!(UCA1IFG & UCTXIFG)) ;
    UCA1TXBUF = character;
}

/**********************************************************************//**
* @brief  Sends a string of characters to the TUSB3410
*
* @param  string[] The array of characters to be transmit to the TUSB3410.
*
* @param  length   The length of the string.
*
* @return none
**************************************************************************/

void halUsbSendString(char string[], unsigned char length)
{
    unsigned char i;

    for (i = 0; i < length; i++)
        halUsbSendChar(string[i]);
}

/************************************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    halUsbReceiveBuffer[bufferSize++] = UCA1RXBUF;
    __bic_SR_register_on_exit(LPM3_bits);
}

