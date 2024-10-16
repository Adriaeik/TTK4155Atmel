/*
 * MultiBoard.c
 *
 * Created: 16.10.2024 09:34:26
 *  Author: adriaeik
 */ 
#include "../include/MultiBoard.h"
void MultiBoard_Init(MultiBoard* board) {
	// Initialiser alle andre verdier
	board->LSpos = 0;
	board->RSpos = 0;
	board->LBtn = 0;
	board->RBtn = 0;
	board->JoyXposCal = 0;
	board->JoyYposCal = 0;
	board->JoyAngle = 0;
	board->JoyBtn = 0;  
	board->JoyYLastAction = 0;

}

void update_board_from_can(MultiBoard* board, CAN_MESSAGE* msg) {
	switch (msg->id) {
		case ID_JOY_Y_POS:
		if (msg->data_length == 2) {
			board->JoyYposCal = (int16_t)(msg->data[0] | (msg->data[1] << 8));
			#if DEBUG_MULTIBOARD == 1
			printf("JoyYpos: %d\n\r", board->JoyYposCal);
			#endif
		}
		break;

		case ID_JOY_X_POS:
		if (msg->data_length == 2) {
			board->JoyXposCal = (int16_t)(msg->data[0] | (msg->data[1] << 8));
			#if DEBUG_MULTIBOARD == 1
			printf("JoyXpos: %d\n\r", board->JoyXposCal);
			#endif
		}
		break;

		case ID_JOY_BTN:
		if (msg->data_length == 1) {
			board->JoyBtn = msg->data[0];
			#if DEBUG_MULTIBOARD == 1
			printf("JoyBtn: %d\n\r", board->JoyBtn);
			#endif
		}
		break;

		case ID_LS_POS:
		if (msg->data_length == 1) {
			board->LSpos = msg->data[0];
			#if DEBUG_MULTIBOARD == 1
			printf("LSpos: %d\n\r", board->LSpos);
			#endif
		}
		break;

		case ID_RS_POS:
		if (msg->data_length == 1) {
			board->RSpos = msg->data[0];
			#if DEBUG_MULTIBOARD == 1
			printf("RSpos: %d\n\r", board->RSpos);
			#endif
		}
		break;

		case ID_L_BTN:
		if (msg->data_length == 1) {
			board->LBtn = msg->data[0];
			#if DEBUG_MULTIBOARD == 1
			printf("LBtn: %d\n\r", board->LBtn);
			#endif
		}
		break;

		case ID_R_BTN:
		if (msg->data_length == 1) {
			board->RBtn = msg->data[0];
			#if DEBUG_MULTIBOARD == 1
			printf("RBtn: %d\n\r", board->RBtn);
			#endif
		}
		break;

		case ID_JOY_ANGLE:
		if (msg->data_length == 2) {
			board->JoyAngle = (int16_t)(msg->data[0] | (msg->data[1] << 8));
			#if DEBUG_MULTIBOARD == 2
			printf("JoyAngle: %d\n\r", board->JoyAngle);
			#endif
		}
		break;

		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
}

