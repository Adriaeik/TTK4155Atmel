/*
 * Timer.c
 *
 * Created: 13.11.2024 09:55:18
 *  Author: ravneb
 */ 


#include "Timer.h"

volatile extern uint8_t tc1_overflow_flag = 0;
volatile extern uint8_t second_flag = 0;

volatile uint8_t start_measure = 0;
volatile uint16_t on_time = 0;
uint16_t prev_on_time = 0;



void initTimer1(void){
	//Overflow på 2^16. her er prescale 1 som bør gi 75 Hz overflow -> t_ovf = 0.01333... s ~ 13.33ms
	
	TCCR1B |= (1 << CS10); //Skru på chipclock uten prescale som source
	TCCR1B |= (1 << ICNC1) | (1 << ICES1); // skru på filter på ICP1 og capt på rising edge
	
	TIFR |= (1 << TOV1); //Clear int på overflow
	TIFR |= (1 << ICF1); //Clear input capture flag
	TIMSK |= (1 << TICIE1); //Enable input capture int
	TIMSK |= (1 << TOIE1); //enable interupt på overflow
}

void initTimer0(void){
	DDRB |= (1 << DDB0); //Sett PB0 som output (OC0)
	
	TCCR0 |= (1 << WGM00) | (1 << WGM01); //Sett i fastPWM mode
	
	//Prescale 64 vil gi PWM freq 1/300, t_pwm = 3.333 ms
	TCCR0 |= (1 << CS01) | (1 << CS00); //Prescale 64
	TCCR0 |= (1 << COM01); //Clear oc0 på compare, set oc0 på TOP (først høy så lav)
	
	
	OCR0 = TC0TOP/2; //starter på dutycycle 0.5
	TIFR |= (1 << OCF0); //Clear output compare flag
	TIMSK |= (1 << OCIE0); //Enable output compare timer 0
}

void setDutyCycle(uint8_t cycle){
	if(cycle > 100){cycle = 100;}
	OCR0 = (uint8_t)((cycle*256)/100);
}


uint8_t readDutyCycle(void){
	
	return (uint8_t)(prev_on_time/163);
}

ISR(TIMER1_OVF_vect){
	tc1_overflow_flag += 1;
	if(tc1_overflow_flag == 75){
		second_flag = 1;
		tc1_overflow_flag = 0;
		TCCR1B |= (1 << ICES1); // skru på capt på rising edge
		TIMSK |= (1 << TICIE1); //Enable input capture int
		
	}
}

ISR(TIMER1_CAPT_vect){
	if(TCCR1B & (1 << ICES1)){
		on_time = TCNT1;
		
		TCCR1B &= ~(1 << ICES1); // skru på capt på falling edge
	}
	else{
		on_time = TCNT1 - on_time;
		prev_on_time = on_time;
		TIMSK &= ~(1 << TICIE1); //Disable input capture int
	}
}

ISR(TIMER0_COMP_vect){
}





