/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"



int main(void) {
	//// Initialiser eksternt minne
	externalMemoryInit();
	SRAM_test();
		
	while (1) {
		
	}

	return 0;
}




