/*
 * ATmega162.c
 *
 * Created: 29.08.2024 13:28:45
 * Author : eikel
 */ 

#include "DriverUART.h"


int main(void) {
	/*-----------------------
	Setup start 
	----------------------*/
	UART_Init(MYUBBR);
	URAT_initStudio();
	UART_EnableReceiveInterrupt();
	sei();
    /*-----------------------
    Setup end 
    ----------------------*/
	
	
    printf("init done\n\r");
    
    while (1) {
        _delay_ms(scaleDelay(1000));
		printf("i while \r\n");
    }
	return 0;
}
