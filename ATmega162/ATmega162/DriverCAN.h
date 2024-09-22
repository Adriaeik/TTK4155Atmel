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
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} CANMessage;

// CAN-funksjoner
void CAN_Init(void);
void CAN_SendMessage(CANMessage* msg);
CANMessage CAN_ReceiveMessage(void);

#endif /* DRIVER_CAN_H_ */
