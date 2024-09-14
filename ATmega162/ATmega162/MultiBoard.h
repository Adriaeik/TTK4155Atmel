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

// Kanal-val byte-koder for ADC (ikkje riktige)
#define ADC_LEFT_SLIDER  0x08   // 1000 0000 -> Kanal 0 (LSpos)
#define ADC_RIGHT_SLIDER 0x09   // 1000 0001 -> Kanal 1 (RSpos)
#define ADC_JOYSTICK_X   0x0A   // 1000 0010 -> Kanal 2 (JoyX)
#define ADC_JOYSTICK_Y   0x0B   // 1000 0011 -> Kanal 3 (JoyY)
// Makrodefinisjoner for knappepinnar og joystick
#define JOY_BUTTON_PIN PA0  // Joystick button pin (Active Low)
#define LEFT_BUTTON_PIN PA1  // Left button pin
#define RIGHT_BUTTON_PIN PA2 // Right button pin BUSY_PIN
#define BUSY_PIN PB1 // Right button pin


// Struktur for å holde alle verdier relatert til MultiBoard (255 - 0)
typedef struct {
	uint8_t LSpos;      // Left slider position
	uint8_t RSpos;      // Right slider position
	uint8_t LB;          // Left button state
	uint8_t RB;          // Right button state
    int16_t JoyXpos;     // Joystick X position (signed to handle negative offsets)
    int16_t JoyYpos;     // Joystick Y position (signed to handle negative offsets)
	uint8_t JoyButton;   // Joystick button state
	uint8_t JoyXOrigo;   // Kalibrert origo for Joystick X
	uint8_t JoyYOrigo;   // Kalibrert origo for Joystick Y
} MultiBoard;


void MultiBoard_Init(MultiBoard* board);             // Initialisering og kalibrering
void MultiBoard_Update(MultiBoard* board);           // Oppdater MultiBoard verdier



#endif /* MULTIBOARD_H_ */