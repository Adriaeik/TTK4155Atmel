/*
 * MultiBoard.h
 *
 * Created: 14.09.2024 11:00:42
 *  Author: ravneb
 */ 



#ifndef MULTIBOARD_H_
#define MULTIBOARD_H_

#include <stdint.h>
#include "Utils.h"
#include "XMEM_Decode.h"

// Makrodefinisjoner for knappepinnar og joystick
#define JOY_BUTTON_PIN PB2		// Joystick button pin (Active Low)
#define RIGHT_BUTTON_PIN PB3	// Right button pin 
#define LEFT_BUTTON_PIN PD4		// Left button pin
#define BUSY_PIN PB1			// når låg er ADC klar for å gi data

#define ID_JOY_Y_POS	1
#define ID_JOY_X_POS	2
#define ID_JOY_BTN		3
#define ID_LS_POS		4
#define ID_RS_POS		5
#define ID_L_BTN		6
#define ID_R_BTN		7
#define ID_JOY_ANGLE	8


#define JOY_ANGLE_HYSTERESIS 5
// Struktur for å holde alle verdier relatert til MultiBoard (255 - 0)
typedef struct {
	uint8_t JoyYpos;    // CH0, Joystick Y position (signed to handle negative offsets)
	uint8_t JoyXpos;    // CH1, Joystick X position (signed to handle negative offsets)
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
	
	uint16_t JoyYLastAction; //Tror denne kan slettes
	
//FLAG
	uint8_t LSpos_l_can;      // CH3, Left slider position
	uint8_t RSpos_l_can;      // CH2, Right slider position
	uint8_t LBtn_l_can;       // Left button state
	uint8_t RBtn_l_can;       // Right button state
	uint8_t JoyBtn_l;		// For flag for meny
	uint8_t JoyBtn_l_can;		// For flag for meny
	int16_t JoyXposCal_l_can;	// Kalibrert joystik pos med +/- for å kunne bruke atan2
	int16_t JoyYposCal_l_can;	// Kalibrert joystik pos med +/- for å kunne bruke atan2
	int16_t JoyAngle_l_can;
} MultiBoard;


void MultiBoard_Init(MultiBoard* board);				// Initialisering og kalibrering
void MultiBoard_Update(MultiBoard* board);				// Oppdater MultiBoard verdier
void MultiBoard_UpdateJoystickAngle(MultiBoard* board);	// Oppdater Vinkel (kallast i _Update), å bruke atan2 virka til å skape problem, så den er hardkoda
void MultiBoard_Send(MultiBoard* board);


#endif /* MULTIBOARD_H_ */