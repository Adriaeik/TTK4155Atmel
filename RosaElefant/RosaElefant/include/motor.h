/*
 * motor.h
 *
 * Created: 23.10.2024 09:33:31
 *  Author: adriaeik
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "sam.h"
#include "../include/encoder.h"

#define PHASE_PIN    PIO_PC23	// Phase/DIR er koblet til D7, som tilsvarer PC23 TEST:PIO_PC17
#define ENABLE_PIN   PIO_PB12   // ENABLE/PWM er koblet til D20/SDA, som tilsvarer PB12 TEST:PIO_PDR_P19

#define ENCODER_RANGE 5654
#define SLIDER_RANGE 255
#define PARAM_SCALE 10E7

// Definert extern og value i external.h
extern  long long int Kp = 199.0;
extern  long long int Ki = 10.0;
extern  long long int Kd = 20.0;
extern  long long int error = 0.0;
extern  long long int prev_error = 0;
extern  long long int integral = 0;
extern  long long int derivat = 0;
	
void motor_init(void);
void motor_control_velocity(void);
void normalize_pos_error(void);
void motor_control_PID(void);
void update_motor_pwm(double u);
int normalize_pos_encoder(int prev_pos);
int normalize_pos_ref(int prev_ref);
int clamp(int value, int min, int max);
void reset_pid(void);


#endif /* MOTOR_H_ */