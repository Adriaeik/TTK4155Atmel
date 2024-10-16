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
	// Aktivere klokka for PIOA og ADC
	PMC->PMC_PCER0 |= (1 << ID_PIOA);  // Aktivere klokka for PIOA
	PMC->PMC_PCER1 |= (1 << (ID_ADC - 32));  // Aktivere klokka for ADC
	

	// Deaktiver PIO kontroll over PA2 (A7)
	PIOA->PIO_PDR |= PIO_PA2;  // Disable PIO control over PA2 (A7)
	
	// Konfigurer ADC-en til å bruke kanal 7 (PA2)
	ADC->ADC_CHER = ADC_CHER_CH7;  // Enable ADC kanal 7

	// Sett ADC-parametere (prescaler, startup, osv.)
	ADC->ADC_MR = ~(1 << ADC_MR_TRGEN_DIS)        // Ingen trigger, manuell start
	| ADC_MR_PRESCAL(10)      // Sett prescaler for ADC-klokka
	| ADC_MR_STARTUP_SUT64    // Start-up time
	| ADC_MR_SETTLING_AST3    // Settling time for analog inngang
	| ADC_MR_TRACKTIM(2)      // Tracking time
	| ADC_MR_TRANSFER(2);     // Conversion til transfer
	

}

uint16_t IR_Read(){
	// Start konvertering
	ADC->ADC_CR = ADC_CR_START;

	// Vent til konvertering er ferdig
	while (!(ADC->ADC_ISR & ADC_ISR_EOC7));  // Vent på end of conversion for kanal 7

	// Returner ADC-data frå kanal 7 (PA2 / A7)
	return ADC->ADC_CDR[7];
}