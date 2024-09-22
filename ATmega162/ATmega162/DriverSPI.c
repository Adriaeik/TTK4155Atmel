/*
 * DriverSPI.c
 *
 * Created: 22.09.2024 13:37:30
 *  Author: eikel
 */ 

#include "DriverSPI.h"
// Initialiser SPI som master
void SPI_MasterInit(void) {
	// Sett MOSI og SCK som output, MISO som input
	setBit(DDRB, PB5);  // MOSI
	setBit(DDRB, PB7);  // SCK
	clearBit(DDRB, PB6); // MISO
	setBit(DDRB, PB4);  // SS som output
	//KOK
	// DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);

	// Aktiver SPI, sett som master, clock rate fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	///KOK
	// SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
	
	clearBit(PORTB, PB4);
}

// Sender byte via SPI
void SPI_Transmit(char data) {
	SPDR = data;  // Send data
	loopUntilBitIsSet(SPSR, SPIF);  // Vent til overf�ring er ferdig
}
/*
KOK
void spi_write(char cData) {
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))) {
	}
}
*/

// Mottar byte via SPI
uint8_t SPI_Receive(void) {
	SPI_Transmit(0x00);  // Send dummy byte for � lese svar
	return SPDR;  // Returner mottatt byte
}

/*
KOK
uint8_t spi_read() {
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF))) {
	}
	return SPDR;
}
*/