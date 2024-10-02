/*
 * RosaElefant.c
 *
 * Created: 02.10.2024 10:34:01
 * Author : adriaeik
 */ 


#include "sam.h"
#include "../include/uart_int.h"
#include "../include/time.h"

int main(void) {
	SystemInit();  // Initialiser systemklokka og mikrokontrolleren
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600

	uint8_t received_char;
	
	  // Initialiser PWM for servo
	  servo_pwm_init();

	while (1) {
		printf("lol\n\r");
		if (uart_rx(&received_char)) {  // Mottar data frå PC via UART
			uart_tx(received_char);     // Send mottatt data tilbake
		}
		// Sett servo til maks posisjon (2.1 ms duty cycle)
		servo_set_position(2100);  // 2100 us = 2.1 ms (maks posisjon)
		time_spinFor(msecs(1000));  // Vent i 1 sekund

		// Sett servo til minimum posisjon (0.9 ms duty cycle)
		servo_set_position(900);  // 900 us = 0.9 ms (minimum posisjon)
		time_spinFor(msecs(1000));  // Vent i 1 sekund
	}
}