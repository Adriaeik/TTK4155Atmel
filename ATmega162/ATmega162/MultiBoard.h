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
#define JOY_BUTTON_PIN PB2  // Joystick button pin (Active Low)
#define RIGHT_BUTTON_PIN PB3 // Right button pin BUSY_PIN
#define LEFT_BUTTON_PIN PB4  // Left button pin
#define BUSY_PIN PB1 // Right button pin


// Struktur for å holde alle verdier relatert til MultiBoard (255 - 0)
typedef struct {
	uint8_t LSpos;      // CH3, Left slider position
	uint8_t RSpos;      // CH2, Right slider position
	uint8_t LBtn;       // Left button state
	uint8_t RBtn;       // Right button state
    uint8_t JoyXpos;    // CH1, Joystick X position (signed to handle negative offsets)
    uint8_t JoyYpos;    // CH0, Joystick Y position (signed to handle negative offsets)
	uint8_t JoyBtn;		// Joystick button state
	uint8_t JoyXOrigo;  // Kalibrert origo for Joystick X
	uint8_t JoyYOrigo;  // Kalibrert origo for Joystick Y
	int16_t JoyXposCal;	// Kalibrert joystik pos med +/- for å kunne btuke atan2
	int16_t JoyYposCal;	// Kalibrert joystik pos med +/- for å kunne btuke atan2
	//float JoyAngel;
} MultiBoard;


void MultiBoard_Init(MultiBoard* board);             // Initialisering og kalibrering
void MultiBoard_Update(MultiBoard* board);           // Oppdater MultiBoard verdier


#endif /* MULTIBOARD_H_ */