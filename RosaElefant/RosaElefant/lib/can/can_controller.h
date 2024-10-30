/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>

#define CAN_BRP 34
// baudrate of 125 kbits/s, sjw = 4 TQ, propag = 2 TQ, phase1 = 6 TQ, phase2 = 7 TQ
// VITAS SIN CONFIG: 0x00290561
#define ATSAM_CAN_BR 0x00290561 /*0x00880561*/ /*0x00423123*/// 0x00143156
#define PS1		7
#define PS2		6
#define PROP	2
#define BRP		42
#define SJW		1

//ID
#define ID_JOY_Y_POS	1
#define ID_JOY_X_POS	2
#define ID_JOY_BTN		3
#define ID_LS_POS		4
#define ID_RS_POS		5
#define ID_L_BTN		6
#define ID_R_BTN		7
#define ID_JOY_ANGLE	8
#define ID_GAME_LIVES_LEFT	9
#define ID_GAME_LIVES	10
#define ID_GAME_DIFFICULTY	11
#define ID_GAME_SCORE	12
#define ID_GAME_START	13
#define ID_GAME_OVER	14
#define ID_GAME_INITIALIZED	15


typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(void);
uint8_t can_init_controller(uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

#endif /* CAN_CONTROLLER_H_ */