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
#define SLIDER_RANGE 255


	extern double Kp = 199;
	extern  double Ki = 10;
	extern  double Kd = 20;
	extern  double error = 0;
	extern  double prev_error = 0;
	extern  double integral = 0;
	extern  double derivat = 0;
	
void motor_init(void);
void motor_control_velocity(void);
void motor_control_PID(void);
void normalize_pos_error(void);
double normalize_pos_encoder(void);



#endif /* MOTOR_H_ */