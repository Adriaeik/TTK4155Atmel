/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"



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
	
	
	//SRAM_test();
	
	uint16_t addr = 420;
	uint8_t data = 69;
	

	//SRAM_write(addr, data);
	//
	//uint8_t rdata = SRAM_read(addr);
	//printf(rdata);
	//printf("\n\r");
	//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", 0, rdata, data);
		
	while (1) {
		SRAM_write(addr, data);
		
		uint8_t rdata = SRAM_read(addr);
		printf("%02X", rdata);
		printf("\n\r");
		printf("Write phase error: ext_ram[%4d] = %2d (should be %2d)\n\r", addr, rdata, data);
		_delay_ms(1000);
	}

	return 0;
}




