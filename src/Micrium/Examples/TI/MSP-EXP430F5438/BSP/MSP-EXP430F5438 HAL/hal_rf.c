/*******************************************************************************
 *
 *  hal_rf.c
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
#include "cc2500.h"

const HAL_RF_CONFIG myRfConfig = {
    0x0B,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x5D,   // FREQ2     Frequency control word, high byte.
    0x93,   // FREQ1     Frequency control word, middle byte.
    0xB1,   // FREQ0     Frequency control word, low byte.
    0x86,   // MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
    0x83,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x44,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x03,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0xA9,   // FSCAL3    Frequency synthesizer calibration.
    0x0A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x0B,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for
            // detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet length.
};

const HAL_RF_CONFIG* rfConfig;

/**********************************************************************//**
* @brief  Initializes the ports and serial communication peripherals to
*         communicate with the radio.
*
* @param  none
*
* @return none
**************************************************************************/

void halRfInit(void)
{
    // Set up pins used by peripheral unit
    RF_PORT_SEL |= RF_SPI_SIMO + RF_SPI_SOMI + RF_SPI_CLK;
    RF_PORT_DIR |= RF_SPI_CS;

    UCB0CTL1 = UCSWRST;

    // 8-bit SPI Master 3-pin mode, with SMCLK as clock source
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST;
    UCB0CTL1 |= UCSSEL_2;

    // Ignore clockrate argument for now, just use clock source/2
    UCB0BR0 = 0x00;
    UCB0BR1 = 0x00;

    // Release for operation
    UCB0CTL1 &= ~UCSWRST;
    UCB0IFG &= ~UCRXIFG;
}

/**********************************************************************//**
* @brief  Disables the serial communications peripheral and clears the GPIO
*         settings used to communicate with the radio.
*
* @param  none
*
* @return none
**************************************************************************/

void halRfShutDown(void)
{
    UCB0CTL1 |= UCSWRST;
    RF_PORT_SEL &= ~(RF_SPI_SIMO + RF_SPI_SOMI + RF_SPI_CLK);
    RF_PORT_OUT &= ~RF_SPI_CS;
}

/**********************************************************************//**
* @brief  Performs a manual resets the CC2500 radio device.
*
* @param  none
*
* @return none
**************************************************************************/

void halRfResetChip(void)
{
    RF_PORT_OUT |= RF_SPI_CS;               // Start CSn high
    // The following CSn strobe must >= 40 us
    RF_PORT_OUT &= ~RF_SPI_CS;              // Set CSn low

    __delay_cycles(180);                    // Hold low

    RF_PORT_OUT |= RF_SPI_CS;               // Set CSn high

    __delay_cycles(540);                    // Hold high

    RF_PORT_OUT &= ~RF_SPI_CS;

    while (RF_PORT_IN & RF_SPI_SOMI) ;      // Wait for SO to go low again
                                            // Chip is now in IDLE state

    UCB0TXBUF = 0x30;                       // Send reset command

    while (UCB0STAT & UCBUSY) ;
    while (RF_PORT_IN & RF_SPI_SOMI) ;

    RF_PORT_OUT &= ~RF_SPI_CS;
}

/**********************************************************************//**
* @brief  Writes the data to the radio register at address.
*
* @param  address     Radio register address to which to write the SPI data
*
* @param  data        Radio register setting to be written
*
* @return receiveChar Dummy receive character for SPI communication
**************************************************************************/

unsigned char halRfWriteReg(unsigned char address, unsigned char data)
{
    unsigned char receiveChar;

    RF_PORT_OUT &= ~RF_SPI_CS;
    while (RF_PORT_IN & RF_SPI_SOMI) ;

    UCB0TXBUF = address;

    while (UCB0STAT & UCBUSY) ;
    receiveChar  = UCB0RXBUF;
    UCB0TXBUF    = data;

    while (!(UCB0IFG & UCTXIFG)) ;
    while (UCB0STAT & UCBUSY) ;
    RF_PORT_OUT |=  RF_SPI_CS;

    return receiveChar;
}

/**********************************************************************//**
* @brief  Reads a register from the radio.
*
* @param  address     The address from which to read the RF register
*
* @return receiveChar The status byte that is read from the radio
**************************************************************************/

unsigned char halRfReadStatus(unsigned char address)
{
    unsigned char receiveChar;

    RF_PORT_OUT &= ~RF_SPI_CS;
    while (RF_PORT_IN & RF_SPI_SOMI) ;

    UCB0TXBUF =  address;
    while (!(UCB0IFG & UCTXIFG)) ;

    while (!(UCB0IFG & UCRXIFG)) ;
    receiveChar = UCB0RXBUF;

    UCB0TXBUF = 0;
    while (!(UCB0IFG & UCTXIFG)) ;

    while (!(UCB0IFG & UCRXIFG)) ;
    receiveChar = UCB0RXBUF;
    RF_PORT_OUT |= RF_SPI_CS;

    return receiveChar;
}

