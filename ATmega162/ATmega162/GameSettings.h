/*
 * GameSettings.h
 *
 * Created: 27/10/2024 12:47:29
 *  Author: Ravn
 */ 


#ifndef GAMESETTINGS_H_
#define GAMESETTINGS_H_
#include "utils.h"
#include "game.h"


extern Game main_game;

void settings_init(Game* s);
void add_lives(Game* s);
void add_sensitivity(Game* s);
void add_difficulty(Game* s);
void reset_settings(Game* s);



#endif /* GAMESETTINGS_H_ */