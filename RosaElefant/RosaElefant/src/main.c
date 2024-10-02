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
#include "../include/can.h"
#include "../lib/can/can_controller.h"


int main(void) {
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600
	can_init_controller(1,2);
	
	
	  // Initialiser PWM for servo
	servo_init();


	uint8_t i = 0;
	while (1) {

		// Gå opp til maksimal posisjon (2.1 ms)
		for (double pos = 0.9; pos <= 2.1; pos += 0.01) {
			servo_set_position(pos);
			time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		}

		// Gå ned til minimum posisjon (0.9 ms)
		for (double pos = 2.1; pos >= 0.9; pos -= 0.01) {
			servo_set_position(pos);
			time_spinFor(msecs(20));  // Vent i 20 ms for glatt overgang
		}
	}
}