/**********************************************************************//**
* @brief  Get function for the RF chip ID
*
* @param  none
*
* @return The RF chip ID
**************************************************************************/

unsigned char halRfGetChipId(void)
{
    return halRfReadStatus(CC2500_PARTNUM | CC2500_READ_BURST);
}

/**********************************************************************//**
* @brief   Get function for the RF chip version.
*
* @param   none
*
* @return  The RF chip version.
**************************************************************************/

unsigned char halRfGetChipVer(void)
{
    return halRfReadStatus(CC2500_VERSION  | CC2500_READ_BURST);
}

/**********************************************************************//**
* @brief   Writes the RF register settings to the radio.
*
* @param   none
*
* @return  none
**************************************************************************/

void halRfSetupChip()
{
    halRfWriteReg(CC2500_FSCTRL1,  rfConfig->fsctrl1);  // Frequency synthesizer control.
    halRfWriteReg(CC2500_FSCTRL0,  rfConfig->fsctrl0);  // Frequency synthesizer control.
    halRfWriteReg(CC2500_FREQ2,    rfConfig->freq2);    // Frequency control word, high byte.
    halRfWriteReg(CC2500_FREQ1,    rfConfig->freq1);    // Frequency control word, middle byte.
    halRfWriteReg(CC2500_FREQ0,    rfConfig->freq0);    // Frequency control word, low byte.
    halRfWriteReg(CC2500_MDMCFG4,  rfConfig->mdmcfg4);  // Modem configuration.
    halRfWriteReg(CC2500_MDMCFG3,  rfConfig->mdmcfg3);  // Modem configuration.
    halRfWriteReg(CC2500_MDMCFG2,  rfConfig->mdmcfg2);  // Modem configuration.
    halRfWriteReg(CC2500_MDMCFG1,  rfConfig->mdmcfg1);  // Modem configuration.
    halRfWriteReg(CC2500_MDMCFG0,  rfConfig->mdmcfg0);  // Modem configuration.
    halRfWriteReg(CC2500_CHANNR,   rfConfig->channr);   // Channel number.
    halRfWriteReg(CC2500_DEVIATN,  rfConfig->deviatn);  // Modem deviation setting (when FSK
                                                        // modulation is enabled).
    halRfWriteReg(CC2500_FREND1,   rfConfig->frend1);   // Front end RX configuration.
    halRfWriteReg(CC2500_FREND0,   rfConfig->frend0);   // Front end RX configuration.
    halRfWriteReg(CC2500_MCSM0,    rfConfig->mcsm0);    // Main Radio Control State Machine
                                                        // configuration.
    halRfWriteReg(CC2500_FOCCFG,   rfConfig->foccfg);   // Frequency Offset Compensation
                                                        // Configuration.
    halRfWriteReg(CC2500_BSCFG,    rfConfig->bscfg);    // Bit synchronization Configuration.
    halRfWriteReg(CC2500_AGCCTRL2, rfConfig->agcctrl2); // AGC control.
    halRfWriteReg(CC2500_AGCCTRL1, rfConfig->agcctrl1); // AGC control.
    halRfWriteReg(CC2500_AGCCTRL0, rfConfig->agcctrl0); // AGC control.
    halRfWriteReg(CC2500_FSCAL3,   rfConfig->fscal3);   // Frequency synthesizer calibration.
    halRfWriteReg(CC2500_FSCAL2,   rfConfig->fscal2);   // Frequency synthesizer calibration.
    halRfWriteReg(CC2500_FSCAL1,   rfConfig->fscal1);   // Frequency synthesizer calibration.
    halRfWriteReg(CC2500_FSCAL0,   rfConfig->fscal0);   // Frequency synthesizer calibration.
    halRfWriteReg(CC2500_FSTEST,   rfConfig->fstest);   // Frequency synthesizer calibration.
    halRfWriteReg(CC2500_TEST2,    rfConfig->test2);    // Various test settings.
    halRfWriteReg(CC2500_TEST1,    rfConfig->test1);    // Various test settings.
    halRfWriteReg(CC2500_TEST0,    rfConfig->test0);    // Various test settings.
    halRfWriteReg(CC2500_IOCFG2,   rfConfig->iocfg2);   // GDO2 output pin configuration.
    halRfWriteReg(CC2500_IOCFG0,   rfConfig->iocfg0);   // GDO0 output pin configuration.
    halRfWriteReg(CC2500_PKTCTRL1, rfConfig->pktctrl1); // Packet automation control.
    halRfWriteReg(CC2500_PKTCTRL0, rfConfig->pktctrl0); // Packet automation control.
    halRfWriteReg(CC2500_ADDR,     rfConfig->addr);     // Device address.
    halRfWriteReg(CC2500_PKTLEN,   rfConfig->pktlen);   // Packet length.
    halRfWriteReg(CC2500_PATABLE | CC2500_WRITE_BURST, 0xFB);
}

