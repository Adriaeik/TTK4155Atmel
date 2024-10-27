/*
 * game.h
 *
 * Created: 24.10.2024 18:40:22
 *  Author: eikel
 */ 


#ifndef GAME_H_
#define GAME_H_
#define  DEBUG_GAME 0

//#include "motor.h"
#include "../lib/can/can_controller.h"
#include "MultiBoard.h"
#include "driver_IR.h"


// Definer enum for vanskegrad
typedef enum {
	EASY,
	MEDIUM,
	HARD
} Difficulty;

// Definer Game struct
typedef struct game_t {
	uint8_t lives;
	Difficulty difficulty;
	uint8_t start_game;
	uint16_t score;
} Game;



void game_Init(Game *game);
static void set_difficulty(Difficulty difficulty);
static void reset_game(void);
static void handle_game_over(void);
void start_game();
void update_game_status_from_can(Game* game, CAN_MESSAGE* msg);
void run_game(void);

#endif /* GAME_H_ */