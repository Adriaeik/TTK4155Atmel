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
	setBit(DDR_SPI, DD_SS);  // SS som output
	setBit(DDR_SPI, DD_MOSI);  // MOSI
	setBit(DDR_SPI, DD_SCK);  // SCK
	clearBit(DDR_SPI, DD_MISO); // MISO
	
	// Aktiver SPI, sett som master, clock rate fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	// clearBit(DDR_SPI, DD_SS);
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
	//SPI_Transmit(0x00);  // Send dummy byte for � lese svar
	//return SPDR;  // Returner mottatt byte
	//
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF))) {
	}
	return SPDR;
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

void SPI_DselectSlave() {
	PORTB |= (1 << DD_SS);
}

void SPI_SelctSlave() {
	PORTB &= ~(1 << DD_SS);
}