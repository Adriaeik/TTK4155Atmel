/*
 * game.c
 *
 * Created: 27.10.2024 12:46:37
 *  Author: adriaeik
 */ 
#include "game.h"
#include "MultiBoard.h"

extern Game main_game;
extern MultiBoard board;

void game_Init(Game *game){
	game->difficulty = EASY;
	game->lives = 5;
	game->sensitivity = 5;
	game->score = 0;
	game->start_game = 0;
	set_difficulty(game->difficulty);
}
void game_Start(Game* game){
	game_Send(game, ID_GAME_LIVES);
	game_Send(game, ID_GAME_DIFFICULTY);
	game_Send(game, ID_GAME_START);
	
}

int game_run(){
	if (main_game.lives == 0)
	{
		main_game.start_game = 0;
		
		return 0; //avslutt
	}
	oled_write_screen_to_SRAM(&solkors);
	MultiBoard_Update(&board);
	MultiBoard_Send(&board);
	return 1;//fortsett å kjøre
}
void game_over(Game* game){
	game->start_game = 0;
	game_Send(game, ID_GAME_START);
}

void game_Send(Game* game, uint8_t ID){
	CANMessage msg_to_send = {
		.id = ID,
		.length = 1,
		.data = {0, 0, 0, 0, 0, 0, 0, 0}
	};
	switch (ID) {
		case ID_GAME_LIVES:  // ID for lives
			msg_to_send.data[0] = game->lives;

		break;
		case ID_GAME_DIFFICULTY:  // ID for difficulty
			msg_to_send.data[0]= (int)game->difficulty;
		
		break;
		case ID_GAME_START: 
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

void print_game_status(Game *game) {
	printf("\n==================== Game Status ====================\n\r");
	printf("| %-15s | %-10s |\n\r", "Attribute", "Value");
	printf("|-----------------|------------|\n\r");
	printf("| %-15s | %-10s |\n\r", "Difficulty", game->difficulty == EASY ? "Easy" : "Hard");
	printf("| %-15s | %-10d |\n\r", "Lives", game->lives);
	printf("| %-15s | %-10d |\n\r", "Score", game->score);
	printf("| %-15s | %-10s |\n\r", "Start Game", game->start_game ? "Yes" : "No");
	printf("=====================================================\n\n\r");
}