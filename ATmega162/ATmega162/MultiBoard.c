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
	Universal_write(ADC_START, 0x80); // For Chip Enable til ADC, (data her er irrelevant)
	loopUntilBitIsClear(PINB, BUSY_PIN);    // Vent til BUSY g�r lav
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
	board->JoyAngle = 0;
	board->JoyBtn = 0;  // Endra fra en, vi bruker fortsatt 0 for av basert på logikk i MB_Update()
}


void MultiBoard_Update(MultiBoard* board) {
	// Velg kanal for venstre skyvebryter (LSpos)
	Universal_write(ADC_START, 0x00);
	loopUntilBitIsClear(PINB, BUSY_PIN);		// Vent til BUSY g�r lav - klar for � sende p� ny
	board->JoyYpos = Universal_read(ADC_START); //  - CH0 f�rste RD low gir channel 0
	board->JoyXpos = Universal_read(ADC_START); //	- CH1 andre RD low gir channel 1
	board->RSpos = Universal_read(ADC_START);	//	- CH2
	board->LSpos = Universal_read(ADC_START);	//	- CH3
	// Les knappestatus
	board->LBtn = 0<testBit(PINB, LEFT_BUTTON_PIN);
	board->RBtn = 0<testBit(PINB, RIGHT_BUTTON_PIN);
	board->JoyBtn = !(0<testBit(PINB, JOY_BUTTON_PIN));
	// Opretter ein int med pluss og minus slik at vi kan finne riktig vinkel med _UpdateJoystickAngel
	board->JoyYposCal = (int16_t)(board->JoyYpos) - (int16_t)(board->JoyYOrigo);
	board->JoyXposCal = (int16_t)(board->JoyXpos) - (int16_t)(board->JoyXOrigo);
	///IKKJE TESTA
	MultiBoard_UpdateJoystickAngle(board);
}

/// IKKJE TESTA 
void MultiBoard_UpdateJoystickAngle(MultiBoard* board) {
	int16_t x = board->JoyXposCal;
	int16_t y = board->JoyYposCal;

	// Unng� divisjon med 0 (n�r senterpunkt) + hysterese
	if (!(abs(x) > JOY_ANGLE_HYSTERESIS || abs(y) >  JOY_ANGLE_HYSTERESIS)) {
		board->JoyAngle = 0;  // Midtpunkt, sett vinkelen til 0
		} else {
		// Bestem kvadrant basert p� verdiane til x og y
		switch ((x >= 0) << 1 | (y >= 0)) {
			case 0b11:  // F�rste kvadrant (x >= 0, y >= 0)
			board->JoyAngle = (y * 90) / (x + y);
			break;
			case 0b01:  // Andre kvadrant (x < 0, y >= 0)
			board->JoyAngle = 90 + ((-x * 90) / (-x + y));
			break;
			case 0b00:  // Tredje kvadrant (x < 0, y < 0)
			board->JoyAngle = 180 + ((-y * 90) / (-x - y));
			break;
			case 0b10:  // Fjerde kvadrant (x >= 0, y < 0)
			board->JoyAngle = 270 + ((x * 90) / (x - y));
			break;
			default:
			board->JoyAngle = 0;  // Feiltilfelle, skal ikkje skje
			break;
		}
	}
}

//void MultiBoard_UpdateJoystickAngle(MultiBoard* board) {
	//int16_t x = board->JoyXposCal;
	//int16_t y = board->JoyYposCal;
//
	//// Unng� divisjon med 0 (n�r senterpunkt)
	//if (!(abs(x) > 10 || abs(y) > 10)) {
		//board->JoyAngle = 0;  // Midtpunkt, sett vinkelen til 0
		//} else {
		//// Beregn vinkelen basert p� kvadrantar
		//if (x >= 0 && y >= 0) {
			//// F�rste kvadrant
			//board->JoyAngle = (y * 90) / (x + y);  // Enklare tiln�rming
			//} else if (x < 0 && y >= 0) {
			//// Andre kvadrant
			//board->JoyAngle = 90 + ((-x * 90) / (-x + y));
			//} else if (x < 0 && y < 0) {
			//// Tredje kvadrant
			//board->JoyAngle = 180 + ((-y * 90) / (-x - y));
			//} else if (x >= 0 && y < 0) {
			//// Fjerde kvadrant
			//board->JoyAngle = 270 + ((x * 90) / (x - y));
		//}
	//}
//}
