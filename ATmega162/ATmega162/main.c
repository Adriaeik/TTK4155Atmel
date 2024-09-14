/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
#include "XMEM_Decode.h"
#include "SRAM.h"


int main(void) {
	
	// Initialiser UART med baudrate 9600
	UART_Init(MYUBBR);
	
	// Initialiser stdio til å bruke UART
	URAT_initStudio();
	// Aktiver mottaksavbrot
	UART_EnableReceiveInterrupt();
	
	// Aktiver globale avbrot
	sei();
	
	
	//// Initialiser eksternt minne
	
	externalMemoryInit();
	
	
	SRAM_test();
	
	//uint16_t addr = 1 + ADC_START;
	uint16_t addr = 1 + ADC_START;

	uint8_t data = 0x80;
	uint8_t rdata = 69;
	
	// Opprett MultiBoard-struktur for å lagre status
	MultiBoard board;
	    
	// Initialiser MultiBoard og kalibrer joystickens origo
	MultiBoard_Init(&board);
	
	// Hovudløkka
	while (1) {
		// Oppdater MultiBoard verdier (skyvebrytere, joystick, og knapper)
		MultiBoard_Update(&board);

		// Skrive ut alle verdier til UART (for debugging eller logging)
		printf("Left Slider: %u, Right Slider: %u\n", board.LSpos, board.RSpos);
		printf("Joystick X: %d, Y: %d, Button: %u\n", board.JoyXpos, board.JoyYpos, board.JoyButton);
		printf("Left Button: %u, Right Button: %u\n\n", board.LB, board.RB);
	
		// Legg inn ei lita forsinkelse for å redusere oppdateringsfrekvensen
		_delay_ms(500);  // Vent i 500 ms før neste oppdatering
	}
	//while (1) {
		//Universal_write(addr, data);
		//rdata = Universal_read(addr);
		//printf("Reading from: ext_ram[%4d] = %2d \n\r", addr, rdata);
//
	//}

	return 0;
}




