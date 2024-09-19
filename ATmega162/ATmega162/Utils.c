/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

// Definer den globale variabelen for å telje millisekund
volatile uint32_t overflow_count = 0;

// Timer-overflyt interrupt service rutine
ISR(TIMER1_OVF_vect) {
	overflow_count++;
}

// Funksjon som returnerer tida i millisekund sidan programstart
uint32_t get_time_in_ms(void){
	return get_time_in_cycles()/(4915200UL/1000);
}

uint32_t get_time_in_cycles(void) {
	// Returner tiden i klokkesykluser
	// Hver gang timeren overflyter, har vi telt 65536 klokkesykluser
	return (overflow_count * 65536UL) + TCNT1;
}

// Funksjon for å setje opp Timer1 til å generere 1 ms avbrot
void setup_timer() {
	// Sett normal modus (WGM12 = 0)
	TCCR1A = 0;      // Normal mode
	TCCR1B = (1 << CS10); // Prescaler = 1 (ingen prescaling, f_CPU direkte)

	// Aktiver Timer1 overflow interrupt
	TIMSK = (1 << TOIE1);

	// Nullstill Timer/Counter1
	TCNT1 = 0;
}
void restart_timer(){
	// Nullstill Timer/Counter1
	TCNT1 = 0;
	overflow_count = 0;
}
