/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"
volatile extern uint8_t tc1_overflow_flag;
volatile extern uint8_t second_flag;

int main(void) {
	/*-----------------------
	Setup start 
	----------------------*/
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
	initTimer1();
	initTimer0();
	sei();
    /*-----------------------
    Setup end 
    ----------------------*/
	uint8_t counter = 0;
	uint8_t sek = 0;
	uint8_t cycle = 37;
	int8_t step = 1;
	
    printf("init done\n\r");
	setDutyCycle(cycle);
    
    while (1) {
        
		
		setDutyCycle(cycle);
		//setDutyCycle(cycle);
		
		if(cycle == 0){step = 1;}
		else if(cycle == 100){step = -1;}
		cycle += step;
		_delay_ms(scaleDelay(15));
		
		if(second_flag == 1){
			second_flag = 0;
			uint8_t dutyCycle = readDutyCycle();
			printf("Read cycle = %d, Actual cycle: %u\r\n", dutyCycle, cycle);
		}
	}
	
	
	return 0;
}
