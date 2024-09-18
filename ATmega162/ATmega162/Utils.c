/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

// Definer den globale variabelen for å telje millisekund
volatile uint32_t millis_counter = 0;  // Initialiser tellaren

// Avbrothandterar for Timer1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	millis_counter++;  // Auk tellaren for kvart millisekund
}

// Funksjon som returnerer tida i millisekund sidan programstart
uint32_t millis() {
	return millis_counter;
}

// Funksjon for å setje opp Timer1 til å generere 1 ms avbrot
void setup_timer() {
	// Set opp CTC-modus med prescaler på 64
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);

	// Set verdien som Timer1 skal samanlikne med for å generere avbrot kvar 1 ms
	OCR1A = 85;  // 1 ms avbrot (F_CPU / (prescaler * (1 + OCR1A)))

	// Aktiver Output Compare A Match Interrupt Enable for Timer1
	TIMSK |= (1 << OCIE1A);  // Aktiver avbrot

	// Aktiver globale avbrot
	sei();
}
