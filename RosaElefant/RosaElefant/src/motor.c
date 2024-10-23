/*
 * motor.c
 *
 * Created: 23.10.2024 09:33:52
 *  Author: adriaeik
 */ 
#include "../include/motor.h"
#include "../include/MultiBoard.h"
MultiBoard board;

void motor_init(void) {
	// 1. Aktivere klokke for PIOA og PIOB
	PMC->PMC_PCER0 |= (1 << ID_PIOA) | (1 << ID_PIOB) | (1 << ID_PWM);  // Aktivere klokkene for PIOA, PIOB, og PWM

	// 2. Konfigurer PHASE_PIN (PC23) som utgang for retning
	PIOC->PIO_OER |= PHASE_PIN; // Sett PHASE_PIN som utgang
	PIOC->PIO_PER |= PHASE_PIN; // Aktiver PIO kontroll over PHASE_PIN
}

void update_motor_control(void) {
	double ms;
	double leftRange = -100;  // Venstre grenseverdi for joystick
	double rightRange = 100;  // Høggrenseverdi for joystick
	double minDutyMs = 0.9;   // Minimum duty syklus i ms for hastigheitsstyring
	double maxDutyMs = 2.1;   // Maksimum duty syklus i ms for hastigheitsstyring
	double midDutyMs = (minDutyMs + maxDutyMs) / 2;  // Midtposisjon

	int x_pos = board.JoyXposCal;  // Les joystick posisjon

	// Kalkuler ms-verdi basert på joystickens posisjon
	if (abs(x_pos) < 2) {
		ms = midDutyMs;  // Midtposisjon, ingen bevegelse
		} else if (x_pos > 0) {
		ms = (x_pos / rightRange) * (maxDutyMs - midDutyMs) + midDutyMs;  // Positiv bevegelse
		} else if (x_pos < 0) {
		ms = (x_pos / leftRange) * (minDutyMs - midDutyMs) + midDutyMs;  // Negativ bevegelse
	}

	// Begrens ms-verdien mellom minimum og maksimum
	if (ms < minDutyMs) {
		ms = minDutyMs;
		} else if (ms > maxDutyMs) {
		ms = maxDutyMs;
	}

	// Kalkuler duty cycle basert på den nye ms-verdien
	// 20 ms periode (20000 ticks), så ms må konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[5].PWM_CPRD);

	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[5].PWM_CDTYUPD = duty_cycle_ticks;

	// Sett motor retning
	if (x_pos > 0) {
		PIOC->PIO_SODR = PHASE_PIN;  // Sett retning til høg (positiv retning)
		} else if (x_pos < 0) {
		PIOC->PIO_CODR = PHASE_PIN;  // Sett retning til låg (negativ retning)
	}

	// printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", duty_cycle_ticks, ms);
}
