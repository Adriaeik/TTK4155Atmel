/*
 * game.c
 *
 * Created: 24.10.2024 18:40:03
 *  Author: eikel
 */ 
#include "../include/game.h"
// Globale variablar
Game main_game;
MultiBoard board;
long long int Kp;
long long int Ki;
long long int Kd;
extern int inverted_controll = 0;


void game_Init(Game *game){
	game->difficulty = EASY;
	game->lives = 5;
	game->score = 0;
	game->start_game = 0;
	game->initialized = 0;
	game->remaining_lives = game->lives;
	set_difficulty(game->difficulty);
}

static void set_difficulty(Difficulty difficulty) {
	switch (difficulty) {
		case EASY:
		Kp = 4;
		Ki = 3;
		Kd = 1;
		inverted_controll = 0;
		break;
		case MEDIUM:
		Kp = 4;
		Ki = 3;
		Kd = 1;
		inverted_controll = 1;
		break;
		case HARD:
		Kp = 15;
		Ki = 1;
		Kd = 5;
		inverted_controll = 0;
		break;
	}
}

static void reset_game(void) {
	reset_pid();
	main_game.score = 0;
	main_game.remaining_lives = main_game.lives;
}
static void handle_game_over(void) {
	reset_game();
	main_game.start_game = 0;
	game_Send(&main_game, ID_GAME_OVER);
	
	
}
// Hovudfunksjon for spelet - non-blocking versjon
// Bær startes av ein melding fra 
#include "../include/servo.h"
void start_game() {
	if (main_game.start_game == 1){
		// Initiering av spelet
		if (!main_game.initialized) {
			set_difficulty(main_game.difficulty);
			calibrate_motor_pos();
			main_game.score = 0;
			main_game.remaining_lives = main_game.lives;
			PIOC->PIO_SODR = SOLENOID_PIN; // ta inn stempelet
			print_game_status(&main_game);
			game_Send_lives();
			main_game.initialized = 1;
			game_Send(&main_game, ID_GAME_INITIALIZED);
			printf("INITIALIZED");
			board.RBtn = 0;
		}
		// Sjekk om exit-knappen er trykt
		if (board.RBtn == 1) {
			handle_game_over();
			main_game.initialized = 0;
			return;
		}
		
		if (main_game.remaining_lives != main_game.lives - main_game.score) {
			main_game.remaining_lives = main_game.lives - main_game.score;
			game_Send_lives();
		}
		// Handter game over dersom ingen liv igjen
		if (main_game.remaining_lives == 0) {
			handle_game_over();
			main_game.initialized = 0; // SKAL SKJE I handle_game_over
		}
		
		if (main_game.initialized == 1){
			run_game();
		}
		
	} else{
		//game_Init(&main_game);
		//print_game_status(&main_game);
		//printf("vi er i else: strt game: %d \n\r", main_game.start_game);
		
	}
}

// Oppdater Game status frå CAN-melding
void update_game_status_from_can(Game* game, CAN_MESSAGE* msg) {
	printf("CAN_ ID: %d \n\r", msg->id);
	switch (msg->id) {
		case ID_GAME_LIVES:  // ID for lives
		if (msg->data_length == 1) {
			game->lives = msg->data[0];
			#if DEBUG_GAME == 1
			printf("Lives: %d\n\r", game->lives);
			#endif
		}
		break;
		case ID_GAME_DIFFICULTY:  // ID for difficulty
		if (msg->data_length == 1) {
			game->difficulty = (Difficulty)msg->data[0];
			set_difficulty(game->difficulty);
			#if DEBUG_GAME == 1
			printf("Difficulty set to: %d\n\r", game->difficulty);
			#endif
		}
		break;
		case ID_GAME_SCORE:  // ID for score
		if (msg->data_length == 2) {
			game->score = (uint16_t)(msg->data[0] | (msg->data[1] << 8));
			#if DEBUG_GAME == 1
			printf("Score: %d\n\r", game->score);
			#endif
		}
		case ID_GAME_START:  // ID for lives
		if (msg->data_length == 1) {
			game->start_game = msg->data[0];
			main_game.initialized = 0;
			#if DEBUG_GAME == 1
			printf("Lives: %d\n\r", game->start_game);
			#endif
		}
		
		break;
		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
}

void run_game(void){
	IR_Handler();
	servo_set_position_joy();
	motor_control_PID();
}

void game_Send(Game* game, uint8_t ID){
	CAN_MESSAGE msg_to_send = {
		.id = ID,
		.data_length = 1,
		.data = {0, 0, 0, 0, 0, 0, 0, 0}
	};
	switch (ID) {
		case ID_GAME_LIVES:  // ID for lives
		msg_to_send.data[0] = game->lives;

		break;
		case ID_GAME_DIFFICULTY:  // ID for difficulty
		msg_to_send.data[0]= (int)game->difficulty;
		
		break;
		case ID_GAME_LIVES_LEFT:
		msg_to_send.data[0] = game->remaining_lives;
		
		case ID_GAME_OVER:
		msg_to_send.data[0] = game->start_game;
		
		case ID_GAME_INITIALIZED:
		msg_to_send.data[0] = game->initialized;
		
		break;
		default:
		// Håndter ukjente CAN-meldinger her, om nødvendig
		break;
	}
	can_send(&msg_to_send, 0);
}

void game_Send_lives(void){
	CAN_MESSAGE msg = {
		.id = ID_GAME_LIVES_LEFT,
		.data_length = 1,
		.data = {main_game.remaining_lives, 0, 0, 0, 0, 0, 0, 0}
	};
	
	// Send meldinga over CAN med mailboks 0
	if (!can_send(&msg, 0)) {
		printf("Send melding om resterande liv: %d\n\r",main_game.remaining_lives);
	}
}

#include <stdio.h>
void print_game_status(Game *game) {
	int diff = (int)game->difficulty;
	int liv = game->lives;
	int liv_left = game->remaining_lives;
	int sco = game->score;
	int start = game->start_game;
	
	printf("\n==================== Game Status ====================\n\r");
	printf("| %-15s | %-10s |\n\r", "Attribute", "Value");
	printf("|-----------------|------------|\n\r");
	printf("");
	printf("| %-15s | %-10s |\n\r", "Difficulty", diff == 0 ? "Easy" : "Hard");
	printf("| %-15s | %-10d |\n\r", "Lives", liv);
	printf("| %-15s | %-10d |\n\r", "Lives left", liv_left);
	printf("| %-15s | %-10d |\n\r", "Score", sco);
	printf("| %-15s | %-10s |\n\r", "Start Game", start ? "Yes" : "No");
	printf("=====================================================\n\r");
}