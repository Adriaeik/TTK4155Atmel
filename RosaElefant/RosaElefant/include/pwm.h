/*
 * IncFile1.h
 *
 * Created: 02/10/2024 12:04:23
 *  Author: Ravn
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "sam.h"

#define MCK 84000000
#define PV 50
#define WPKEY 0x50494F

#define PERIOD_A    20E-3
#define PERIOD_B    100E-3
#define DIVA        84
#define DIVB        84
#define CLK_A       1E6
#define CLK_B       1E6
#define CPRDA       (int) (PERIOD_A * MCK / DIVA)
#define CPRDB       (int) (PERIOD_B * MCK / DIVB)

void pwm_init();


#endif /* INCFILE1_H_ */