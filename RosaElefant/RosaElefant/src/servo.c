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

	// Kalkuler duty cycle basert på den nye ms-verdien
	// 20 ms periode (20000 ticks), så ms må konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[1].PWM_CPRD);

	// Inverter duty cycle slik at høg tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;

	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = inverted_duty_cycle;

	//printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", inverted_duty_cycle, ms);
}
void servo_set_position_joy(void) {
	// Sjekk og avgrens millisekundverdien innanfor 0.9 ms og 2.1 ms
	// til venstre frå 0 til - 161. til høgre 0 til 93 for JoyXposCal
	double ms;
	double leftZat = -161;
	double righZat = 93;
	double midt = (0.9+2.1)/2;
	if (abs(board.JoyXposCal) < 2){
		ms = midt;
	} else if (board.JoyXposCal > 0){
		ms = board.JoyXposCal/righZat* (2.1 - midt) + midt;
	} else if (board.JoyXposCal < 0) {
		ms = board.JoyXposCal/leftZat* (0.9- midt) + midt;
	}
	if (ms < 0.9) {
		ms = 0.9;
		} else if (ms > 2.1) {
		ms = 2.1;
	}

	// Kalkuler duty cycle basert på den nye ms-verdien
	// 20 ms periode (20000 ticks), så ms må konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[1].PWM_CPRD);

	// Inverter duty cycle slik at høg tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;

	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = inverted_duty_cycle;

	//printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", inverted_duty_cycle, ms);
}


