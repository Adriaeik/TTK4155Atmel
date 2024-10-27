/*
 * external.h
 *
 * Created: 24.10.2024 21:27:15
 *  Author: eikel
 */ 


#ifndef EXTERNAL_H_
#define EXTERNAL_H_

extern  long long int Kp;
extern  long long int Ki;
extern  long long int Kd;
extern  long long int error;
extern  long long int prev_error;
extern  long long int integral;
extern  long long int derivat;

extern MultiBoard board;
extern Game main_game;
extern uint16_t score;




#endif /* EXTERNAL_H_ */