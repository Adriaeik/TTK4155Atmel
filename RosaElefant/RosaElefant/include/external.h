/*
 * external.h
 *
 * Created: 24.10.2024 21:27:15
 *  Author: eikel
 */ 


#ifndef EXTERNAL_H_
#define EXTERNAL_H_

extern double Kp = 199;
extern  double Ki = 10;
extern  double Kd = 20;
extern  double error = 0;
extern  double prev_error = 0;
extern  double integral = 0;
extern  double derivat = 0;

extern MultiBoard board;
extern Game main_game;
extern uint16_t score;




#endif /* EXTERNAL_H_ */