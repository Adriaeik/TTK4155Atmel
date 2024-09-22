/*
 * DriverMCP2515.c
 *
 * Created: 22.09.2024 13:56:38
 *  Author: eikel
 */ 
#include "DriverMCP2515.h"

// MCP2515 Reset-funksjon
void MCP2515_Reset(void) {
	clearBit(PORTB, PB4);
	MCP2515_SendCommand(MCP2515_RESET);  // Send RESET-kommandoen
	setBit(PORTB, PB4);
}

// MCP2515 Read-funksjon
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result;

	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ);         // Send READ-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil lese fra
	result = SPI_Receive();             // Les resultatet fra MCP2515
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)

	return result;
}

// MCP2515 Write-funksjon
void MCP2515_Write(uint8_t address, uint8_t data) {
	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_WRITE);        // Send WRITE-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil skrive til
	SPI_Transmit(data);                 // Skriv dataen til MCP2515
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)
}

// MCP2515 Request to Send-funksjon
void MCP2515_RequestToSend(uint8_t txBuffers) {
	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_RTS_TX0 | txBuffers);  // Send RTS-kommandoen med buffer-parameter
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)
}

// MCP2515 Read Status-funksjon
uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;

	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_READ_STATUS);  // Send READ STATUS-kommandoen
	status = SPI_Receive();             // Les status
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)

	return status;
}

// MCP2515 Bit Modify-funksjon
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(MCP2515_BIT_MODIFY);   // Send BIT MODIFY-kommandoen
	SPI_Transmit(address);              // Send adressen vi vil modifisere
	SPI_Transmit(mask);                 // Send bitmasken
	SPI_Transmit(data);                 // Skriv nye bit-verdier
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)
}

// MCP2515 Send Command-funksjon
void MCP2515_SendCommand(uint8_t command) {
	clearBit(PORTB, PB4);               // Aktiver slave (CS lav)
	SPI_Transmit(command);              // Send kommandoen til MCP2515
	setBit(PORTB, PB4);                 // Deaktiver slave (CS h�y)
}

// MCP2515 Set Mode-funksjon
void MCP2515_SetMode(uint8_t mode) {
	MCP2515_BitModify(MCP2515_CANCTRL, 0xE0, mode);  // Sett mode-bits (bits 5-7) i CANCTRL-registeret
}
