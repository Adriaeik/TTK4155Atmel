/*
 * DriverMCP2515.h
 *
 * Created: 22.09.2024 13:55:51
 *  Author: eikel
 */ 


#ifndef DRIVER_MCP2515_H_
#define DRIVER_MCP2515_H_

#include <avr/io.h>
#include "DriverSPI.h"

// MCP2515-instruksjoner
#define MCP2515_RESET        0xC0
#define MCP2515_READ_         0x03
#define MCP2515_WRITE        0x02
#define MCP2515_RTS_TX0      0x81
#define MCP2515_RTS_TX1      0x82
#define MCP2515_RTS_TX2      0x84
#define MCP2515_READ_STATUS  0xA0
#define MCP2515_BIT_MODIFY   0x05

#define MCP2515_LOAD_TX0	0x40
#define MCP2515_LOAD_TX1	0x42
#define MCP2515_LOAD_TX2	0x44

#define MCP2515_RTS_TX0		0x81
#define MCP2515_RTS_TX1		0x82
#define MCP2515_RTS_TX2		0x84
#define MCP2515_RTS_ALL		0x87

#define MCP2515_READ_RX0	0x90
#define MCP2515_READ_RX1	0x94

#define MCP2515_READ_STATUS	0xA0

#define MCP2515_RX_STATUS	0xB0

#define MCP2515_RESET		0xC0


// CANCTRL Register Values
#define MCP2515_MODE_NORMAL      0x00  // Normal Mode
#define MCP2515_MODE_SLEEP       0x20  // Sleep Mode
#define MCP2515_MODE_LOOPBACK    0x40  // Loopback Mode
#define MCP2515_MODE_LISTENONLY  0x60  // Listen-Only Mode
#define MCP2515_MODE_CONFIG      0x80  // Configuration Mode
#define MCP2515_MODE_POWERUP	0xE0
#define MCP2515_MODE_MASK		0xE0


#define MCP2515_ABORT_TX        0x10
#define MCP2515_MODE_ONESHOT	0x08
#define MCP2515_CLKOUT_ENABLE	0x04
#define MCP2515_CLKOUT_DISABLE	0x00
#define MCP2515_CLKOUT_PS1		0x00
#define MCP2515_CLKOUT_PS2		0x01
#define MCP2515_CLKOUT_PS4		0x02
#define MCP2515_CLKOUT_PS8		0x03

// Send-buffer 0
#define MCP2515_TXB0SIDH 0x31
#define MCP2515_TXB0SIDL 0x32
#define MCP2515_TXB0DLC 0x35
#define MCP2515_TXB0D0 0x36

//Send-buffer 1
#define MCP2515_TXB1SIDH 0x41
#define MCP2515_TXB1SIDL 0x42
#define MCP2515_TXB1DLC 0x45
#define MCP2515_TXB1D0 0x46

//Send-buffer 2
#define MCP2515_TXB2SIDH 0x51
#define MCP2515_TXB2SIDL 0x52
#define MCP2515_TXB2DLC 0x55
#define MCP2515_TXB2D0 0x56

// Motta-buffer 0
#define MCP2515_RXB0SIDH 0x61
#define MCP2515_RXB0SIDL 0x62
#define MCP2515_RXB0DLC 0x65
#define MCP2515_RXB0D0 0x66

// Motta-buffer 1
#define MCP2515_RXB1SIDH 0x71
#define MCP2515_RXB1SIDL 0x72
#define MCP2515_RXB1DLC 0x75
#define MCP2515_RXB1D0 0x76

// CNF1 Register Values

#define MCP2515_SJW1            0x00
#define MCP2515_SJW2            0x40
#define MCP2515_SJW3            0x80
#define MCP2515_SJW4            0xC0


// CNF2 Register Values

#define MCP2515_BTLMODE			0x80
#define MCP2515_SAMPLE_1X       0x00
#define MCP2515_SAMPLE_3X       0x40


// CNF3 Register Values

#define MCP2515_SOF_ENABLE		0x80
#define MCP2515_SOF_DISABLE		0x00
#define MCP2515_WAKFIL_ENABLE	0x40
#define MCP2515_WAKFIL_DISABLE	0x00


// CANINTF Register Bits

#define MCP2515_RX0IF		0x01
#define MCP2515_RX1IF		0x02
#define MCP2515_TX0IF		0x04
#define MCP2515_TX1IF		0x08
#define MCP2515_TX2IF		0x10
#define MCP2515_ERRIF		0x20
#define MCP2515_WAKIF		0x40
#define MCP2515_MERRF		0x80

// Define MCP2515 register addresses

#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F
#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC			0x1C
#define MCP2515_REC			0x1D
#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D
#define MCP2515_TXB0CTRL	0x30
#define MCP2515_TXB1CTRL	0x40
#define MCP2515_TXB2CTRL	0x50
#define MCP2515_RXB0CTRL	0x60
#define MCP2515_RXB0SIDH	0x61
#define MCP2515_RXB1CTRL	0x70
#define MCP2515_RXB1SIDH	0x71


#define MCP2515_TX_INT		0x1C		// Enable all transmit interrupts
#define MCP2515_TX01_INT	0x0C		// Enable TXB0 and TXB1 interrupts
#define MCP2515_RX_INT		0x03		// Enable receive interrupts
#define MCP2515_NO_INT		0x00		// Disable all interrupts

#define MCP2515_TX01_MASK	0x14
#define MCP2515_TX_MASK		0x54

// MCP2515-funksjoner
void MCP2515_init();
void MCP2515_Reset(void);
uint8_t MCP2515_Read(uint8_t address);
void MCP2515_Write(uint8_t address, uint8_t data);
void MCP2515_RequestToSend(uint8_t txBuffers);
uint8_t MCP2515_ReadStatus(void);
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_SendCommand(uint8_t command);
void MCP2515_SetMode(uint8_t mode);

/*___________FRA BB____________*/
/*
mcp2515.h

This file contains constants that are specific to the MCP2515.

Version     Date        Description
----------------------------------------------------------------------
v1.00       2003/12/11  Initial release

Copyright 2003 Kimberly Otten Software Consulting
*/




#endif /* DRIVER_MCP2515_H_ */
