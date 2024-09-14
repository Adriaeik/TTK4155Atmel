/*
 * Utils.c
 *
 * Created: 04.09.2024 17:16:46
 *  Author: adriaeik
 */ 
#include "Utils.h"

float claculateJoystickAngel(int16_t x, int16_t y){
	float angel = atan2((float)y, (float)x) * (180/M_PI);
	
	if (angel<0){
		angel +=360;
	}
	return angel;
}