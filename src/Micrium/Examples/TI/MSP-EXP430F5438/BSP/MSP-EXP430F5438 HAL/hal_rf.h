/*******************************************************************************
 *
 *  hal_rf.h
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

#ifndef HAL_RF_H
#define HAL_RF_H

#define RF_PORT_OUT     P3OUT
#define RF_PORT_DIR     P3DIR
#define RF_PORT_SEL     P3SEL
#define RF_PORT_IN      P3IN
#define RF_SPI_SIMO     BIT1
#define RF_SPI_SOMI     BIT2
#define RF_SPI_CLK      BIT3
#define RF_SPI_CS       BIT0
typedef struct {
    unsigned char fsctrl1;     // Frequency synthesizer control.
    unsigned char fsctrl0;     // Frequency synthesizer control.
    unsigned char freq2;       // Frequency control word, high byte.
    unsigned char freq1;       // Frequency control word, middle byte.
    unsigned char freq0;       // Frequency control word, low byte.
    unsigned char mdmcfg4;     // Modem configuration.
    unsigned char mdmcfg3;     // Modem configuration.
    unsigned char mdmcfg2;     // Modem configuration.
    unsigned char mdmcfg1;     // Modem configuration.
    unsigned char mdmcfg0;     // Modem configuration.
    unsigned char channr;      // Channel number.
    unsigned char deviatn;     // Modem deviation setting (when FSK modulation is enabled).
    unsigned char frend1;      // Front end RX configuration.
    unsigned char frend0;      // Front end RX configuration.
    unsigned char mcsm0;       // Main Radio Control State Machine configuration.
    unsigned char foccfg;      // Frequency Offset Compensation Configuration.
    unsigned char bscfg;       // Bit synchronization Configuration.
    unsigned char agcctrl2;    // AGC control.
    unsigned char agcctrl1;    // AGC control.
    unsigned char agcctrl0;    // AGC control.
    unsigned char fscal3;      // Frequency synthesizer calibration.
    unsigned char fscal2;      // Frequency synthesizer calibration.
    unsigned char fscal1;      // Frequency synthesizer calibration.
    unsigned char fscal0;      // Frequency synthesizer calibration.
    unsigned char fstest;      // Frequency synthesizer calibration.
    unsigned char test2;       // Various test settings.
    unsigned char test1;       // Various test settings.
    unsigned char test0;       // Various test settings.
    unsigned char iocfg2;      // GDO2 output pin configuration.
    unsigned char iocfg0;      // GDO0 output pin configuration.
    unsigned char pktctrl1;    // Packet automation control.
    unsigned char pktctrl0;    // Packet automation control.
    unsigned char addr;        // Device address.
    unsigned char pktlen;      // Packet length.
} HAL_RF_CONFIG;

/*-------------------------------------------------------------
 *                  Function Prototypes
 * ------------------------------------------------------------*/
extern void halRfInit(void);
extern void halRfShutDown(void);
extern void halRfResetChip(void);
extern unsigned char halRfWriteReg(unsigned char address, unsigned char data);
extern unsigned char halRfReadStatus(unsigned char address);
extern unsigned char halRfGetChipId(void);
extern unsigned char halRfGetChipVer(void);
extern void halRfSetupChip(void);

#endif /* HAL_RF_H */
