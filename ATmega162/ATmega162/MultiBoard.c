/*
 * MultiBoard.c
 *
 * Created: 14.09.2024 11:00:59
 *  Author: ravneb
 */ 
#include "MultiBoard.h"
#include "DriverCAN.h"

void MultiBoard_Init(MultiBoard* board) {
	// Set pinner for knapper som input
	clearBit(DDRD, LEFT_BUTTON_PIN);  // Set Left button pin as input
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
	board->JoyYLastAction = 0;
	
	//FLAG
	// Initialiser alle andre verdier
	board->LSpos_l_can = 0;
	board->RSpos_l_can = 0;
	board->LBtn_l_can = 0;
	board->RBtn_l_can = 0;
	board->JoyBtn_l = 0;
	board->JoyBtn_l_can = 0;
	board->JoyXposCal_l_can = 0;
	board->JoyYposCal_l_can = 0;
	board->JoyAngle_l_can = 0;
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
	board->LBtn = 0<testBit(PIND, LEFT_BUTTON_PIN);
	board->RBtn = 0<testBit(PINB, RIGHT_BUTTON_PIN);
	board->JoyBtn = !(0<testBit(PINB, JOY_BUTTON_PIN));
	// Opretter ein int med pluss og minus slik at vi kan finne riktig vinkel med _UpdateJoystickAngel
	board->JoyYposCal = (int16_t)(board->JoyYpos) - (int16_t)(board->JoyYOrigo);
	board->JoyXposCal = (int16_t)(board->JoyXpos) - (int16_t)(board->JoyXOrigo);
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

void MultiBoard_Send(MultiBoard* board){
	// CAN-melding å sende
	CANMessage msg_to_send;
	/* KAn vere fornuftig å slå sammen litt sia vi bare bruker 1 til 2 bytes per variabel*/
	// Sjekk for alle verdier som har endra seg
	if (abs(board->JoyYposCal - board->JoyYposCal_l_can) > 2) {
		msg_to_send.id = ID_JOY_Y_POS;
		msg_to_send.length = 2; // Vi sender 2 bytes for int16_t
		msg_to_send.data[0] = (uint8_t)(board->JoyYposCal & 0xFF); // LSB
		msg_to_send.data[1] = (uint8_t)((board->JoyYposCal >> 8) & 0xFF); // MSB
		CAN_SendMessage(&msg_to_send);
		board->JoyYposCal_l_can = board->JoyYposCal;
	}
	
	if (abs(board->JoyXposCal - board->JoyXposCal_l_can) > 2) {
		msg_to_send.id = ID_JOY_X_POS;
		msg_to_send.length = 2; // Vi sender 2 bytes for int16_t
		msg_to_send.data[0] = (uint8_t)(board->JoyXposCal & 0xFF); // LSB
		msg_to_send.data[1] = (uint8_t)((board->JoyXposCal >> 8) & 0xFF); // MSB
		CAN_SendMessage(&msg_to_send);
		board->JoyXposCal_l_can = board->JoyXposCal;
	}
	
	if (board->LBtn != board->LBtn_l_can) {
		msg_to_send.id = ID_L_BTN;
		msg_to_send.length = 1;
		msg_to_send.data[0] = board->LBtn;
		CAN_SendMessage(&msg_to_send);
		board->LBtn_l_can = board->LBtn;
	}
	
	if (board->RBtn != board->RBtn_l_can) {
		msg_to_send.id = ID_R_BTN;
		msg_to_send.length = 1;
		msg_to_send.data[0] = board->RBtn;
		CAN_SendMessage(&msg_to_send);
		board->RBtn_l_can = board->RBtn;
	}
	
/* PROBLEMER!!! sikkert eigentlig kjempe lett, men fuck det */
/*
	if (board->JoyBtn != board->JoyBtn_l_can) {
		msg_to_send.id = ID_JOY_BTN;
		msg_to_send.length = 1;
		msg_to_send.data[0] = board->JoyBtn;
		CAN_SendMessage(&msg_to_send);
		board->JoyBtn_l_can = !board->JoyBtn;
	}
*/
	if (abs(board->RSpos - board->RSpos_l_can) >2) {
		msg_to_send.id = ID_RS_POS;
		msg_to_send.length = 1;
		msg_to_send.data[0] = board->RSpos;
		CAN_SendMessage(&msg_to_send);
		board->RSpos_l_can = board->RSpos;
	}
	
	if (abs(board->LSpos - board->LSpos_l_can) >2) {
		msg_to_send.id = ID_LS_POS;
		msg_to_send.length = 1;
		msg_to_send.data[0] = board->LSpos;
		CAN_SendMessage(&msg_to_send);
		board->LSpos_l_can = board->LSpos;
	}

	if (board->JoyAngle != board->JoyAngle_l_can) {
		msg_to_send.id = ID_JOY_ANGLE;
		msg_to_send.length = 2;
		msg_to_send.data[0] = (uint8_t)(board->JoyAngle & 0xFF);
		msg_to_send.data[1] = (uint8_t)((board->JoyAngle >> 8) & 0xFF);
		CAN_SendMessage(&msg_to_send);
		board->JoyAngle_l_can = board->JoyAngle;
	}
}