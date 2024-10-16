/*
 * driver_IR.c
 *
 * Created: 16.10.2024 14:32:18
 *  Author: adriaeik
 */ 

#include "sam.h"
#include "../include/driver_IR.h"

/*
atsam | arduino
PA2   = A7
PA3   = A6
PA4   = A5
*/

// AD7 = CH7 på ADC mode

void IR_Init(){
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
	

}

uint16_t IR_Read(){
	// Returner ADC-data frå kanal 7 (PA2 / A7)
	return ADC->ADC_CDR[0];
}