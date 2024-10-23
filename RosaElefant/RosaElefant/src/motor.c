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

	PIOC->PIO_OER |= PHASE_PIN; // Sett PHASE_PIN som utgang
	PIOC->PIO_PER |= PHASE_PIN; // Aktiver PIO kontroll over PHASE_PIN
	// Aktiver klokka til PIOC
	//PMC->PMC_PCER0 = (1 << ID_PIOC);  // Aktiver klokka til PIOC

}

void update_motor_control(void) {
	// Definer parametrene for joystickkalibrering og duty cycle
	double ms;
	double leftZat = -161;
	double righZat = 93;
	double minDutyMs = 0.0; // Minimum duty cycle (0 %)
	double maxDutyMs = 1.0; // Maksimum duty cycle (100 %)
	double midDutyMs = 0.0; // Midtverdi, som er 0 % når motoren står stille

	int x_pos = board.JoyXposCal;  // Les joystick posisjon

	// Kalkuler ms-verdi (duty cycle) basert på joystickens posisjon
	if (abs(x_pos) < 5) {
		ms = midDutyMs;  // Midtposisjon, ingen bevegelse (0 % duty cycle)
		} else if (x_pos > 0) {
		ms = (x_pos / righZat) * maxDutyMs;  // Positiv bevegelse (0 til 100 % duty cycle)
		} else if (x_pos < 0) {
		ms = (x_pos / leftZat) * maxDutyMs;  // Negativ bevegelse (0 til 100 % duty cycle)
	}

	// Begrens ms-verdien mellom minimum og maksimum
	if (ms < minDutyMs) {
		ms = minDutyMs;
		} else if (ms > maxDutyMs) {
		ms = maxDutyMs;
	}

	// Kalkuler duty cycle basert på den nye ms-verdien
	uint32_t duty_cycle_ticks = (uint32_t)(ms * PWM->PWM_CH_NUM[5].PWM_CPRD);
	// Inverter duty cycle slik at høg tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;
	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[5].PWM_CDTYUPD = inverted_duty_cycle;

	// Sett motor retning basert på fortegnet til joystick-verdien
	if (x_pos > 0) {
		PIOC->PIO_SODR = PHASE_PIN;  // Sett pin 0 høg (positiv retning)
		} else if (x_pos < 0) {
		PIOC->PIO_CODR = PHASE_PIN;  // Sett pin 0 låg (negativ retning)
	}


	// Optional: printf for å debug duty cycle og retning
	// printf("Ny PWM duty cycle sett til: %u ticks (for joystick posisjon: %d)\n\r", duty_cycle_ticks, x_pos);
}

