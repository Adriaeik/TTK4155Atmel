/*
 * MultiBoard.c
 *
 * Created: 14.09.2024 11:00:59
 *  Author: ravneb
 */ 
#include "MultiBoard.h"

void MultiBoard_Init(MultiBoard* board) {
	// Set pinner for knapper som input
	clearBit(DDRD, LEFT_BUTTON_PIN);  // Set Left button pin as input
	clearBit(DDRD, RIGHT_BUTTON_PIN); // Set Right button pin as input
	clearBit(DDRD, JOY_BUTTON_PIN);   // Set Joystick button pin as input
	clearBit(DDRD, BUSY_PIN);         // Set BUSY pin as input

	// Kalibrer joysticken (finn origo)
	Universal_write(ADC_START, ADC_JOYSTICK_X); // Velg JoyX
	while (testBit(PINB, BUSY_PIN));    // Vent til BUSY går lav
	board->JoyXOrigo = Universal_read(0);

	Universal_write(ADC_START, ADC_JOYSTICK_Y); // Velg JoyY
	while (testBit(PINA, BUSY_PIN));    // Vent til BUSY går lav
	board->JoyYOrigo = Universal_read(0);
	
	// Initialiser alle andre verdier
	board->LSpos = 0;
	board->RSpos = 0;
	board->LB = 0;
	board->RB = 0;
	board->JoyXpos = 0;
	board->JoyYpos = 0;
	board->JoyButton = 1;  // 1 betyr ikkje trykt (Active Low)
}


void MultiBoard_Update(MultiBoard* board) {
	// Velg kanal for venstre skyvebryter (LSpos)
	Universal_write(ADC_START, ADC_LEFT_SLIDER);
	while (testBit(PINB, BUSY_PIN));  // Vent til BUSY blir lav
	board->LSpos = Universal_read(ADC_START);

	// Velg kanal for høgre skyvebryter (RSpos)
	Universal_write(ADC_START, ADC_RIGHT_SLIDER);
	while (testBit(PINB, BUSY_PIN));  // Vent til BUSY blir lav
	board->RSpos = Universal_read(ADC_START);

	// Velg kanal for joystick X
	Universal_write(ADC_START, ADC_JOYSTICK_X);
	while (testBit(PINB, BUSY_PIN));  // Vent til BUSY blir lav
	board->JoyXpos = (int16_t)(Universal_read(0)) - board->JoyXOrigo;

	// Velg kanal for joystick Y
	Universal_write(ADC_START, ADC_JOYSTICK_Y);
	while (testBit(PINB, BUSY_PIN));  // Vent til BUSY blir lav
	board->JoyYpos = (int16_t)(Universal_read(ADC_START)) - board->JoyYOrigo;

	// Les knappestatus
	board->LB = testBit(PIND, LEFT_BUTTON_PIN);
	board->RB = testBit(PIND, RIGHT_BUTTON_PIN);
	board->JoyButton = !testBit(PIND, JOY_BUTTON_PIN);
}
