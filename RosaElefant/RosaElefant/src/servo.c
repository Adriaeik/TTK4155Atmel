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
	PIOC->PIO_OER |= SOLENOID_PIN; // Sett SOLENOID_PIN som utgang
	PIOC->PIO_PER |= SOLENOID_PIN; // Aktiver PIO kontroll over SOLENOID_PIN
}

//IKKJE I BRUK
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
extern int inverted_controll;
void servo_set_position_joy(void) {
	// Sjekk og avgrens millisekundverdien innanfor 0.9 ms og 2.1 ms
	// til venstre frå 0 til - 161. til høgre 0 til 93 for JoyXposCal
	double ms;
	double leftZat = (inverted_controll == 1) ? -93 : -161; // egentlig 93
	double righZat = (inverted_controll == 1) ? 161 : 93;
	double midt = (0.9+2.1)/2;
	
	int x_pos = (inverted_controll == 1) ? -board.JoyXposCal : board.JoyXposCal; // Les joystick posisjon if (abs(x_pos) < 2){
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

	// Kalkuler duty cycle basert på den nye ms-verdien
	// 20 ms periode (20000 ticks), så ms må konverterast til ticks
	uint32_t duty_cycle_ticks = (uint32_t)((ms / 20.0) * PWM->PWM_CH_NUM[1].PWM_CPRD);

	// Inverter duty cycle slik at høg tid svarer til 0.9 til 2.1 ms
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[1].PWM_CPRD - duty_cycle_ticks;

	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[1].PWM_CDTYUPD = inverted_duty_cycle;
	void solenoid_handle();

	//printf("Ny PWM duty cycle sett til: %u ticks (for %f ms)\n\r", inverted_duty_cycle, ms);
}

void solenoid_handle(){
	if (board.JoyBtn == 1) {
		PIOC->PIO_SODR = SOLENOID_PIN;  // Sett pin høg (positiv retning)
		} else if (board.JoyBtn == 0) {
		PIOC->PIO_CODR = SOLENOID_PIN;  // Sett pin låg (negativ retning)
	}
}

