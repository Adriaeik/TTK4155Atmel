/*
 * servo.c
 *
 * Created: 02.10.2024 11:26:09
 *  Author: adriaeik
 */ 
#include "../include/servo.h"
#include "../include/MultiBoard.h"

MultiBoard board;

void servo_init(void) {
	pwm_init();
}

void servo_set_position(double ms) {
	// Sjekk og avgrens millisekundverdien innanfor 0.9 ms og 2.1 ms
	if (ms < 0.9) {
		ms = 0.9;
		} else if (ms > 2.1) {
		ms = 2.1;
	}

	// Kalkuler duty cycle basert p� den nye ms-verdien
	// 20 ms periode (20000 ticks), s� ms m� konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[1].PWM_CPRD);

	// Inverter duty cycle slik at h�g tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;

	// Oppdater duty cycle direkte ved � skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = inverted_duty_cycle;

	//printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", inverted_duty_cycle, ms);
}
void servo_set_position_joy(void) {
	// Sjekk og avgrens millisekundverdien innanfor 0.9 ms og 2.1 ms
	// til venstre fr� 0 til - 161. til h�gre 0 til 93 for JoyXposCal
	double ms;
	double leftZat = -161;
	double righZat = 93;
	double midt = (0.9+2.1)/2;
	
	int x_pos = board.JoyYposCal;  // Les joystick posisjon
	if (abs(x_pos) < 2){
		ms = midt;
	} else if (x_pos > 5){
		ms = x_pos/righZat* (2.1 - midt) + midt;
	} else if (x_pos < -5) {
		ms = x_pos/leftZat* (0.9- midt) + midt;
	}
	if (ms < 0.9) {
		ms = 0.9;
		} else if (ms > 2.1) {
		ms = 2.1;
	}

	// Kalkuler duty cycle basert p� den nye ms-verdien
	// 20 ms periode (20000 ticks), s� ms m� konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[1].PWM_CPRD);

	// Inverter duty cycle slik at h�g tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;

	// Oppdater duty cycle direkte ved � skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = inverted_duty_cycle;

	//printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", inverted_duty_cycle, ms);
}


