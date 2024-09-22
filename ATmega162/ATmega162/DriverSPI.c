/*
 * DriverSPI.c
 *
 * Created: 22.09.2024 13:37:30
 *  Author: eikel
 */ 

#include "DriverSPI.h"
// Initialiser SPI som master
void SPI_Init(void) {
	// Sett MOSI og SCK som output, MISO som input
	setBit(DDRB, PB5);  // MOSI
	setBit(DDRB, PB7);  // SCK
	clearBit(DDRB, PB6); // MISO
	setBit(DDRB, PB4);  // SS som output

	// Aktiver SPI, sett som master, clock rate fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

// Sender byte via SPI
void SPI_Transmit(uint8_t data) {
	SPDR = data;  // Send data
	loopUntilBitIsSet(SPSR, SPIF);  // Vent til overføring er ferdig
}

// Mottar byte via SPI
uint8_t SPI_Receive(void) {
	SPI_Transmit(0xFF);  // Send dummy byte for å lese svar
	return SPDR;  // Returner mottatt byte
}