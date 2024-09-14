/*
 * MultiBoard.c
 *
 * Created: 14.09.2024 11:00:59
 *  Author: ravneb
 */ 
#include "MultiBoard.h"

void MultiBoard_Init(MultiBoard* board) {
	// Set pinner for knapper som input
	clearBit(DDRB, LEFT_BUTTON_PIN);  // Set Left button pin as input
	clearBit(DDRB, RIGHT_BUTTON_PIN); // Set Right button pin as input
	clearBit(DDRB, JOY_BUTTON_PIN);   // Set Joystick button pin as input
	clearBit(DDRB, BUSY_PIN);         // Set BUSY pin as input
	
	// Aktiver pullup
	setBit(PORTB, JOY_BUTTON_PIN);
	// Kalibrer joysticken (finn origo)
	Universal_write(ADC_START, 0x80); // Velg JoyX
	loopUntilBitIsClear(PINB, BUSY_PIN);    // Vent til BUSY går lav
	board->JoyYOrigo = Universal_read(ADC_START); //CH0
	board->JoyXOrigo = Universal_read(ADC_START); //CH1
	
	// Initialiser alle andre verdier
	board->LSpos = 0;
	board->RSpos = 0;
	board->LBtn = 0;
	board->RBtn = 0;
	board->JoyXpos = 0;
	board->JoyYpos = 0;
	board->JoyXposCal = 0;
	board->JoyYposCal = 0;
	board->JoyBtn = 1;  // 1 betyr ikkje trykt (Active Low)
}


void MultiBoard_Update(MultiBoard* board) {
	// Velg kanal for venstre skyvebryter (LSpos)
	Universal_write(ADC_START, 0x00);
	loopUntilBitIsClear(PINB, BUSY_PIN);    // Vent til BUSY går lav
	board->JoyYpos = Universal_read(ADC_START); //første RD low gir channel 0
	board->JoyXpos = Universal_read(ADC_START); //andre RD low gir channel 1
	board->RSpos = Universal_read(ADC_START); // - CH2
	board->LSpos = Universal_read(ADC_START); // - CH3
	board->JoyYposCal = (int16_t)(board->JoyYpos) - (int16_t)(board->JoyYOrigo);
	board->JoyXposCal = (int16_t)(board->JoyXpos) - (int16_t)(board->JoyXOrigo);
	// Les knappestatus
	board->LBtn = 0<testBit(PINB, LEFT_BUTTON_PIN);
	board->RBtn = 0<testBit(PINB, RIGHT_BUTTON_PIN);
	board->JoyBtn = !(0<testBit(PINB, JOY_BUTTON_PIN));
}
