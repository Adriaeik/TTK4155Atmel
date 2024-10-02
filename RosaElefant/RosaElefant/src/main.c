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
	uart_init(84000000, 9600);  // Initialiser UART med ein baudrate på 9600

	uint8_t received_char;
	
	  // Initialiser PWM for servo
	  pwm_init();

	while (1) {
		
		// Sett PWM duty cycle til 100% (høy)
		PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(0);
		time_spinFor(msecs(100));  // Vent i 1 sekund

		// Sett PWM duty cycle til 0% (lav)
		PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(PWM->PWM_CH_NUM[1].PWM_CPRD);
		time_spinFor(msecs(100));  // Vent 100 ms
		
	}
}