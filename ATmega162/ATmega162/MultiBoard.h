/*
 * MultiBoard.h
 *
 * Created: 14.09.2024 11:00:42
 *  Author: ravneb
 */ 


#ifndef MULTIBOARD_H_
#define MULTIBOARD_H_

#define RS_PWM_PIN 

#include <stdint.h>
#include "Utils.h"

void MultiBoard_Init(void);
void MultiBoard_ReadSlider(void);
uint16_t MultiBoard_getSliderPosition(void);
void MultiBoard_timerInterruptHandler(void);



#endif /* MULTIBOARD_H_ */