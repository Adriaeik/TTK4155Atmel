/*
 * XMEM_Decode.c
 *
 * Created: 11.09.2024 16:27:54
 *  Author: ravneb
 */ 
#include "XMEM_Decode.h"
// Funksjon for � initialisere det eksterne minnet (SRAM + latch)
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

// Funksjon for � skrive data til SRAM
void SRAM_write(volatile uint16_t addr, uint8_t data) {
	volatile char *ext_ram = (char *) SRAM_START; // Startadresse for SRAM
	uint16_t ext_ram_size = SRAM_SIZE; // St�rrelsen p� SRAM (2 KB)
	ext_ram[addr] = data;
}

// Funksjon for � lese data fr� SRAM
uint8_t SRAM_read(volatile uint16_t addr) {
	volatile char *ext_ram = (char *) SRAM_START; // Startadresse for SRAM
	uint16_t ext_ram_size = SRAM_SIZE; // St�rrelsen p� SRAM (2 KB)
	uint8_t data = ext_ram[addr];
	
	return data;
}

// Funksjon for � skrive data til SRAM
void Universal_write(volatile uint16_t addr, uint8_t data) {
	volatile char *ext_ram = (char *) OLED_START; // Startadresse for SRAM
	ext_ram[addr] = data;
		
}

// Funksjon for � lese data fr� SRAM
uint8_t Universal_read(volatile uint16_t addr) {
	volatile char *ext_ram = (char *) OLED_START; // Startadresse for SRAM
	uint8_t data = ext_ram[addr];
	
	return data;
}
