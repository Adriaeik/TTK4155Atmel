/*
 * SRAM_Test.c
 *
 * Created: 04.09.2024 18:23:23
 *  Author: adriaeik
 */ 
#include "SRAM.h"

void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1800; // Startadresse for SRAM
	uint16_t ext_ram_size = 0x0800; // Størrelsen på SRAM (2 KB)
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
