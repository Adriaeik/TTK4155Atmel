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
double Kp;
double Ki;
double Kd;


void game_Init(Game *game){
	game->difficulty = HARD;
	game->lives = 5;
	game->score = 0;
	game->start_game = 0;
}

static void set_difficulty(Difficulty difficulty) {
	switch (difficulty) {
		case EASY:
		Kp = 100;
		Ki = 5;
		Kd = 10;
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

// Funksjon for handtering av game over
static void handle_game_over(void) {
	reset_game();
}


// Hovudfunksjon for spelet - non-blocking versjon
// Bær startes av ein melding fra 
void start_game() {
	static uint8_t remaining_lives = 0;
	static uint8_t game_initialized = 0;

	// Initiering av spelet
	if (!game_initialized) {
		set_difficulty(main_game.difficulty);
		main_game.score = 0;
		remaining_lives = main_game.lives;
		game_initialized = 1;
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
			//printf("Send melding om resterande liv: %d\n\r", remaining_lives);
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