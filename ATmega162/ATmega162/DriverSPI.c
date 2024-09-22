/*
 * DriverSPI.c
 *
 * Created: 22.09.2024 13:37:30
 *  Author: eikel
 */ 

#include "DriverSPI.h"

void SPI_MasterInit(void) {
	// Sett MOSI (PB5), SCK (PB7), og SS (PB4) som utgangar
	setBit(DDRB, PB4); // SS
	setBit(DDRB, PB5); // MOSI
	setBit(DDRB, PB7); // SCK
	
	// Sett MISO (PB6) som inngang
	clearBit(DDRB, PB6);

	// Aktiver SPI, sett opp SPI i master-modus, klokkhastighet fosc/16
	setBit(SPCR, SPE);   // Aktivere SPI
	setBit(SPCR, MSTR);  // Master-modus
	setBit(SPCR, SPR0);  // fosc/16 for SPI-klokkehastighet
	
	// Deselect MCP2515 ved å sette SS høgt
	setBit(PORTB, PB4);
}

uint8_t SPI_MasterTransmit(uint8_t data) {
	// Last data inn i SPDR-registeret
	SPDR = data;
	
	// Vent til overføringa er ferdig (SPIF sett i SPSR når ferdig)
	loopUntilBitIsSet(SPSR, SPIF);
	
	// Returner data mottatt frå slave
	return SPDR;
}

void SPI_SelectSlave(void) {
	// Velje MCP2515 ved å sette SS lågt
	clearBit(PORTB, PB4);
}

void SPI_DeselectSlave(void) {
	// Frigjere MCP2515 ved å sette SS høgt
	setBit(PORTB, PB4);
}

void MCP2515_SendCommand(uint8_t command) {
	SPI_SelectSlave();               // Velje MCP2515 som slave
	SPI_MasterTransmit(command);     // Send kommando
	SPI_DeselectSlave();             // Frigjere MCP2515
}

uint8_t MCP2515_ReadStatus(void) {
	uint8_t status;
	SPI_SelectSlave();               // Velje MCP2515
	SPI_MasterTransmit(0xA0);        // Kommando for å lese status
	status = SPI_MasterTransmit(0x00);  // Les status frå MCP2515
	SPI_DeselectSlave();             // Frigjere MCP2515
	
	return status;
}
