/*
 * Timer.h
 *
 * Created: 13.11.2024 09:55:05
 *  Author: ravneb
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "avr/io.h"
#include "avr/interrupt.h"

#define FOSC 4915200UL
#define TC0TOP 256

void initTimer1(void);
void initTimer0(void);
void setDutyCycle(uint8_t cycle);
uint8_t readDutyCycle(void);



#endif /* TIMER_H_ */