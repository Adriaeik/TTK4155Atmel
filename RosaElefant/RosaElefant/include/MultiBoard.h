/*
 * MultiBoard.h
 *
 * Created: 16.10.2024 09:35:04
 *  Author: adriaeik
 */ 


#ifndef MULTIBOARD_H_
#define MULTIBOARD_H_

#include "../lib/can/can_controller.h"

#define JOY_ANGLE_HYSTERESIS 5
#define  DEBUG_MULTIBOARD 0

#define ID_JOY_Y_POS	1
#define ID_JOY_X_POS	2
#define ID_JOY_BTN		3
#define ID_LS_POS		4
#define ID_RS_POS		5
#define ID_L_BTN		6
#define ID_R_BTN		7
#define ID_JOY_ANGLE	8


// Struktur for å holde alle verdier relatert til MultiBoard (255 - 0)
typedef struct {
	uint8_t LSpos;      // CH3, Left slider position
	uint8_t RSpos;      // CH2, Right slider position
	uint8_t LBtn;       // Left button state
	uint8_t RBtn;       // Right button state
	uint8_t JoyBtn;		// Joystick button state
	uint8_t JoyXOrigo;  // Kalibrert origo for Joystick X
	uint8_t JoyYOrigo;  // Kalibrert origo for Joystick Y
	int16_t JoyXposCal;	// Kalibrert joystik pos med +/- for å kunne bruke atan2
	int16_t JoyYposCal;	// Kalibrert joystik pos med +/- for å kunne bruke atan2
	int16_t JoyAngle;
	uint16_t JoyYLastAction;
} MultiBoard;


void MultiBoard_Init(MultiBoard* board);				// Initialisering og kalibrering
void update_board_from_can(MultiBoard* board, CAN_MESSAGE* msg);


#endif /* MULTIBOARD_H_ */