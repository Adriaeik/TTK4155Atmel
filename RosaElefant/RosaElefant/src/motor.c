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
}

void motor_control_velocity(void) {
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
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[5].PWM_CPRD - duty_cycle_ticks;
	// Oppdater duty cycle direkte ved å skrive til PWM_CDTYUPD
	PWM->PWM_CH_NUM[5].PWM_CDTYUPD = inverted_duty_cycle;

	// Sett motor retning basert på fortegnet til joystick-verdien
	if (x_pos > 0)PIOC->PIO_SODR = PHASE_PIN;  // Sett pin 0 høg (positiv retning)} 
	else if (x_pos < 0)PIOC->PIO_CODR = PHASE_PIN;  // Sett pin 0 låg (negativ retning)

	// Optional: printf for å debug duty cycle og retning
	// printf("Ny PWM duty cycle sett til: %u ticks (for joystick posisjon: %d)\n\r", duty_cycle_ticks, x_pos);
}
// Funksjonar for normalisering
static double normalize_pos_encoder(void) {
	uint32_t encoder_pos = read_encoder_position();
	// Sørg for at posisjonen ikkje er negativ
	if (encoder_pos < 0)encoder_pos = 0;
	if (encoder_pos > ENCODER_RANGE)encoder_pos = ENCODER_RANGE;
	// Normaliserer posisjonen til eit verdi mellom 0 og 1
	return (double)encoder_pos / ENCODER_RANGE;
}

static double normalize_pos_ref(void) {
	uint8_t slider_pos = board.RSpos;
	return (double)slider_pos / SLIDER_RANGE;
}

static double clamp(double value, double min, double max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

uint8_t reset_PID_flag = 0;
// PID-regulator
void motor_control_PID(void) {
	// Normaliser posisjon og referanse
	double pos = normalize_pos_encoder();
	double ref = reset_PID_flag == 0 ? normalize_pos_ref() : 0.5; // Set referanse til midtposisjon ved reset
	
	// Beregn avviket
	error = ref - pos;
	integral += error;
	derivat = error - prev_error;
	// Sett motor retning basert på fortegnet til avviket
	if (error > 0) {
		PIOC->PIO_SODR = PHASE_PIN;  // Sett pin høg (positiv retning)
		} else if (error < 0) {
		PIOC->PIO_CODR = PHASE_PIN;  // Sett pin låg (negativ retning)
	}
	
	double actuation = (Kp * error) + (Ki * integral) + (Kd * derivat);
	prev_error = error;
	
	//VIKTIG med abs verdi då u må vere eit tall mellom 0 og 1
	double u = clamp(abs(actuation), 0.0, 1.0);	// Normaliser og klamp pådraget
	// Oppdater PWM duty cycle
	update_motor_pwm(u);
}

// Funksjon for å oppdatere motor PWM
void update_motor_pwm(double u) {
	uint32_t duty_cycle_ticks = (uint32_t)(u * PWM->PWM_CH_NUM[5].PWM_CPRD);
	uint32_t inverted_duty_cycle = PWM->PWM_CH_NUM[5].PWM_CPRD - duty_cycle_ticks;
	PWM->PWM_CH_NUM[5].PWM_CDTYUPD = inverted_duty_cycle;
}

// Reset PID og sett motor til midtposisjon
//blokerer men la gå
void reset_pid(void) {
	error = 0;
	prev_error = 0;
	integral = 0;
	derivat = 0;
	uint32_t timeout = 1000; // Tidsavbrudd for å unngå evig løkke
	
	reset_PID_flag = 1;
	while ((error > 0.01 || error < -0.01) && timeout > 0) { 
		motor_control_PID();
		timeout--;
	}
	reset_PID_flag = 0;
}
