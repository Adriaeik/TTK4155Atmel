/*
 * driver_IR.c
 *
 * Created: 16.10.2024 14:32:18
 *  Author: adriaeik
 */ 

#include "sam.h"
#include "../include/driver_IR.h"
#include "../include/time.h"
uint16_t score;
/*
atsam | arduino
PA2   = A7
PA3   = A6
PA4   = A5
*/

// AD7 = CH7 på ADC mode

/*
NB!!!!
Schematics bladet til arduino due lyver!! veldig kult
ADC channel 7 er A0 og ADC channel 0 er A7. vet ikke hva resten av ADC kanalene er, men må testes
*/
void IR_Init(){
	uint16_t threshold_value = 500;
	ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;
	// Aktivere klokka for PIOA og ADC
	PMC->PMC_PCER1 |= (1 << (ID_ADC - 32));  // Aktivere klokka for ADC
	

	// Deaktiver PIO kontroll over PA2 (A7)
	PIOA->PIO_PDR |= PIO_PA2;  // Disable PIO control over PA2 (A7)
	

	// Sett ADC-parametere (prescaler, startup, osv.)
	ADC->ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(255);
	// Konfigurer ADC-en til å bruke kanal 7 (PA2)
	ADC->ADC_CHER = ADC_CHER_CH0;  // Enable ADC kanal 7
	
	// Start konvertering
	ADC->ADC_CR = ADC_CR_START;
	
	
	ADC->ADC_CWR = ADC_CWR_LOWTHRES(threshold_value);
	ADC->ADC_EMR = ADC_EMR_CMPMODE_LOW;  // Trigge når signalet går under terskelverdi
	ADC->ADC_IER = ADC_IER_COMPE;  // Enable interrupt for comparison event
	

}

uint16_t IR_Read(){
	// Returner ADC-data frå kanal 7 (PA2 / A7)
	return ADC->ADC_CDR[0];
}

#include "../include/game.h"
uint8_t IR_l = 0;
Game main_game;

void IR_Handler() {
	// Sjekk om ADC har registrert sammenligningshendinga (lav verdi)
	if (ADC->ADC_ISR & ADC_ISR_COMPE) {
		// Om linja har gått frå høg til låg (det er her vi legg til poeng)
		if (IR_l == 0) {
			main_game.score++;   // Legg til eit poeng
			IR_l++;  // Oppdater tilstanden til lav (linja er brutt)
			//time_spinFor(msecs(10));
		}
		} else {
		// Om linja har gått frå låg til høg (linja ikkje lenger brutt)
		if (IR_l == 1) {
			IR_l = 0;  // Nullstill flagget for å kunne registrere neste negative flanke
		}
	}

	// Slette interrupt-flagget ved å lese ADC_ISR for å fjerne COMPE-flagget
	volatile uint32_t dummy = ADC->ADC_ISR; // Dette leser og sletter flagget
}
