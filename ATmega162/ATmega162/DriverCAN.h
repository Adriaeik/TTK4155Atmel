/*
 * DriverCAN.h
 *
 * Created: 22.09.2024 14:04:43
 *  Author: eikel
 */ 


#ifndef DRIVER_CAN_H_
#define DRIVER_CAN_H_

#include "DriverMCP2515.h"

// CAN-meldingsstruktur
typedef struct {
	uint16_t id; //ID må vre 
	uint8_t length;
	uint8_t data[8];
} CANMessage;

// CAN-funksjoner
void CAN_Init(uint8_t mode);
void CAN_SendMessage(CANMessage* msg);
uint8_t CAN_ReceiveMessage(CANMessage* msg);
void CAN_Interrupt_recive_int();

#endif /* DRIVER_CAN_H_ */
