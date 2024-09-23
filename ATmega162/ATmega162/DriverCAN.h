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
void CAN_Init(uint8_t mode); //La til mode her for � generalisere
							//Gj�r det lettere � initialisere i annen mode
							//N�r vi ikke lenger vil bruke loopback
void CAN_SendMessage(CANMessage* msg);
void CAN_ReceiveMessage(CANMessage* msg); //Flyttet return verdi til pointer inn verdi
										//S� funksjonen kalles ved CAN_recieve(message)
										//istedenfor message = CAN_recieve();
										//blir b�de penere og mindre minne m� kopieres og sendes rundt s� det er per definisjon bedre (raskere)
										//Dette gj�r det ogs� mulig � returnere 0/1 basert p� om man mottar korrekt eller ikke!!

#endif /* DRIVER_CAN_H_ */
