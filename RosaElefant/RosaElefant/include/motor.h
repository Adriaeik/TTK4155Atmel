/*
 * motor.h
 *
 * Created: 23.10.2024 09:33:31
 *  Author: adriaeik
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam.h"

#define SOLENOID_PIN    PIO_PC17   // Phase/DIR er koblet til D7, som tilsvarer PC23 TEST:PIO_PC17
#define PHASE_PIN    PIO_PC23 
#define ENABLE_PIN   PIO_PB12   // ENABLE/PWM er koblet til D20/SDA, som tilsvarer PB12 TEST:PIO_PDR_P19

#define ENCODER_RANGE 5654

void motor_init(void);
void update_motor_control(void);


#endif /* MOTOR_H_ */