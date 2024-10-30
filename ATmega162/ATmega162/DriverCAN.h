/*
 * DriverCAN.h
 *
 * Created: 22.09.2024 14:04:43
 *  Author: eikel
 */ 


#ifndef DRIVER_CAN_H_
#define DRIVER_CAN_H_

#include "DriverMCP2515.h"

#define ID_JOY_Y_POS		1
#define ID_JOY_X_POS		2
#define ID_JOY_BTN			3
#define ID_LS_POS			4
#define ID_RS_POS			5
#define ID_L_BTN			6
#define ID_R_BTN			7
#define ID_JOY_ANGLE		8
#define ID_GAME_LIVES_LEFT	9
#define ID_GAME_LIVES		10
#define ID_GAME_DIFFICULTY	11
#define ID_GAME_SCORE		12
#define ID_GAME_START		13
#define ID_GAME_OVER		14
#define ID_GAME_INITIALIZED 15

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
void CAN_Check_startup(void);

#endif /* DRIVER_CAN_H_ */
