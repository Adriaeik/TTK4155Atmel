/*
 * servo.h
 *
 * Created: 02.10.2024 11:25:32
 *  Author: adriaeik
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include "sam.h"
#include "pwm.h"

#define SERVO_PIN PIO_PB13  // Servo signal pin (PB13)
#define PWM_FREQUENCY 50     // PWM frekvens: 50 Hz (20 ms periode)

// Funksjonsprototypar for å initialisere og styre PWM-signalet for servoen
void servo_pwm_init(void);
void servo_set_position(double ms);  // Juster posisjonen til servoen basert på duty cycle
void servo_set_position_joy(void);

#endif /* SERVO_H_ */