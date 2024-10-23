/*
 * encoder.c
 *
 * Created: 23.10.2024 09:11:59
 *  Author: adriaeik
 */ 
#include "../include/encoder.h"

void encoder_init(void) {
	// Aktivere klokka for TC2
	PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32));  // Aktivere klokke for TC2 Channel 0 (TC6)

	// Sette TC2 i quadrature decoder mode ved å skrive til TC_BMR
	TC2->TC_BMR = TC_BMR_QDEN   // Aktivere Quadrature Decoder Mode
	| TC_BMR_POSEN  // Aktivere posisjonsmåling
	| TC_BMR_EDGPHA // Bruke overgangar for dekoding
	| TC_BMR_MAXFILT(2);  // Angi filter for støyreduksjon (valfri verdi)

	// Velje klokka for TC2, eks. interne klokke (f.eks. TIMER_CLOCK1)
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK2;

	// Start Timer Counter Channel 0
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
uint32_t read_encoder_position(void) {
	//if  (TC2->TC_CHANNEL[0].TC_CV < 0) {
		//encoder_init();
	//}
	return TC2->TC_CHANNEL[0].TC_CV;  // Les posisjonsverdi
}

