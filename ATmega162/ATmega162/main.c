/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include <avr/io.h>
#include <util/delay.h>




int main(void) {
	// Sett Port B pin 0 som utgang
	DDRB |= (1 << PB0);

	int a = 3;

	while (1) {
		// Sett PB0 høg
		PORTB |= (1 << PB0);
		_delay_ms(500);  // Vent 500 ms

		// Sett PB0 låg
		PORTB &= ~(1 << PB0);
		_delay_ms(500);  // Vent 500 ms
	}

	return 0;
}

