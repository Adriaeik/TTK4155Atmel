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
int normalize_pos_encoder(int prev_pos) {
	//printf("encoder val: %d\n\r", read_encoder_position() );
	int32_t encoder_pos = read_encoder_position();
	// Sørg for at posisjonen ikkje er negativ
	if (encoder_pos < 0)encoder_pos = 0;
	if (encoder_pos > ENCODER_RANGE)encoder_pos = ENCODER_RANGE;
	double a = 10;
	double b = 10 - a;
	return (int)((double)encoder_pos / ENCODER_RANGE * a * PARAM_SCALE + prev_pos*b)/10 ;
}

int normalize_pos_ref(int prev_ref) {
	int a = 10;
	int b = 10 - a;
	uint8_t slider_pos = board.RSpos;
	return (int)((double)slider_pos / SLIDER_RANGE* a*PARAM_SCALE + prev_ref * b)/10; //IKKJE 1000 for å fjerne 10
}

int clamp(int value, int min, int max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

uint8_t reset_PID_flag = 0;
long long int ref = 0;
extern int inverted_controll;
// PID-regulator
void motor_control_PID(void) {
	// Normaliser posisjon og referanse
	static long long int prev_ref = 0;
	static long long int prev_pos = 0;
	
	long long int pos = clamp(normalize_pos_encoder(prev_pos), 0, PARAM_SCALE);
	if (reset_PID_flag == 0) {
		if (inverted_controll) {
			// Inverter referansen basert på SLIDER_RANGE
			ref = clamp(PARAM_SCALE - normalize_pos_ref(prev_ref),0, PARAM_SCALE);
			} else {
			// Normal referanse utan invertering
			ref = clamp(normalize_pos_ref(prev_ref), 0,  PARAM_SCALE) ;
		}
		} else {
		// Set referanse til midtposisjon ved reset
		ref = PARAM_SCALE / 2;
	}

	prev_ref = ref;
	prev_pos = pos;
	
	// Beregn avviket
	static int error_hyst = 100;
	error = pos - ref;
	integral = clamp((integral + error)/50, -PARAM_SCALE,PARAM_SCALE);
	derivat = error - prev_error;
	
	//int test = (int)(ref * 1000 - pos*1000);
	//int integral_1000 = (int)(integral * 1000);
	//int derivat_1000 = (int)(derivat * 1000);



	// Sett motor retning basert på fortegnet til avviket

	if (error > 0) {
		if (abs(error)> error_hyst){
			PIOC->PIO_SODR = PHASE_PIN;  // Sett pin høg (positiv retning) mot høyre?
			}
		} else if (error < 0) {
		if (abs(error)> error_hyst){
			PIOC->PIO_CODR = PHASE_PIN;  // Sett pin låg (negativ retning)
		}
	}
	
	long long int actuation = (Kp * error) + (Ki * integral) + (Kd * derivat);
	//printf("podrag1: %d ", (int)(actuation));
	prev_error = error;
	
	//VIKTIG med abs verdi då u må vere eit tall mellom 0 og 1
	long long int u = clamp(abs(actuation), 0, PARAM_SCALE);	// Normaliser og klamp pådraget

	// Oppdater PWM duty cycle. normalisert
	update_motor_pwm((double)u/PARAM_SCALE);

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
	ref = 0;
	
	reset_PID_flag = 1;
	while ((error > 1 || error < -1) && timeout > 0) { 
		motor_control_PID();
		timeout--;
	}
	reset_PID_flag = 0;
}
#include "../include/time.h"
int calibrate_motor_pos(void) {
	int prev_pos = read_encoder_position();  // Startposisjon
	int curr_pos;
	int stable_count = 0;       // Teller for stabil posisjon
	const int stable_threshold = 15;  // Antal gonger posisjonen må vere uendra
	const int hysteresis = 2;   // Toleranse for stillstand

	PIOC->PIO_CODR = PHASE_PIN;  // Sett pin låg (negativ retning)
	printf("Starting calibration towards wall...\n\r");

	while (stable_count < stable_threshold) {
		// Beveg motoren mot veggen
		PIOC->PIO_SODR = PHASE_PIN;  // Sett pin låg (negativ retning)
		// Sett motor retning basert på fortegnet til avviket
		//error = -1;
		//if (error > 0) {
			//PIOC->PIO_SODR = PHASE_PIN;  // Sett pin høg (positiv retning) mot høyre?
			//} else if (error < 0) {
			//PIOC->PIO_CODR = PHASE_PIN;  // Sett pin låg (negativ retning)
		//}
		update_motor_pwm(0.4);

		curr_pos = read_encoder_position();  
		printf("Position: %d\n\r", curr_pos);

		// Sjekk om posisjonen er stabil innanfor hysterese
		if (abs(curr_pos - prev_pos) <= hysteresis) {
			stable_count++;  
			} else {
			stable_count = 0; 
		}
	
		prev_pos = curr_pos;  
		time_spinFor(msecs(100));
	}

	update_motor_pwm(0); 
	encoder_init();  // Sett referansepunktet til 0
	printf("Calibration complete. Reference set to 0.\n\r");

	return 0;
}

