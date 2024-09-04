/*
 * Utils.h
 *
 * Created: 04.09.2024 15:14:20
 *  Author: adriaeik
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define scaleDelay(delay) (5 * delay)
#define setBit(reg, bit) (reg |= (1 << bit))  // DDRB |= (1 << PB0); PORTB |= (1 << PB0);
#define clearBit(reg, bit) (reg &= ~(1 << bit)) // PORTB &= ~(1 << PB0)
#define testBit(reg, bit) (reg & (1 << bit))
#define loopUntilBitIsSet(reg, bit) while(!testBit(reg, bit))
#define loopUntilBitIsClear(reg, bit) while(testBit(reg, bit))





#endif /* UTILS_H_ */