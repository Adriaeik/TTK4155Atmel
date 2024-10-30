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
extern uint16_t score_counter;

void game_Init(Game *game){
	game->difficulty = MEDIUM;
	game->lives = 9;
	game->sensitivity = 5;
	game->score = 0;
	game->start_game = 0;
	game->lives_left = game->lives;
	game->game_initialized = 0;
	set_difficulty(game->difficulty);
}
void game_Start(Game* game){
	game_Send(game, ID_GAME_LIVES);
	game_Send(game, ID_GAME_DIFFICULTY);
	game_Send(game, ID_GAME_START);
	
}

int game_run(){
	if (main_game.start_game == 0)
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
	update_highscore_list(score_counter);
	game->start_game = 0;
	game->lives_left = game->lives;
	game->game_initialized = 0;
	
	//printf("Startgame er nå null \r\n");
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
			game->lives_left = msg->data[0];
			#if DEBUG_GAME == 1
			printf("Lives left: %d\n\r", game->lives_left);
			#endif
		break;
		case ID_GAME_OVER:
			animate_ash_and_pikachu_laughing();
			game_over(&main_game);
			
			
			
			break;
		case ID_GAME_INITIALIZED:
			game->game_initialized = 1;
			
		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
}
void set_difficulty(Difficulty difficulty) {
	main_game.difficulty = difficulty;
}

void print_game_status(void) {
	printf("\n==================== Game Status ====================\n\r");
	printf("| %-15s | %-10s |\n\r", "Attribute", "Value");
	printf("|-----------------|------------|\n\r");
	printf("| %-15s | %-10s |\n\r", "Difficulty", main_game.difficulty == EASY ? "Easy" : "Hard");
	printf("| %-15s | %-10d |\n\r", "Lives", main_game.lives);
	printf("| %-15s | %-10d |\n\r", "Score", main_game.score);
	printf("| %-15s | %-10s |\n\r", "Start Game", main_game.start_game ? "Yes" : "No");
	printf("=====================================================\n\n\r");
}