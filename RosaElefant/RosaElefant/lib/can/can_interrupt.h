/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#define ID_JOY_Y_POS	1
#define ID_JOY_X_POS	2
#define ID_JOY_BTN		3
#define ID_LS_POS		4
#define ID_RS_POS		5
#define ID_L_BTN		6
#define ID_R_BTN		7
#define ID_JOY_ANGLE	8


void CAN0_Handler       ( void );






#endif /* CAN_INTERRUPT_H_ */