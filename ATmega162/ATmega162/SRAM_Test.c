/*
 * SRAM_Test.c
 *
 * Created: 04.09.2024 18:23:23
 *  Author: adriaeik
 */ 
#include "SRAM.h"

// Funksjon for å initialisere det eksterne minnet (SRAM + latch)
void externalMemoryInit(void) {
	// Set Port A (AD0-AD7) og Port C (A8-A15) som utgang for adresse- og databuss
	DDRA = 0xFF;  // PA0-PA7 som utgang
	DDRC = 0x0F ;  // PC0-PC3 som utgang
 
	// Set Port D (PD6 og PD7) som utgang for WR og RD signal
	setBit(DDRD, PD6);  // WR
	setBit(DDRD, PD7);  // RD

	// Set Port E (PE1) som utgang for ALE
	setBit(DDRE, PE1);  // ALE

	// Aktivere ekstern minnegrensesnitt
	setBit(MCUCR, SRE);   // Enable external SRAM interface
	SFIOR = 0x00;         // No wait state
	
	//Maskes pc4-pc7 (disables as output) (s32 ATmega datasheet)
	setBit(SFIOR, XMM2);
}

// Funksjon for å skrive data til SRAM
void SRAM_write(volatile uint16_t addr, uint8_t data) {
	volatile char *ext_ram = (char *) SRAM_START; // Startadresse for SRAM
	uint16_t ext_ram_size = SRAM_SIZE; // Størrelsen på SRAM (2 KB)	
	ext_ram[addr] = data;
	
	
}

// Funksjon for å lese data frå SRAM
uint8_t SRAM_read(volatile uint16_t addr) {
	volatile char *ext_ram = (char *) SRAM_START; // Startadresse for SRAM
	uint16_t ext_ram_size = SRAM_SIZE; // Størrelsen på SRAM (2 KB)
	uint8_t data = ext_ram[addr];
	
	return data;
}
void SRAM_test(void) {
	volatile char *ext_ram = (char *) SRAM_START; // Startadresse for SRAM
	uint16_t ext_ram_size = SRAM_SIZE; // Størrelsen på SRAM (2 KB)
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	
	printf("Starting SRAM test...\n");
	
	uint16_t seed = rand(); // Genererer eit frø for random-tall
	
	// Skrivefasen: Skriv og sjekk verdien umiddelbart
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand(); // Genererer eit tilfeldig verdi
		ext_ram[i] = some_value;
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retrieved_value, some_value);
			write_errors++;
		}
	}
	
	// Hente-fasen: Sjekk at ingen verdier har blitt endra etter skrivefasen
	srand(seed); // Resetter PRNG for å få same sekvens av random tall
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}
	
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\r", write_errors, retrieval_errors);
}
