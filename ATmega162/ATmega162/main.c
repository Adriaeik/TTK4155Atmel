/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"



int main(void) {
	// Initialiser eksternt minne
	externalMemoryInit();

	// Skrive data til SRAM
	SRAM_write(0x1800, 0xAB);  // Skriv 0xAB til adresse 0x1800
	SRAM_write(0x1801, 0xCD);  // Skriv 0xCD til adresse 0x1801

	//// Lese data frå SRAM
	//uint8_t data1 = SRAM_read(0x1800);
	//uint8_t data2 = SRAM_read(0x1801);

	while (1) {
		// Skriv 0x01 til adresse 0x1800, som vil sette PA0 høg og slå på LED kobla til PA0
		SRAM_write(0x1800, 0x01);
		_delay_ms(500);  // Vent 500 ms

		// Skriv 0x02 til adresse 0x1800, som vil sette PA1 høg og slå på LED kobla til PA1
		SRAM_write(0x1800, 0x02);
		_delay_ms(500);  // Vent 500 ms

		// Skriv 0x00 til adresse 0x1800, som vil slå av begge LED-ar
		SRAM_write(0x1800, 0x00);
		_delay_ms(500);  // Vent 500 ms

		// Skriv 0x03 til adresse 0x1800, som vil sette både PA0 og PA1 høg og slå på begge LED-ar
		SRAM_write(0x1800, 0x03);
		_delay_ms(500);  // Vent 500 ms
	}

	return 0;
}




