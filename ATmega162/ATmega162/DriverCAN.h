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
void CAN_Init(uint8_t mode); //La til mode her for å generalisere
							//Gjør det lettere å initialisere i annen mode
							//Når vi ikke lenger vil bruke loopback
void CAN_SendMessage(CANMessage* msg);
void CAN_ReceiveMessage(CANMessage* msg); //Flyttet return verdi til pointer inn verdi
										//Så funksjonen kalles ved CAN_recieve(message)
										//istedenfor message = CAN_recieve();
										//blir både penere og mindre minne må kopieres og sendes rundt så det er per definisjon bedre (raskere)
										//Dette gjør det også mulig å returnere 0/1 basert på om man mottar korrekt eller ikke!!

#endif /* DRIVER_CAN_H_ */
