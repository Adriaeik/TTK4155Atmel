/*
 * SRAM_Test.c
 *
 * Created: 04.09.2024 18:23:23
 *  Author: adriaeik
 */ 
#include "SRAM.h"

// Funksjon for � initialisere det eksterne minnet (SRAM + latch)
void externalMemoryInit(void) {
	// Set Port A (AD0-AD7) og Port C (A8-A15) som utgang for adresse- og databuss
	DDRA = 0xFF;  // PA0-PA7 som utgang
	DDRC = 0xFF;  // PC0-PC7 som utgang

	// Set Port D (PD6 og PD7) som utgang for WR og RD signal
	setBit(DDRD, PD6);  // WR
	setBit(DDRD, PD7);  // RD

	// Set Port E (PE1) som utgang for ALE
	setBit(DDRE, PE1);  // ALE

	// Aktivere ekstern minnegrensesnitt
	setBit(MCUCR, SRE);   // Enable external SRAM interface
	SFIOR = 0x00;         // No wait state
}

// Funksjon for � skrive data til SRAM
void SRAM_write(uint16_t addr, uint8_t data) {
	// Plassere adresse p� AD0-AD7 (lavbyte) og A8-A15 (h�gbyte)
	PORTA = (uint8_t)(addr & 0xFF);  // AD0-AD7
	PORTC = (uint8_t)((addr >> 8) & 0xFF);  // A8-A15

	// Aktivere adresse latch (ALE signal)
	setBit(PORTE, PE1);  // ALE h�g
	_delay_us(1);  // Kort forsinkelse for � sikre at adressa blir fanga opp
	clearBit(PORTE, PE1);  // ALE l�g

	// Plassere data p� AD0-AD7
	PORTA = data;

	// Skrive data til SRAM (/WR signal)
	clearBit(PORTD, PD6);  // WR l�g
	_delay_us(1);  // Kort forsinkelse for � sikre korrekt skriving
	setBit(PORTD, PD6);   // WR h�g
}

// Funksjon for � lese data fr� SRAM
uint8_t SRAM_read(uint16_t addr) {
	uint8_t data;

	// Plassere adresse p� AD0-AD7 (lavbyte) og A8-A15 (h�gbyte)
	PORTA = (uint8_t)(addr & 0xFF);  // AD0-AD7
	PORTC = (uint8_t)((addr >> 8) & 0xFF);  // A8-A15

	// Aktivere adresse latch (ALE signal)
	setBit(PORTE, PE1);  // ALE h�g
	_delay_us(1);  // Kort forsinkelse for � sikre at adressa blir fanga opp
	clearBit(PORTE, PE1);  // ALE l�g

	// Aktivere lesing fr� SRAM (/RD signal)
	clearBit(PORTD, PD7);  // RD l�g
	_delay_us(1);  // Kort forsinkelse for � sikre korrekt lesing
	data = PINA;  // Les data fr� AD0-AD7
	setBit(PORTD, PD7);   // RD h�g

	return data;
}
void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1800; // Startadresse for SRAM
	uint16_t ext_ram_size = 0x800; // St�rrelsen p� SRAM (2 KB)
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	
	printf("Starting SRAM test...\n");
	
	uint16_t seed = rand(); // Genererer eit fr� for random-tall
	
	// Skrivefasen: Skriv og sjekk verdien umiddelbart
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand(); // Genererer eit tilfeldig verdi
		ext_ram[i] = some_value;
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			write_errors++;
		}
	}
	
	// Hente-fasen: Sjekk at ingen verdier har blitt endra etter skrivefasen
	srand(seed); // Resetter PRNG for � f� same sekvens av random tall
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}
	
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n", write_errors, retrieval_errors);
}
