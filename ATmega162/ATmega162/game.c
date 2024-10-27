/*
 * game.c
 *
 * Created: 27.10.2024 12:46:37
 *  Author: adriaeik
 */ 
#include "game.h"
extern Game main_game;

void game_Init(Game *game){
	game->difficulty = EASY;
	game->lives = 5;
	game->score = 0;
	game->start_game = 0;
	set_difficulty(game->difficulty);
}
void game_over(Game* game){
	game->start_game = 0;
	game_Send(game, ID_GAME_START);
}

void game_Send(Game* game, uint8_t ID){
	CANMessage msg_to_send = {
		.id = ID,
		.data = 1,
		.data = {0, 0, 0, 0, 0, 0, 0, 0}
	};
	switch (ID) {
		case ID_GAME_LIVES:  // ID for lives
			msg_to_send.data[0] = game->lives;

		break;
		case ID_GAME_DIFFICULTY:  // ID for difficulty
			msg_to_send.data[0]= (int)game->difficulty;
		
		break;
		case ID_GAME_START:  // ID for lives
			msg_to_send.data[0] = game->start_game;
	
		break;
		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
	CAN_SendMessage(&msg_to_send);
}
void game_Recive(Game* game, CANMessage* msg) {
			printf("ID: %d\n\r", msg->id);
			
	switch (msg->id) {
		case ID_GAME_LIVES_LEFT:  // ID for lives
			game->lives = msg->data[0];
			#if DEBUG_GAME == 1
			printf("Lives: %d\n\r", game->lives);
			#endif
		break;

		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
}
void set_difficulty(Difficulty difficulty) {
	main_game.difficulty = difficulty;
}