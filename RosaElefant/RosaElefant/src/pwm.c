/*
 * CFile1.c
 *
 * Created: 02/10/2024 12:03:40
 *  Author: Ravn
 */ 

#include "../include/pwm.h"


#define MCK 84000000
#define PV 50
#define WPKEY 0x50494F

#define PERIOD_A    20E-3
#define DIVA        84
#define CLK_A       1E6
#define CPRDA       (int) (PERIOD_A * MCK / DIVA)


//P value = MCK/(PRESCALER*PV)

void pwm_init(){
	
	//setter pin PB13 til � bruke Peripheral B-funksjonaliteten (i dette tilfellet PWM)
	PIOB->PIO_ABSR |= PIO_ABSR_P13; 
	//Deaktiverer vanlig PIO-kontroll av pinnen slik at periferien (PWM) kan overta kontrollen.
	PIOB->PIO_PDR |= PIO_PDR_P13;
	
	// PMC_PCR aktiverer periferiklokken for PWM. Klokken settes til � bruke masterklokken (MCK), og ID_PWM er ID-en til PWM-modulen (36). Dette gj�r at PWM-enheten f�r str�m og kan operere.
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
	// Aktiverer klokken for PWM-modulen ved � sette riktig bit i PMC
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
	
	
	//Enable channel 1. Dette gj�r at kanal 1 begynner � generere et PWM-signal
	PWM->PWM_ENA |= PWM_ENA_CHID1; 
	
	// Setter opp PWM-klokke A til en frekvens p� 1 MHz ved � dividere MCK (84 MHz) med DIVA (84). PREB for klokke B settes til 0 (ikke relevant her siden vi kun bruker klokke A).
	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
	
	// Konfigurerer PWM-kanal 1 til � bruke klokke A som kilde
	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;
	
	// Setter periodeverdien (CPRD) til CPRDA, som representerer en periode p� 20 ms (50 Hz).
	PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRDA);	
}