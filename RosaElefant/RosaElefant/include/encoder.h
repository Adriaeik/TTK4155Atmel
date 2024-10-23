/*
 * encoder.h
 *
 * Created: 23.10.2024 09:11:44
 *  Author: adriaeik
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_
#include "sam.h"         // Denne inkluderer definisjonar for ATSAM3X8E
#include <stdint.h>      // For faste bredder av heiltallstypar

void encoder_init(void);
uint32_t read_encoder_position(void);



#endif /* ENCODER_H_ */