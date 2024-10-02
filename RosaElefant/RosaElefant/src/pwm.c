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
	// enabe wpenable
	//PIOB->PIO_WPMR_WPKEY = PIO_WPMR_WPKEY(WPKEY);
	//PIOB->PIO_WPMR_WPEN = 0;
	
	
	//Enable peripheral B at PB13
	PIOB->PIO_ABSR |= PIO_ABSR_P13; 
	//Disable PIO from controlling pin13
	PIOB->PIO_PDR |= PIO_PDR_P13;
	
	// enable clock for PWM:    DIV = 0 (clk = MCK), CMD = 0 (read), PID = 36 (PWM)
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
	
	// set PWM clock A to 84 MHz
	PWM->PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVA(DIVA);
	
	
	//Enable channel 1?
	PWM->PWM_ENA |= PWM_ENA_CHID1; 
	
	// set PWM clock A to 1 MHz and clock B to 84 MHz
	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
	
	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;
	
	// set channel 1 to a period to 20 ms, giving a frequency of 50 Hz
	PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRDA);	
}