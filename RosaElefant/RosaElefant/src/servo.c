/*
 * servo.c
 *
 * Created: 02.10.2024 11:26:09
 *  Author: adriaeik
 */ 
#include "../include/servo.h"

void servo_pwm_init(void) {
	// Aktiver Peripheral Clock for PWM og PIOB
	PMC->PMC_PCER1 |= (1 << ID_PWM);  // Aktiver klokke for PWM
	PMC->PMC_PCER0 |= (1 << ID_PIOB);  // Aktiver klokke for PIOB

	// Sett opp PB13 som PWM-output
	PIOB->PIO_PDR = SERVO_PIN;  // Deaktiver GPIO control for PB13
	PIOB->PIO_ABSR |= SERVO_PIN;  // Sett PB13 til Peripheral B (PWM)

	// Sett opp PWM-kanal 1 for 50 Hz (20 ms periode)
	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_32;  // Sett prescaler
	PWM->PWM_CH_NUM[1].PWM_CPRD = 20000;  // 20 ms periode (50 Hz frekvens)
	PWM->PWM_CH_NUM[1].PWM_CDTY = 1500;  // Sett initial duty cycle til 1.5 ms (midtposisjon)

	// Aktiver PWM-kanalen
	PWM->PWM_ENA = (1 << PWM_ENA_CHID1);  // Aktiver PWM på kanal 1
}

void servo_set_position(uint32_t duty_cycle_us) {
	// Konverter duty cycle frå mikrosekund til PWM-ticks
	uint32_t duty_cycle_ticks = (duty_cycle_us * 2.625);  // 2.625 ticks per mikrosekund
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = duty_cycle_ticks;  // Oppdater duty cycle
}