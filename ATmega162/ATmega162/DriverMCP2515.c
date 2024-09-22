/*
 * DriverMCP2515.c
 *
 * Created: 22.09.2024 13:56:38
 *  Author: eikel
 */ 
#include "DriverMCP2515.h"

// Reset MCP2515
void MCP2515_Reset(void) {
	MCP2515_SendCommand(MCP2515_RESET);  // Send RESET-kommandoen
}

// Les fra MCP2515 på gitt adresse
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result;

	MCP2515_SendCommand(MCP2515_READ);  // Send READ-kommandoen
	MCP2515_SendCommand(address);  // Send adressen vi vil lese fra
	result = SPI_Receive();  // Les resultatet fra MCP2515
	
	return result;
}
void MCP2515_SendCommand(uint8_t command){
	clearBit(PORTB, PB4);
	SPI_Transmit(command);  // Send WRITE-kommandoen
	setBit(PORTB, PB4);
}
// Skriv til MCP2515 på gitt adresse
void MCP2515_Write(uint8_t address, uint8_t data) {
	MCP2515_SendCommand(MCP2515_WRITE);  // Send WRITE-kommandoen
	MCP2515_SendCommand(address);  // Send adressen vi vil skrive til
	MCP2515_SendCommand(data);  // Skriv dataen til MCP2515
}

// Be MCP2515 om å sende via buffer
void MCP2515_RequestToSend(uint8_t txBuffers) {
	MCP2515_SendCommand(MCP2515_RTS_TX0 | txBuffers);  // Send RTS-kommandoen
}

// Les status fra MCP2515
uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;
	
	MCP2515_SendCommand(MCP2515_READ_STATUS);  // Send READ STATUS-kommandoen
	status = SPI_Receive();  // Les status
	return status;
}

// Endre spesifikke bit på MCP2515
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data) {
	MCP2515_SendCommand(MCP2515_BIT_MODIFY);  // Send BIT MODIFY-kommandoen
	MCP2515_SendCommand(address);  // Send adressen
	MCP2515_SendCommand(mask);  // Send bitmask
	MCP2515_SendCommand(data);  // Skriv nye bit-verdier
}
