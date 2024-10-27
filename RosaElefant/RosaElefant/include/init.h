/*
 * init.h
 *
 * Created: 24.10.2024 21:08:11
 *  Author: eikel
 */ 


#ifndef INIT_H_
#define INIT_H_

#include "sam.h"
#include "uart_int.h"
#include "time.h"
#include "pwm.h"
//#include "../lib/can/can_controller.h"
#include "MultiBoard.h"
#include "driver_IR.h"
//#include "../include/encoder.h"
#include "game.h"

#define URAT_CPUFREQ 84000000
#define URAT_BAUD 9600

void init(void);
void checkExternalValues_toScreen(void);



#endif /* INIT_H_ */