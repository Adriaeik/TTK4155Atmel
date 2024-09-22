/*
 * DriverMCP2515.c
 *
 * Created: 22.09.2024 13:56:38
 *  Author: eikel
 */ 
#include "DriverMCP2515.h"

// Reset MCP2515
void MCP2515_Reset(void) {
	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_RESET);  // Send RESET-kommandoen
	setBit(PORTB, PB2);  // Sett SS høy igjen
}

// Les fra MCP2515 på gitt adresse
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result;

	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_READ);  // Send READ-kommandoen
	SPI_Transmit(address);  // Send adressen vi vil lese fra
	result = SPI_Receive();  // Les resultatet fra MCP2515
	setBit(PORTB, PB2);  // Sett SS høy igjen

	return result;
}

// Skriv til MCP2515 på gitt adresse
void MCP2515_Write(uint8_t address, uint8_t data) {
	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_WRITE);  // Send WRITE-kommandoen
	SPI_Transmit(address);  // Send adressen vi vil skrive til
	SPI_Transmit(data);  // Skriv dataen til MCP2515
	setBit(PORTB, PB2);  // Sett SS høy igjen
}

// Be MCP2515 om å sende via buffer
void MCP2515_RequestToSend(uint8_t txBuffers) {
	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_RTS_TX0 | txBuffers);  // Send RTS-kommandoen
	setBit(PORTB, PB2);  // Sett SS høy igjen
}

// Les status fra MCP2515
uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;

	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_READ_STATUS);  // Send READ STATUS-kommandoen
	status = SPI_Receive();  // Les status
	setBit(PORTB, PB2);  // Sett SS høy igjen

	return status;
}

// Endre spesifikke bit på MCP2515
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
	clearBit(PORTB, PB2);  // Velg MCP2515 ved å sette SS lav
	SPI_Transmit(MCP2515_BIT_MODIFY);  // Send BIT MODIFY-kommandoen
	SPI_Transmit(address);  // Send adressen
	SPI_Transmit(mask);  // Send bitmask
	SPI_Transmit(data);  // Skriv nye bit-verdier
	setBit(PORTB, PB2);  // Sett SS høy igjen
}
