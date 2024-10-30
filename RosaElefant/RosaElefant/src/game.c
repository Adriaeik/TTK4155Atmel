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


void game_Init(Game *game){
	game->difficulty = EASY;
	game->lives = 5;
	game->score = 0;
	game->start_game = 0;
	set_difficulty(game->difficulty);
}

static void set_difficulty(Difficulty difficulty) {
	switch (difficulty) {
		case EASY:
		Kp = 4;
		Ki = 3;
		Kd = 1;
		break;
		case MEDIUM:
		Kp = 150;
		Ki = 10;
		Kd = 15;
		break;
		case HARD:
		Kp = 200;
		Ki = 15;
		Kd = 20;
		break;
	}
}

static void reset_game(void) {
	reset_pid();
	main_game.score = 0;
}

extern int IR_initialized;
// Funksjon for handtering av game over
static void handle_game_over(void) {
	reset_game();
	IR_initialized = 0;
}

void print_game_status(Game *game) {
	printf("\n==================== Game Status ====================\n\r");
	printf("| %-15s | %-10s |\n\r", "Attribute", "Value");
	printf("|-----------------|------------|\n");
	printf("| %-15s | %-10s |\n\r", "Difficulty", game->difficulty == EASY ? "Easy" : "Hard");
	printf("| %-15s | %-10d |\n\r", "Lives", game->lives);
	printf("| %-15s | %-10d |\n\r", "Score", game->score);
	printf("| %-15s | %-10s |\n\r", "Start Game", game->start_game ? "Yes" : "No");
	printf("=====================================================\n\r");
}
static uint8_t game_initialized = 0;
// Hovudfunksjon for spelet - non-blocking versjon
// Bær startes av ein melding fra 
void start_game() {
	static uint8_t remaining_lives = 0;
	
	//Kalibrer pos
	
	// Initiering av spelet
	if (!game_initialized) {
		set_difficulty(main_game.difficulty);
		calibrate_motor_pos();
		main_game.score = 0;
		remaining_lives = main_game.lives;
		game_initialized = 1;
		print_game_status(&main_game);
		printf("\n\r %d\n\r", main_game.lives);
	}

	// Sjekk om exit-knappen er trykt
	if (board.RBtn == 1) {
		handle_game_over();
		game_initialized = 0;
		return;
	}

	// Her legg vi til logikk for spelets gang, som å sjekke kollisjonar, oppdatere ballen sin posisjon osv.
	if (remaining_lives != main_game.lives - main_game.score) {
		remaining_lives = main_game.lives - main_game.score;

		// Konfigurer meldinga som skal sendast Kanskje sende meir info og?
		CAN_MESSAGE msg = {
			.id = ID_GAME_LIVES_LEFT,
			.data_length = 1,
			.data = {remaining_lives, 0, 0, 0, 0, 0, 0, 0}
		};

		// Send meldinga over CAN med mailboks 0
		if (!can_send(&msg, 0)) {
			printf("Send melding om resterande liv: %d\n\r", remaining_lives);
		}
	}

	// Handter game over dersom ingen liv igjen
	if (remaining_lives == 0) {
		handle_game_over();
		game_initialized = 0;
	}
	
	if (game_initialized == 1){
		run_game();
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
			game_initialized = 0;
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

