/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"


int main(void) {
	/*-----------------------
	Setup start 
	----------------------*/
	// Initialiser UART med baudrate 9600
		//// Sett Port B pin 0 som utgang
	setBit(DDRB, PB0);
	UART_Init(MYUBBR);
	
	// Aktiver mottaksavbrot
	UART_EnableReceiveInterrupt();
	
	// Aktiver globale avbrot
	sei();
	/*-----------------------
	Setup end 
	----------------------*/
	
	while (1) {
		// Send ein testkarakter
		UART_SendChar('A');
		//Sett PB0 høg
		setBit(PORTB, PB0);
		// Legg til forsinkelse
		_delay_ms(scaleDelay(1000));
		//UART_ReceiveChar();
		//Sett PB0 låg
		clearBit(PORTB, PB0);
		_delay_ms(scaleDelay(500));  // Vent 500 ms
	}
	
	return 0;
}


//int main(void) {
	//// Sett Port B pin 0 som utgang
	////DDRB |= (1 << PB0);
	//setBit(DDRB, PB0);
//
	//while (1) {
		//// Sett PB0 høg
		////PORTB |= (1 << PB0);
		//setBit(PORTB, PB0);
		//_delay_ms(500);  // Vent 500 ms
//
		//// Sett PB0 låg
		////PORTB &= ~(1 << PB0);
		//clearBit(PORTB, PB0);
		//_delay_ms(500);  // Vent 500 ms
	//}
//
	//return 0;
//}
