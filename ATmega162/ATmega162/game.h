/*
 * game.h
 *
 * Created: 27.10.2024 12:46:50
 *  Author: adriaeik
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "DriverCAN.h"

#define DEBUG_GAME 1

// Definer enum for vanskegrad
typedef enum {
	EASY,
	MEDIUM,
	HARD
} Difficulty;

// Definer Game struct
typedef struct game_t {
	uint8_t lives;
	uint8_t difficulty;
	uint8_t start_game;
	uint16_t score;
	uint8_t sensitivity;
} Game;
extern Game main_game;

void game_Init(Game *game);
void game_Start(Game *game);
void game_over(Game* game);
void game_Send(Game* game, uint8_t ID);
void game_Recive(Game* game, CANMessage* msg);
void set_difficulty(Difficulty difficulty);
int game_run(void);
void print_game_status(void);
#endif /* GAME_H_ */