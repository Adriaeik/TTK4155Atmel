/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

// Definer den globale variabelen for å telje millisekund
volatile uint32_t overflow_count1 = 0;


// Funksjon for å setje opp Timer1 til å generere 1 ms avbrot
void setup_timer() {
	/*______SCREEN TIMER______*/
	// Set Timer1 til normal-modus //CTC-modus
	TCCR1A = 0; // (1 << WGM12);

	// Set verdien for sammenligning, f.eks. 15624 for 1 Hz med 16 MHz klokke
	//Fiks denne til riktig Hz seneree 569 funker bra
	//OCR1A = 569;


	// -> trenger ikke mer Aktiver interrupt for sammenligning A
	//TIMSK |= (1 << OCIE1A);

	// Velg prescaler 0
	TCCR1B |= (1 << CS10);
	//.... 1024
	//TCCR1B |= (1 << CS12) | (1 << CS10);

	//enable interupt overflow for timer 1
	TIMSK |= (1 << TOIE1);
	// Nullstill telleren
	TCNT1 = 0;	
}

uint8_t count_digits(uint16_t number) {
	int digits = 0;
	if (number == 0) {
		return 1;
	}
	while (number > 0) {
		number /= 10;
		digits++;
	}
	return digits;
}

void number_to_chars(uint16_t number, char* buffer, uint8_t length) {
	buffer[length] = '\0';  // Null-terminator for å avslutte strengen

	// Fyll buffer fra høyre til venstre
	for (int i = length - 1; i >= 0; i--) {
		buffer[i] = (number % 10) + '0';  // Konverter siste siffer til ASCII-tegn
		number /= 10;  // Fjern det siste sifferet
	}
}