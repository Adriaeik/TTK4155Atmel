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
#define MCP2515_READ         0x03
#define MCP2515_WRITE        0x02
#define MCP2515_RTS_TX0      0x81
#define MCP2515_READ_STATUS  0xA0
#define MCP2515_BIT_MODIFY   0x05

#define MCP2515_MODE_NORMAL      0x00  // Normal Mode
#define MCP2515_MODE_SLEEP       0x20  // Sleep Mode
#define MCP2515_MODE_LOOPBACK    0x40  // Loopback Mode
#define MCP2515_MODE_LISTENONLY  0x60  // Listen-Only Mode
#define MCP2515_MODE_CONFIG      0x80  // Configuration Mode
#define MCP2515_CANCTRL  0x0F  // CANCTRL-registeradresse


// MCP2515-funksjoner
void MCP2515_Reset(void);
uint8_t MCP2515_Read(uint8_t address);
void MCP2515_Write(uint8_t address, uint8_t data);
void MCP2515_RequestToSend(uint8_t txBuffers);
uint8_t MCP2515_ReadStatus(void);
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_SendCommand(uint8_t command);
void MCP2515_SetMode(uint8_t mode);

#endif /* DRIVER_MCP2515_H_ */
