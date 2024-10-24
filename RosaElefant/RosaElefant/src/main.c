/*
 * RosaElefant.c
 *
 * Created: 02.10.2024 10:34:01
 * Author : adriaeik
 */ 

#include "../include/init.h"

Game main_game;

int main(void) {
	init();

	while (1) {
		

		/*	can_send(&msg,0);
			time_spinFor(msecs(1000));*/
		// Lage conditions for dette!
		// kan lage ein ID for start game og sette ein variabel true/false
		// i game objektet
		if (main_game.start_game == 1){
			start_game(); // krever at det er verdia på lives og sånt, skal sendast frå atmega
		}
		
		CAN0_Handler();
		
	}
}