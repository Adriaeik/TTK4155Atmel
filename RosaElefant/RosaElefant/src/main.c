/*
 * RosaElefant.c
 *
 * Created: 02.10.2024 10:34:01
 * Author : adriaeik
 */ 


#include "sam.h"
#include "../include/uart_int.h"
#include "../include/time.h"
#include "../include/pwm.h"


int main(void) {
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate p� 9600

	uint8_t received_char;
	
	  // Initialiser PWM for servo
	  servo_init();

	while (1) {
		// G� opp til maksimal posisjon (2.1 ms)
		for (double pos = 0.9; pos <= 2.1; pos += 0.01) {
			servo_set_position(pos);
			time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		}

		// G� ned til minimum posisjon (0.9 ms)
		for (double pos = 2.1; pos >= 0.9; pos -= 0.01) {
			servo_set_position(pos);
			time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		}
	}
}