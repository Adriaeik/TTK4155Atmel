/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

// Definer den globale variabelen for å telje millisekund
volatile uint32_t overflow_count1 = 0;
volatile uint32_t overflow_count3 = 0;

// Timer-overflyt interrupt service rutine
ISR(TIMER1_OVF_vect) {
	overflow_count1++;
}
ISR(TIMER3_OVF_vect) {
	overflow_count3++;
}


// Funksjon for å setje opp Timer1 til å generere 1 ms avbrot
void setup_timer() {
	/*______SCREEN TIMER______*/
	// Sett normal modus (WGM12 = 0)
	TCCR1A = 0;      // Normal mode
	TCCR1B = (1 << CS10); // Prescaler = 1 (ingen prescaling, f_CPU direkte)

	// Aktiver Timer1 overflow interrupt
	TIMSK |= (1 << TOIE1);

	// Nullstill Timer/Counter1
	TCNT1 = 0;
	
	
	/*______GENERAL TIMER______*/
	TCCR3A = 0;      // Normal mode
	TCCR3B = (1 << CS32) | (1 << CS30); // Prescaler = 1/1024

	// Aktiver Timer3 overflow interrupt
	TIMSK |= (1 << TOIE3);

	// Nullstill Timer/Counter1
	TCNT3 = 0;
	
}



// Funksjon som returnerer tida i millisekund sidan programstart
uint32_t screen_ms(void){
	return screen_cycles()/(4915200UL/1000);
}

uint32_t screen_cycles(void) {
	// Returner tiden i klokkesykluser
	// Hver gang timeren overflyter, har vi telt 65536 klokkesykluser
	return (overflow_count1 * 65536UL) + TCNT1;
}


void restart_screen_timer(){
	// Nullstill Timer/Counter1
	TCNT1 = 0;
	overflow_count1 = 0;
}



// Funksjon som returnerer tida i millisekund sidan programstart
uint32_t general_ms(void){
	return 1000*general_cycles()/(4800UL);
}

uint32_t general_cycles(void) {
	// Returner tiden i klokkesykluser
	// Hver gang timeren overflyter, har vi telt 65536 klokkesykluser
	return (overflow_count3 * 65536UL) + TCNT3;
}


void restart_general_timer(){
	// Nullstill Timer/Counter1
	TCNT3 = 0;
	overflow_count3 = 0;
}